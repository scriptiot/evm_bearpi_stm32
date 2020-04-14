#include "evm_module.h"
#include "./adc/adc.h"

//ADC()
static evm_val_t evm_module_adc(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	MX_ADC1_Init();
	return EVM_VAL_UNDEFINED;
}
//ADC.read()
static evm_val_t evm_module_adc_read(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return evm_mk_number(getAdcValue());
}


evm_val_t evm_class_adc(evm_t * e){
	evm_builtin_t class_adc[] = {
		{"read", evm_mk_native( (intptr_t)evm_module_adc_read )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_adc, class_adc, NULL);
}
