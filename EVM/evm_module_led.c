#include "evm_module.h"
#include "./led/evm_led.h"

#define LED_ID_NAME			".id"


//LED(id)
static evm_val_t evm_module_led(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1  ) EVM_ARG_LENGTH_ERR;
	uint8_t id = 0;
	
	if(argc == 1) {
		if(!evm_is_number(v)) EVM_ARG_TYPE_ERR;
		id = evm_2_integer(v);
	}
	
	led_init(id);
	
	evm_prop_set_value(e, p, LED_ID_NAME, *v);

	return EVM_VAL_UNDEFINED;
}

//off()
static evm_val_t evm_module_led_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	uint8_t id = evm_2_integer(evm_prop_get(e, p, LED_ID_NAME, 0));

	led_off(id);
	return EVM_VAL_UNDEFINED;
}

//on()
static evm_val_t evm_module_led_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	uint8_t id = evm_2_integer(evm_prop_get(e, p, LED_ID_NAME, 0));
	led_on(id);
	return EVM_VAL_UNDEFINED;
}

//toggle()
static evm_val_t evm_module_led_toggle(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	uint8_t id = evm_2_integer(evm_prop_get(e, p, LED_ID_NAME, 0));
	led_toggle(id);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_led(evm_t * e){
	evm_builtin_t class_led[] = {
		{"off", evm_mk_native( (intptr_t)evm_module_led_off )},
		{"on", evm_mk_native( (intptr_t)evm_module_led_on )},
		{"toggle", evm_mk_native( (intptr_t)evm_module_led_toggle )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_led, class_led, NULL);
}
