#include "evm_module.h"

//Switch(id)
static evm_val_t evm_module_switch(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v)) EVM_ARG_TYPE_ERR;
	
	GPIO_InitTypeDef GPIO_InitStructure;
    /*开启按键GPIO口的时钟*/
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*选择按键的引脚*/	
    GPIO_InitStructure.Pin = GPIO_PIN_0; 
    /*设置引脚为输入模式*/
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 
    /*设置引脚不上拉也不下拉*/
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /*使用上面的结构体初始化按键*/
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*选择按键的引脚*/
	return EVM_VAL_UNDEFINED;
}

//Switch.__call__()
static evm_val_t evm_module_switch___call__(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	
	return EVM_VAL_UNDEFINED;
}

//Switch.value()
static evm_val_t evm_module_switch_value(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Switch.callback(fun)
static evm_val_t evm_module_switch_callback(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_switch(evm_t * e){
	evm_builtin_t class_switch[] = {
//		{"__call__", evm_mk_native( (intptr_t)evm_module_switch___call__ )},
		{"value", evm_mk_native( (intptr_t)evm_module_switch_value )},
		{"callback", evm_mk_native( (intptr_t)evm_module_switch_callback )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_class_switch, class_switch, NULL);
}

