#include "evm_module.h"


//Timerchannel.callback(fun)
static evm_val_t evm_module_timerchannel_callback(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timerchannel.capture([value])
static evm_val_t evm_module_timerchannel_capture(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timerchannel.compare([value])
static evm_val_t evm_module_timerchannel_compare(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timerchannel.pulse_width([value])
static evm_val_t evm_module_timerchannel_pulse_width(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timerchannel.pulse_width_percent([value])
static evm_val_t evm_module_timerchannel_pulse_width_percent(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}


evm_val_t evm_class_timerchannel(evm_t * e){
	evm_builtin_t class_timerchannel[] = {
		{"callback", evm_mk_native( (intptr_t)evm_module_timerchannel_callback )},
		{"capture", evm_mk_native( (intptr_t)evm_module_timerchannel_capture )},
		{"compare", evm_mk_native( (intptr_t)evm_module_timerchannel_compare )},
		{"pulse_width", evm_mk_native( (intptr_t)evm_module_timerchannel_pulse_width )},
		{"pulse_width_percent", evm_mk_native( (intptr_t)evm_module_timerchannel_pulse_width_percent )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)NULL, class_timerchannel, NULL);
}
