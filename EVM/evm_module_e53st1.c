#include "evm_module.h"
#include "./E53_ST1/E53_ST1.h"

//E53ST1()
static evm_val_t evm_module_e53st1(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	Init_E53_ST1();

	return EVM_VAL_UNDEFINED;
}

//E53ST1.location()
static evm_val_t evm_module_e53st1_location(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	float longitude = 0.0;
	float latitude = 0.0;
	
	E53_ST1_Read_Data(&longitude,&latitude);
	
	evm_val_t *ebuff = evm_list_create(e,GC_LIST,2); 
	evm_list_set(e,ebuff,0,evm_mk_number(longitude));
	evm_list_set(e,ebuff,1,evm_mk_number(latitude));

	return *ebuff;
}

//E53ST1.lighton()
static evm_val_t evm_module_e53st1_light_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_ST1_LED_StatusSet(0);
	return EVM_VAL_UNDEFINED;
}


//E53ST1.lightoff()
static evm_val_t evm_module_e53st1_light_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_ST1_LED_StatusSet(1);
	return EVM_VAL_UNDEFINED;
}

//E53ST1.beepon()
static evm_val_t evm_module_e53st1_beep_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_ST1_Beep_StatusSet(0);
	return EVM_VAL_UNDEFINED;
}

//E53ST1.beepoff()
static evm_val_t evm_module_e53st1_beep_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_ST1_Beep_StatusSet(1);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_e53st1(evm_t * e){
	evm_builtin_t class_e53st1[] = {
		{"location", evm_mk_native( (intptr_t)evm_module_e53st1_location )},
		{"lighton", evm_mk_native( (intptr_t)evm_module_e53st1_light_on )},
		{"lightoff", evm_mk_native( (intptr_t)evm_module_e53st1_light_off )},
		{"beepon", evm_mk_native( (intptr_t)evm_module_e53st1_beep_on )},
		{"beepoff", evm_mk_native( (intptr_t)evm_module_e53st1_beep_off )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_e53st1, class_e53st1, NULL);
}
