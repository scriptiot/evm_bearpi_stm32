#include "evm_module.h"
#include "./E53_IA1/E53_IA1.h"

//E53IA1()
static evm_val_t evm_module_e53ia1(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	Init_E53_IA1();

	return EVM_VAL_UNDEFINED;
}

//E53IA1.info()
static evm_val_t evm_module_e53ia1_location(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	float lux = 0.0;
	float temper = 0.0;
	float humidity = 0.0;
	
	E53_IA1_Read_Data(&lux,&temper,&humidity);
	
	evm_val_t *ebuff = evm_list_create(e,GC_LIST,3); 
	evm_list_set(e,ebuff,0,evm_mk_number(lux));
	evm_list_set(e,ebuff,1,evm_mk_number(temper));
	evm_list_set(e,ebuff,2,evm_mk_number(humidity));

	return *ebuff;
}

//E53IA1.lighton()
static evm_val_t evm_module_e53ia1_light_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_IA1_LED_StatusSet(0);
	return EVM_VAL_UNDEFINED;
}


//E53IA1.lightoff()
static evm_val_t evm_module_e53ia1_light_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_IA1_LED_StatusSet(1);
	return EVM_VAL_UNDEFINED;
}

//E53IA1.motoron()
static evm_val_t evm_module_e53ia1_motor_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_IA1_MOTOR_StatusSet(0);
	return EVM_VAL_UNDEFINED;
}

//E53IA1.motoroff()
static evm_val_t evm_module_e53ia1_motor_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_IA1_MOTOR_StatusSet(1);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_e53ia1(evm_t * e){
	evm_builtin_t class_e53ia1[] = {
		{"info", evm_mk_native( (intptr_t)evm_module_e53ia1_location )},
		{"lighton", evm_mk_native( (intptr_t)evm_module_e53ia1_light_on )},
		{"lightoff", evm_mk_native( (intptr_t)evm_module_e53ia1_light_off )},
		{"motoron", evm_mk_native( (intptr_t)evm_module_e53ia1_motor_on )},
		{"motoroff", evm_mk_native( (intptr_t)evm_module_e53ia1_motor_off )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_e53ia1, class_e53ia1, NULL);
}
