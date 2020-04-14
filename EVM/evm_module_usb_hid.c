#include "evm_module.h"

//USB_HID
static evm_val_t evm_module_usbhid(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_HID.recv(data, *, timeout=5000)
static evm_val_t evm_module_usbhid_recv(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_HID.send(data)
static evm_val_t evm_module_usbhid_send(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_usbhid(evm_t * e){
	evm_builtin_t class_usbhid[] = {
		{"recv", evm_mk_native( (intptr_t)evm_module_usbhid_recv )},
		{"send", evm_mk_native( (intptr_t)evm_module_usbhid_send )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_usbhid, class_usbhid, NULL);
}

