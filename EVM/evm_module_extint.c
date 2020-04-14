#include "evm_module.h"
#include "evm_main.h"
#include "epolling_module.h"

#define PIN_PORT 	".port"
#define PIN_NUMBER 	".number"

#define PORT_NUM	3

int pin_cb_id[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

uint8_t getPinIndex(uint16_t index)
{
	uint8_t cnt = 0;
	for(;;)
	{
		if((index&0xFF)==1){
			break;
		}else index = (index>>1);
		cnt++;
	}
	return cnt;
}

IRQn_Type getIRQn(uint16_t pin_number)
{
	if(pin_number == GPIO_PIN_0) return EXTI0_IRQn;
	else if(pin_number == GPIO_PIN_1) return EXTI1_IRQn;
	else if(pin_number == GPIO_PIN_2) return EXTI2_IRQn;
	else if(pin_number == GPIO_PIN_3) return EXTI3_IRQn;
	else if(pin_number == GPIO_PIN_4) return EXTI4_IRQn;
	else if(pin_number >= GPIO_PIN_5 && pin_number <= GPIO_PIN_9) return EXTI9_5_IRQn; 
	else return EXTI15_10_IRQn; 
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( pin_cb_id[getPinIndex(GPIO_Pin)] != -1 ){
		evm_val_t v = epolling_register_get(my_env, pin_cb_id[getPinIndex(GPIO_Pin)]);
		epolling_add(my_env, v, EVM_VAL_NULL);
    }
}

//ExtInt(pin_port, pin_num, mode, pull, callback)
static evm_val_t evm_module_extint(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 5 ) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v)) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;

	
	GPIO_TypeDef* port = (GPIO_TypeDef*)evm_2_integer(v);
	if(port != GPIOA && port!=GPIOB && port != GPIOC) EVM_ARG_TYPE_ERR;
	
	uint16_t number = (uint16_t)evm_2_integer(v+1);
	if(number>15 ) EVM_ARG_TYPE_ERR;
	
	uint32_t mode = (uint32_t)evm_2_integer(v+2);
	if(mode != GPIO_MODE_IT_FALLING && mode != GPIO_MODE_IT_RISING && mode != GPIO_MODE_IT_RISING_FALLING) EVM_ARG_TYPE_ERR;
	
	uint32_t pull = (uint32_t)evm_2_integer(v+3);
	if(pull != GPIO_NOPULL && pull != GPIO_PULLUP && pull != GPIO_PULLDOWN) EVM_ARG_TYPE_ERR;
	
	IRQn_Type irqn = getIRQn((1<<number)&0xFFFF);
	if(port == GPIOA){__HAL_RCC_GPIOA_CLK_ENABLE();}
	else if(port == GPIOB){ __HAL_RCC_GPIOB_CLK_ENABLE();}
	else if(port == GPIOC){ __HAL_RCC_GPIOC_CLK_ENABLE();}
	
	if( evm_is_script(v+4) ) {
		if( pin_cb_id[number] == -1){
			pin_cb_id[number] = epolling_register(e, *(v+4));
		} else {
			pin_cb_id[number] = epolling_register_replace(e, pin_cb_id[number], *(v+4));
		}
	}
	
	GPIO_InitTypeDef GPIO_InitStructure; 
    GPIO_InitStructure.Pin = (1<<number)&0xFFFF;
    GPIO_InitStructure.Mode = mode;	    		
    GPIO_InitStructure.Pull = pull;
    HAL_GPIO_Init(port, &GPIO_InitStructure); 
    HAL_NVIC_SetPriority(irqn, 1, 0);
    /* 使能中断 */
    HAL_NVIC_EnableIRQ(irqn);
	
	evm_prop_set_value(e, p, ".irqn", irqn);

	return EVM_VAL_UNDEFINED;
}
//ExtInt.regs()
static evm_val_t evm_module_extint_regs(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}
//ExtInt.disable()
static evm_val_t evm_module_extint_disable(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	IRQn_Type irqn = (IRQn_Type)evm_2_integer(evm_prop_get(e, p, ".irqn", 0));
	HAL_NVIC_DisableIRQ(irqn);

	return EVM_VAL_UNDEFINED;
}
//ExtInt.enable()
static evm_val_t evm_module_extint_enable(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	IRQn_Type irqn = (IRQn_Type)evm_2_integer(evm_prop_get(e, p, ".irqn", 0));
	HAL_NVIC_EnableIRQ(irqn);
	
	return EVM_VAL_UNDEFINED;
}
//ExtInt.line()
static evm_val_t evm_module_extint_line(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return evm_2_integer(evm_prop_get(e, p, ".irqn", 0));
}
//ExtInt.swint()
static evm_val_t evm_module_extint_swint(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	IRQn_Type irqn = (IRQn_Type)evm_2_integer(evm_prop_get(e, p, ".irqn", 0));

	__HAL_GPIO_EXTI_GENERATE_SWIT(irqn);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_extint(evm_t * e){
	evm_builtin_t class_extint[] = {
		{"regs", evm_mk_native( (intptr_t)evm_module_extint_regs )},
		{"disable", evm_mk_native( (intptr_t)evm_module_extint_disable )},
		{"enable", evm_mk_native( (intptr_t)evm_module_extint_enable )},
		{"line", evm_mk_native( (intptr_t)evm_module_extint_line )},
		{"swint", evm_mk_native( (intptr_t)evm_module_extint_swint )},
		{"IRQ_FALLING",evm_mk_number((uint32_t)GPIO_MODE_IT_FALLING)},
		{"IRQ_RISING",evm_mk_number((uint32_t)GPIO_MODE_IT_RISING)},
		{"IRQ_RISING_FALLING",evm_mk_number((uint32_t)GPIO_MODE_IT_RISING_FALLING)},
		
		{".irqn",evm_mk_number(0)},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_extint, class_extint, NULL);
}
