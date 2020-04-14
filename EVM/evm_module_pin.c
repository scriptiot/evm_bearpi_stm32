#include "evm_module.h"

#define PIN_PORT	".port"
#define PIN_NUMBER	".number"
#define PIN_MODE	".mode"
#define PIN_PULL	".pull"



static uint16_t hw_pin_2_gpio_pin(uint16_t pin)
{
	int val = 1;
	return (val << pin);
}

static evm_val_t hw_gpio_init(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, uint32_t mode, uint32_t pull)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(GPIOx == GPIOA){ __HAL_RCC_GPIOA_CLK_ENABLE();}
	else if(GPIOx == GPIOB){ __HAL_RCC_GPIOB_CLK_ENABLE();}
	else if(GPIOx == GPIOC){ __HAL_RCC_GPIOC_CLK_ENABLE();}
	else return EVM_VAL_UNDEFINED;

	GPIO_InitStructure.Pin  = GPIO_Pin;
	GPIO_InitStructure.Mode = mode;
	GPIO_InitStructure.Pull = pull;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
 	HAL_GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	return EVM_VAL_INFINITE;
}

//Pin(port, number, mode, pull)
static evm_val_t evm_module_pin(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 4 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 1) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 3) ) EVM_ARG_TYPE_ERR;

	
	GPIO_TypeDef* port = (GPIO_TypeDef*)evm_2_integer(v);
	uint32_t pin = evm_2_integer(v + 1);
	uint32_t mode = (uint32_t)evm_2_integer(v + 2);
	uint32_t pull = (uint32_t)evm_2_integer(v + 3);

	
	if(hw_gpio_init(port, hw_pin_2_gpio_pin(pin), mode, pull) == EVM_VAL_UNDEFINED) EVM_ARG_TYPE_ERR;
	
	evm_prop_set_value(e, p, PIN_PORT, *v);
	evm_prop_set_value(e, p, PIN_NUMBER, *(v + 1));
	evm_prop_set_value(e, p, PIN_MODE, *(v + 2));
	evm_prop_set_value(e, p, PIN_PULL, *(v + 3));
	
	return EVM_VAL_UNDEFINED;
}
//Pin.value(value)
static evm_val_t evm_module_pin_value(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc == 0 ){
		GPIO_TypeDef* port = (GPIO_TypeDef*)evm_2_integer(evm_prop_get(e, p, PIN_PORT, 0));
		uint32_t pin = evm_2_integer(evm_prop_get(e, p, PIN_NUMBER, 0));
		return evm_mk_number(HAL_GPIO_ReadPin(port, hw_pin_2_gpio_pin(pin)));
	} else if( argc == 1 ) {
		GPIO_TypeDef* port = (GPIO_TypeDef*)evm_2_integer(evm_prop_get(e, p, PIN_PORT, 0));
		uint32_t pin = evm_2_integer(evm_prop_get(e, p, PIN_NUMBER, 0));
		int value = evm_2_integer(v);
		if( value == 0 ) HAL_GPIO_WritePin(port, hw_pin_2_gpio_pin(pin),GPIO_PIN_RESET);
		else HAL_GPIO_WritePin(port, hw_pin_2_gpio_pin(pin),GPIO_PIN_SET);
	}
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_pin(evm_t * e){
	evm_builtin_t cls[] = {
		{"value", evm_mk_native( (intptr_t)evm_module_pin_value )},
		{PIN_PORT, evm_mk_number(0) },
		{PIN_MODE, evm_mk_number(0) },
		{PIN_NUMBER, evm_mk_number(0) },
		{PIN_PULL, evm_mk_number(0) },
		
		{"AF_OD", evm_mk_number(GPIO_MODE_AF_OD) },
		{"AF_PP", evm_mk_number(GPIO_MODE_AF_PP) },
		{"ANALOG", evm_mk_number(GPIO_MODE_ANALOG) },
		{"IN", evm_mk_number(GPIO_MODE_INPUT) },
		{"OUT_OD", evm_mk_number(GPIO_MODE_OUTPUT_OD) },
		{"OUT_PP", evm_mk_number(GPIO_MODE_OUTPUT_PP) },
		{"PULL_DOWN", evm_mk_number(GPIO_PULLDOWN) },
		{"PULL_NONE", evm_mk_number(GPIO_NOPULL) },
		{"PULL_UP", evm_mk_number(GPIO_PULLUP) },
		{"GPIOA", evm_mk_number((uint32_t)GPIOA) },
		{"GPIOB", evm_mk_number((uint32_t)GPIOB) },
		{"GPIOC", evm_mk_number((uint32_t)GPIOC) },
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_pin, cls, NULL);
}
