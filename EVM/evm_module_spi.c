#include "evm_module.h"
#include "./flash/bsp_spi_flash.h"

static SPI_TypeDef* spi[3] = {SPI1,SPI2,SPI3};
static SPI_HandleTypeDef eSpiHandle[3] = {0};

static evm_val_t evm_module_spi_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v);

void setCSLevel(uint8_t bus,uint8_t state)
{
	if(bus==1)
	{
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(state==0)?GPIO_PIN_RESET:GPIO_PIN_SET);
	}else if(bus == 2){}
	else if(bus == 3){}
}

//void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
//{
//	GPIO_InitTypeDef  GPIO_InitStruct;

//	/*##-1- Enable peripherals and GPIO Clocks #################################*/
//	/* Enable GPIO TX/RX clock */
//	if(hspi->Instance == SPI1)
//	{
//		__HAL_RCC_GPIOA_CLK_ENABLE();
//		/* Enable SPI clock */
//		__HAL_RCC_SPI1_CLK_ENABLE(); 
//		
//		/* SPI SCK GPIO pin configuration  */
//		GPIO_InitStruct.Pin       = GPIO_PIN_5;
//		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//		GPIO_InitStruct.Pull      = GPIO_PULLUP;
//		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

//		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//		/* SPI MISO GPIO pin configuration  */
//		GPIO_InitStruct.Pin = GPIO_PIN_6;  
//		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//		/* SPI MOSI GPIO pin configuration  */
//		GPIO_InitStruct.Pin = GPIO_PIN_7; 
//		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   

//		GPIO_InitStruct.Pin = GPIO_PIN_4 ;
//		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
//		
//	}else if(hspi->Instance == SPI2){}
//	else if(hspi->Instance == SPI3){}

//}

//SPI(bus, ...)
static evm_val_t evm_module_spi(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc == 0 || argc>9 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v)) EVM_ARG_TYPE_ERR;
	
	uint8_t bus = evm_2_integer(v);
	if(bus==0 || bus >3) EVM_ARG_TYPE_ERR;
	
	evm_prop_set_value(e, p, ".bus", bus);
	
	if (argc>1) evm_module_spi_init(e,p,argc-1,v+1);

	return EVM_VAL_UNDEFINED;
}

//SPI.deinit()
static evm_val_t evm_module_spi_deinit(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	uint8_t bus = evm_2_integer(evm_prop_get(e, p, ".bus", 0));

	HAL_SPI_DeInit(&eSpiHandle[bus-1]);
	return EVM_VAL_UNDEFINED;
}

//SPI.init(mode, baudrate=328125, *, prescaler, polarity=1, phase=0, bits=8, firstbit=SPI.MSB, ti=False, crc=None)
static evm_val_t evm_module_spi_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>8) EVM_ARG_LENGTH_ERR;
	uint32_t mode = SPI_MODE_MASTER;
	uint32_t prescaler = SPI_BAUDRATEPRESCALER_4;
	uint32_t polarity = 1;
	uint32_t phase = 0;
	uint32_t bits = 8;
	uint32_t firstbit = 0;
	uint32_t ti = 0;
	uint32_t crc = SPI_CRCCALCULATION_DISABLE;
	
	if(argc >=1) mode = evm_2_integer(v);
	if(argc >=2) prescaler = evm_2_integer(v+1);
	if(argc >=3) polarity = evm_2_integer(v+2);
	if(argc >=4) phase = evm_2_integer(v+3);
	if(argc >=5) bits = evm_2_integer(v+4);
	if(argc >=6) firstbit = evm_2_integer(v+5);
	if(argc >=7) ti = evm_2_integer(v+6);
	if(argc ==8) crc = evm_2_integer(v+7);

	uint8_t bus = evm_2_integer(evm_prop_get(e, p, ".bus", 0));

	
	eSpiHandle[bus-1].Instance               = spi[bus-1];
	eSpiHandle[bus-1].Init.BaudRatePrescaler = prescaler;
	eSpiHandle[bus-1].Init.Direction         = SPI_DIRECTION_2LINES;
	eSpiHandle[bus-1].Init.CLKPhase          = phase;
	eSpiHandle[bus-1].Init.CLKPolarity       = polarity;
	eSpiHandle[bus-1].Init.CRCCalculation    = crc;
	eSpiHandle[bus-1].Init.CRCPolynomial     = 7;
	eSpiHandle[bus-1].Init.DataSize          = bits;
	eSpiHandle[bus-1].Init.FirstBit          = firstbit;
	eSpiHandle[bus-1].Init.NSS               = SPI_NSS_SOFT;
	eSpiHandle[bus-1].Init.TIMode            = ti;
	eSpiHandle[bus-1].Init.Mode 			 = mode;
	
	HAL_SPI_Init(&eSpiHandle[bus-1]); 
	__HAL_SPI_ENABLE(&eSpiHandle[bus-1]); 
	
	return EVM_VAL_UNDEFINED;
}

