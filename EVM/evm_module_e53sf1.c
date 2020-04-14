#include "evm_module.h"
#include "./E53_SF1/E53_SF1.h"

static evm_val_t evm_module_e53sf1(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	Init_E53_SF1();

	return EVM_VAL_UNDEFINED;
}

//e53sf1.smog()
static evm_val_t evm_module_e53sf1_smog(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	
	return evm_mk_number(E53_SF1_Read_Data());
	
	return EVM_VAL_UNDEFINED;
}

//e53sf1.lighton()
static evm_val_t evm_module_e53sf1_light_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_SF1_LED_StatusSet(ON);
	return EVM_VAL_UNDEFINED;
}


//e53sf1.lightoff()
static evm_val_t evm_module_e53sf1_light_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_SF1_LED_StatusSet(OFF);
	return EVM_VAL_UNDEFINED;
}

//e53sf1.beepon()
static evm_val_t evm_module_e53sf1_beep_on(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_SF1_Beep_StatusSet(ON);
	return EVM_VAL_UNDEFINED;
}


//e53sf1.beepoff()
static evm_val_t evm_module_e53sf1_beep_off(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=0) EVM_ARG_LENGTH_ERR;
	E53_SF1_Beep_StatusSet(OFF);
	return EVM_VAL_UNDEFINED;
}


evm_val_t evm_class_e53sf1(evm_t * e){
	evm_builtin_t class_e53sf1[] = {
		{"smog", evm_mk_native( (intptr_t)evm_module_e53sf1_smog )},
		{"lighton", evm_mk_native( (intptr_t)evm_module_e53sf1_light_on )},
		{"lightoff", evm_mk_native( (intptr_t)evm_module_e53sf1_light_off )},
		{"beepon", evm_mk_native( (intptr_t)evm_module_e53sf1_beep_on )},
		{"beepoff", evm_mk_native( (intptr_t)evm_module_e53sf1_beep_off )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_e53sf1, class_e53sf1, NULL);
}
