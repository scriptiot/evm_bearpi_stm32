#include "evm_module.h"

#define UART_NUM 2

typedef struct REC_FIFO_STRUCT{
	uint32_t head;
	uint32_t tail;
	uint32_t buflen;
	uint8_t init;
	uint8_t *rx;
}REC_FIFO;

static USART_TypeDef* uart[UART_NUM] = {USART1,USART2};
static UART_HandleTypeDef eUartHandle[UART_NUM] = {0};
static REC_FIFO recv_fifo[UART_NUM] = {0};

static evm_val_t evm_module_uart_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v);

//UART(portNum)
static evm_val_t evm_module_uart(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc == 0 || argc>9 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v)) EVM_ARG_TYPE_ERR;
	
	uint8_t portnum = evm_2_integer(v);
	if(portnum==0 || portnum >UART_NUM) EVM_ARG_TYPE_ERR;
	
	evm_prop_set_value(e, p, ".portnum", *v);
	
	evm_module_uart_init(e,p,argc-1,v+1);
	
	return EVM_VAL_UNDEFINED;
}

//UART.init(baudrate, bits=8, parity=None, stop=1, *, timeout=0, flow=0, timeout_char=0, read_buf_len=64)
static evm_val_t evm_module_uart_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>8) EVM_ARG_LENGTH_ERR;
	
	uint32_t baudrate = 115200;
	uint32_t bits = UART_WORDLENGTH_8B;
	uint32_t parity = UART_PARITY_NONE;
	uint32_t stop = UART_STOPBITS_1;
	uint32_t flow = UART_HWCONTROL_NONE;
	uint32_t time_out = 0;
	uint32_t timeout_char = 0;
	
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));

	
	if(argc >=1) baudrate = evm_2_integer(v);
	if(argc >=2) bits = evm_2_integer(v+1);
	if(argc >=3) parity = evm_2_integer(v+2);
	if(argc >=4) stop = evm_2_integer(v+3);
	if(argc >=5) time_out = evm_2_integer(v+4);
	if(argc >=6) flow = evm_2_integer(v+5);
	if(argc >=7) timeout_char = evm_2_integer(v+6);
	if(argc ==8) recv_fifo[portnum-1].buflen = evm_2_integer(v+7);

	
	eUartHandle[portnum-1].Instance          = uart[portnum-1];
	eUartHandle[portnum-1].Init.BaudRate     = baudrate;
	eUartHandle[portnum-1].Init.WordLength   = bits;
	eUartHandle[portnum-1].Init.StopBits     = stop;
	eUartHandle[portnum-1].Init.Parity       = parity;
	eUartHandle[portnum-1].Init.HwFlowCtl    = flow;
	eUartHandle[portnum-1].Init.Mode         = UART_MODE_TX_RX;
	HAL_UART_Init(&eUartHandle[portnum-1]);
	
	__HAL_UART_ENABLE_IT(&eUartHandle[portnum-1],UART_IT_RXNE);  

	if(recv_fifo[portnum-1].init==1) free(recv_fifo[portnum-1].rx);
	recv_fifo[portnum-1].rx = (uint8_t *)malloc(sizeof(uint8_t)*recv_fifo[portnum-1].buflen);
	recv_fifo[portnum-1].init = 1;
	
	evm_prop_set_value(e, p, ".read_buf_len", evm_mk_number(recv_fifo[portnum-1].buflen));
	evm_prop_set_value(e, p, ".time_out", evm_mk_number(time_out));

	return EVM_VAL_UNDEFINED;
}

//UART.deinit()
static evm_val_t evm_module_uart_deinit(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	
	__HAL_UART_DISABLE_IT(&eUartHandle[portnum-1],UART_IT_RXNE);  
	HAL_UART_DeInit(&eUartHandle[portnum-1]);

	recv_fifo[portnum-1].buflen = 0;
	recv_fifo[portnum-1].head = 0;
	recv_fifo[portnum-1].init = 0;
	recv_fifo[portnum-1].tail = 0;
	free(recv_fifo[portnum-1].rx);
	return EVM_VAL_UNDEFINED;
}