//SPI.recv(recv, *, timeout=5000)
static evm_val_t evm_module_spi_recv(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>2) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v) && ! evm_is_buffer(v)) EVM_ARG_TYPE_ERR;
	
	uint32_t timeout = 5000;
	if(argc == 2)
	{
		if(!evm_is_number(v+1)) EVM_ARG_TYPE_ERR;
		timeout = (uint32_t)evm_2_integer(v+1); 
	}	
	
	uint8_t bus = evm_2_integer(evm_prop_get(e, p, ".bus", 0));
	
	if(evm_is_number(v)){
		uint16_t recv_size = evm_2_integer(v);
		uint8_t *recv = NULL;
		recv = (uint8_t *)malloc(sizeof(uint8_t)*recv_size);
		if(HAL_SPI_Receive(&eSpiHandle[bus-1],recv,recv_size,timeout)!=HAL_OK) 
		{
			free(recv);
			return evm_mk_null();
		}
		
		evm_val_t *ebuff = evm_buffer_create(e,recv_size);
		evm_buffer_set(ebuff,recv,0,recv_size);
		free(recv);
		return *ebuff;
	}else{
		uint8_t *recv = evm_buffer_addr(v);
		uint16_t recv_size = evm_buffer_len(v);
		recv = (uint8_t *)malloc(recv_size);
		if(HAL_SPI_Receive(&eSpiHandle[bus-1],recv,recv_size,timeout)!=HAL_OK) 
		{
			free(recv);
			return evm_mk_null();
		}
		free(recv);
		return evm_mk_number(recv_size);
	}
}

//SPI.send(send, *, timeout=5000)
static evm_val_t evm_module_spi_send(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>2) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v) && ! evm_is_buffer(v)) EVM_ARG_TYPE_ERR;
	
	uint32_t timeout = 5000;
	if(argc == 2)
	{
		if(!evm_is_number(v+1)) EVM_ARG_TYPE_ERR;
		timeout = (uint32_t)evm_2_integer(v+1); 
	}	
	
	uint8_t bus = evm_2_integer(evm_prop_get(e, p, ".bus", 0));
	
	if(evm_is_number(v)){
		uint16_t recv_size = evm_2_integer(v);
		uint8_t *recv = NULL;
		recv = (uint8_t *)malloc(sizeof(uint8_t)*recv_size);
		if(HAL_SPI_Transmit(&eSpiHandle[bus-1],recv,recv_size,timeout)!=HAL_OK) 
		{
			free(recv);
			return evm_mk_null();
		}
		
		evm_val_t *ebuff = evm_buffer_create(e,recv_size);
		evm_buffer_set(ebuff,recv,0,recv_size);
		free(recv);
		return *ebuff;
	}else{
		uint8_t *recv = evm_buffer_addr(v);
		uint16_t recv_size = evm_buffer_len(v);
		recv = (uint8_t *)malloc(recv_size);
		if(HAL_SPI_Transmit(&eSpiHandle[bus-1],recv,recv_size,timeout)!=HAL_OK) 
		{
			free(recv);
			return evm_mk_null();
		}
		
		free(recv);
		return evm_mk_number(recv_size);
	}
}

//SPI.send_recv(send, recv=None, *, timeout=5000)
static evm_val_t evm_module_spi_send_recv(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>3) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v) && ! evm_is_buffer(v)) EVM_ARG_TYPE_ERR;
	
	uint32_t timeout = 5000;
	if(argc == 3)
	{
		if(!evm_is_number(v+1)) EVM_ARG_TYPE_ERR;
		timeout = (uint32_t)evm_2_integer(v+1); 
	}	
	
	uint8_t bus = evm_2_integer(evm_prop_get(e, p, ".bus", 0));

	uint8_t *send = evm_buffer_addr(v);
	uint16_t send_size = evm_buffer_len(v);
	
	uint8_t *recv = evm_buffer_addr(v+1);
	uint16_t recv_size = evm_buffer_len(v+1);
	recv = (uint8_t *)malloc(recv_size);
	if(HAL_SPI_TransmitReceive(&eSpiHandle[bus-1],send,recv,send_size,timeout)!=HAL_OK) 
	{
		free(recv);
		return evm_mk_null();
	}
	evm_val_t *ebuff = evm_buffer_create(e,recv_size);
	evm_buffer_set(ebuff,recv,0,recv_size);
	free(recv);
	return *ebuff;
}

evm_val_t evm_class_spi(evm_t * e){
	evm_builtin_t class_spi[] = {
		{"deinit", evm_mk_native( (intptr_t)evm_module_spi_deinit )},
		{"init", evm_mk_native( (intptr_t)evm_module_spi_init )},
		{"recv", evm_mk_native( (intptr_t)evm_module_spi_recv )},
		{"send", evm_mk_native( (intptr_t)evm_module_spi_send )},
		{"send_recv", evm_mk_native( (intptr_t)evm_module_spi_send_recv )},
		{"MASTER",evm_mk_number((uint32_t)SPI_MODE_MASTER)},
		{"SLAVE",evm_mk_number((uint32_t)SPI_MODE_SLAVE)},
		{"LSB",evm_mk_number((uint32_t)SPI_FIRSTBIT_LSB)},
		{"MSB",evm_mk_number((uint32_t)SPI_FIRSTBIT_MSB)},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_spi, class_spi, NULL);
}


