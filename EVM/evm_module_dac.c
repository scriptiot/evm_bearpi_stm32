#include "evm_module.h"
#include "./dac/dac.h" 

//DAC()
static evm_val_t evm_module_dac(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;
	DAC1_Init();
	return EVM_VAL_UNDEFINED;
}

//DAC.deinit()
static evm_val_t evm_module_dac_deinit(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;
	DAC1_DeInit();
	return EVM_VAL_UNDEFINED;
}

//DAC.write(value)
static evm_val_t evm_module_dac_write(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
	int value = evm_2_integer(v);
	DAC1_Set_Value(value);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_dac(evm_t * e){
	evm_builtin_t class_dac[] = {
		{"deinit", evm_mk_native( (intptr_t)evm_module_dac_deinit )},
		{"write", evm_mk_native( (intptr_t)evm_module_dac_write )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_dac, class_dac, NULL);
}