//UART.any()
static evm_val_t evm_module_uart_any(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0) EVM_ARG_LENGTH_ERR;

	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	uint32_t read_buf_len = (uint32_t)evm_2_integer(evm_prop_get(e, p, ".read_buf_len", 0));

	return (recv_fifo[portnum-1].tail + read_buf_len - recv_fifo[portnum-1].head)%read_buf_len;
}

//UART.read([nbytes])
static evm_val_t evm_module_uart_read(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc <= 1) {
		uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
		uint8_t timeout = evm_2_integer(evm_prop_get(e, p, ".timeout", 0));

		uint32_t read_buf_len = (uint32_t)evm_2_integer(evm_prop_get(e, p, ".read_buf_len", 0));
		
		uint32_t len = 0;
		uint32_t time_start = HAL_GetTick();

		if(argc == 0) 
		{
			while(HAL_GetTick() - time_start < timeout);//等待超时
			len = (recv_fifo[portnum-1].head + read_buf_len - recv_fifo[portnum-1].tail)%read_buf_len;
		}
		else {
			uint32_t nbytes = (uint32_t)evm_2_integer(v);
			uint32_t fifolen = 0;
			while(HAL_GetTick() - time_start < timeout)
			{
				fifolen = (recv_fifo[portnum-1].head + read_buf_len - recv_fifo[portnum-1].tail)%read_buf_len;
				if(fifolen >= nbytes) break;
			}
			if(fifolen < nbytes) return evm_mk_null();
			len = nbytes;
		}
		if(len == 0) return evm_mk_null();

		uint8_t *buff = (uint8_t *)malloc(sizeof(uint8_t)*len);
		if(recv_fifo[portnum-1].tail + len<= recv_fifo[portnum-1].head)
		{
			memcpy(buff,recv_fifo[portnum-1].rx + recv_fifo[portnum-1].tail,len);
		}else{
			memcpy(buff,recv_fifo[portnum-1].rx + recv_fifo[portnum-1].tail,read_buf_len - recv_fifo[portnum-1].tail);
			memcpy(buff + read_buf_len - recv_fifo[portnum-1].tail,recv_fifo[portnum-1].rx,len - read_buf_len + recv_fifo[portnum-1].tail);
		}
		recv_fifo[portnum-1].tail = (recv_fifo[portnum-1].tail+len)%read_buf_len;
		
		evm_val_t *ebuff = evm_buffer_create(e,len);
		evm_buffer_set(ebuff,buff,0,len);
		
		free(buff);
		return *ebuff;
	}else EVM_ARG_LENGTH_ERR;
}

//UART.readchar()
static evm_val_t evm_module_uart_readchar(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;
	
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	uint8_t timeout = evm_2_integer(evm_prop_get(e, p, ".timeout", 0));
	
	uint32_t read_buf_len = (uint32_t)evm_2_integer(evm_prop_get(e, p, ".read_buf_len", 0));
	uint32_t fifolen = (recv_fifo[portnum-1].head + read_buf_len - recv_fifo[portnum-1].tail)%read_buf_len;
	
	if(fifolen == 0){
		uint32_t time_start = HAL_GetTick();
		while(HAL_GetTick() - time_start < timeout){
			fifolen = (recv_fifo[portnum-1].head + read_buf_len - recv_fifo[portnum-1].tail)%read_buf_len;
			if(fifolen > 0) break;
		}
		if(fifolen ==0) return evm_mk_null();
	}
	
	uint8_t data = recv_fifo[portnum-1].rx[recv_fifo[portnum-1].tail];
	recv_fifo[portnum-1].tail = (recv_fifo[portnum-1].tail+1)%read_buf_len;
	return evm_mk_number(data);	
}

