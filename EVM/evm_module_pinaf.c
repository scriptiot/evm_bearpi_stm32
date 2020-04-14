#include "evm_module.h"

//pinaf.__str__()
static evm_val_t evm_module_pin___str__(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//pinaf.index()
static evm_val_t evm_module_pin_index(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//pinaf.name()
static evm_val_t evm_module_pin_name(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//pinaf.reg()
static evm_val_t evm_module_pin_reg(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_pinaf(evm_t * e){
	evm_builtin_t class_pinaf[] = {
		{"__str__", evm_mk_native( (intptr_t)evm_module_pin___str__ )},
		{"off", evm_mk_native( (intptr_t)evm_module_pin_index )},
		{"name", evm_mk_native( (intptr_t)evm_module_pin_name )},
		{"reg", evm_mk_native( (intptr_t)evm_module_pin_reg )},
		{NULL, NULL}
	};
	return *evm_class_create(e, NULL, class_pinaf, NULL);
}

