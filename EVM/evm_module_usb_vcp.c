#include "evm_module.h"

//USB_VCP(id=0)
static evm_val_t evm_module_usbvcp(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.init(*, flow=-1)
static evm_val_t evm_module_usbvcp_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	
	
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.setinterrupt(chr)
static evm_val_t evm_module_usbvcp_setinterrupt(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.isconnected()
static evm_val_t evm_module_usbvcp_isconnected(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.any()
static evm_val_t evm_module_usbvcp_any(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.close()
static evm_val_t evm_module_usbvcp_close(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.read([nbytes])
static evm_val_t evm_module_usbvcp_read(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.readinto(buf[, maxlen])
static evm_val_t evm_module_usbvcp_readinto(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.readline()
static evm_val_t evm_module_usbvcp_readline(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.readlines()
static evm_val_t evm_module_usbvcp_readlines(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.write(buf)
static evm_val_t evm_module_usbvcp_write(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.recv(data, *, timeout=5000)
static evm_val_t evm_module_usbvcp_recv(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//USB_VCP.send(data, *, timeout=5000)
static evm_val_t evm_module_usbvcp_send(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}


evm_val_t evm_class_usbvcp(evm_t * e){
	evm_builtin_t class_usbvcp[] = {
		{"init", evm_mk_native( (intptr_t)evm_module_usbvcp_init )},
		{"setinterrupt", evm_mk_native( (intptr_t)evm_module_usbvcp_setinterrupt )},
		{"isconnected", evm_mk_native( (intptr_t)evm_module_usbvcp_isconnected )},
		{"any", evm_mk_native( (intptr_t)evm_module_usbvcp_close )},
		{"close", evm_mk_native( (intptr_t)evm_module_usbvcp_init )},
		{"read", evm_mk_native( (intptr_t)evm_module_usbvcp_read )},
		{"readinto", evm_mk_native( (intptr_t)evm_module_usbvcp_readinto )},
		{"readline", evm_mk_native( (intptr_t)evm_module_usbvcp_readline )},
		{"readlines", evm_mk_native( (intptr_t)evm_module_usbvcp_readlines )},
		{"write", evm_mk_native( (intptr_t)evm_module_usbvcp_write )},
		{"recv", evm_mk_native( (intptr_t)evm_module_usbvcp_recv )},
		{"send", evm_mk_native( (intptr_t)evm_module_usbvcp_send )},
		{"RTS",evm_mk_number((uint32_t)0)},
		{"CTS",evm_mk_number((uint32_t)1)},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_usbvcp, class_usbvcp, NULL);
}