//UART.readinto(buf[, nbytes])
static evm_val_t evm_module_uart_readinto(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//UART.readline()
static evm_val_t evm_module_uart_readline(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//UART.write(buf)
static evm_val_t evm_module_uart_write(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc !=1 ) EVM_ARG_LENGTH_ERR;
	if(evm_is_buffer(v)) EVM_ARG_TYPE_ERR;

	uint8_t *buf = evm_buffer_addr(v);
	uint32_t buf_len = evm_buffer_len(v);
	
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	uint8_t timeout = evm_2_integer(evm_prop_get(e, p, ".timeout", 0));
	
	if(HAL_UART_Transmit(&eUartHandle[portnum-1],buf,buf_len,timeout)!= HAL_OK) return evm_mk_null();
	return evm_mk_number(buf_len);
}

//UART.writechar(char)
static evm_val_t evm_module_uart_writechar(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc !=1 ) EVM_ARG_LENGTH_ERR;
	if(evm_is_integer(v)) EVM_ARG_TYPE_ERR;

	uint8_t buf[1] = {evm_2_integer(v)};
	
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	uint8_t timeout = evm_2_integer(evm_prop_get(e, p, ".timeout", 0));
	
	HAL_UART_Transmit(&eUartHandle[portnum-1],buf,1,timeout);
	return evm_mk_null();
}

//UART.sendbreak()
static evm_val_t evm_module_uart_sendbreak(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	uint8_t portnum = evm_2_integer(evm_prop_get(e, p, ".portnum", 0));
	HAL_LIN_SendBreak(&eUartHandle[portnum-1]);
	return evm_mk_null();
}

evm_val_t evm_class_uart(evm_t * e){
	evm_builtin_t class_uart[] = {
		{"init", evm_mk_native( (intptr_t)evm_module_uart_init )},
		{"deinit", evm_mk_native( (intptr_t)evm_module_uart_deinit )},
		{"any", evm_mk_native( (intptr_t)evm_module_uart_any )},
		{"read", evm_mk_native( (intptr_t)evm_module_uart_read )},
		{"readchar", evm_mk_native( (intptr_t)evm_module_uart_readchar )},
//		{"readinto", evm_mk_native( (intptr_t)evm_module_uart_readinto )},
//		{"readline", evm_mk_native( (intptr_t)evm_module_uart_readline )},
		{"write", evm_mk_native( (intptr_t)evm_module_uart_write )},
		{"writechar", evm_mk_native( (intptr_t)evm_module_uart_writechar )},
		{"sendbreak", evm_mk_native( (intptr_t)evm_module_uart_sendbreak )},
		{"RTS",evm_mk_number((uint32_t)UART_HWCONTROL_RTS)},
		{"CTS",evm_mk_number((uint32_t)UART_HWCONTROL_CTS)},
		
		{".portnum",evm_mk_number(0)},
		{".read_buf_len",evm_mk_number(0)},
		{".timeout",evm_mk_number(0)},

		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_uart, class_uart, NULL);
}

void usart_msphandler(uint8_t uart_index)
{
	uint8_t buff[1] = {0};
	HAL_UART_IRQHandler(&eUartHandle[uart_index]);	//调用HAL库中断处理公用函数

	uint32_t timeout=0;
    while (HAL_UART_GetState(&eUartHandle[uart_index]) != HAL_UART_STATE_READY)//等待就绪
	{
		timeout++;////超时处理
		if(timeout>HAL_MAX_DELAY) break;		
	}
	
	if(HAL_UART_Receive_IT(&eUartHandle[uart_index],buff,1)==HAL_OK)
	{
		recv_fifo[uart_index].rx[recv_fifo[uart_index].head] = buff[0];
		recv_fifo[uart_index].head = (recv_fifo[uart_index].head+1)%recv_fifo[uart_index].buflen;
	}
}

void USART1_IRQHandler(void)
{
	usart_msphandler(0);
}

void USART2_IRQHandler(void)
{
	usart_msphandler(1);
}






