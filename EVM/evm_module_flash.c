#include "evm_module.h"
#include "./flash/bsp_spi_flash.h"

#define BUFFER_SIZE	512

//Flash()
static evm_val_t evm_module_flash(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	SPI_FLASH_Init();
	return EVM_VAL_UNDEFINED;
}
//Flash.read(address, buf, size)
static evm_val_t evm_module_flash_readblocks(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 3 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_buffer(v + 1) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
	int address = evm_2_integer(v);
	uint8_t * buf = evm_buffer_addr(v + 1);
	int size = evm_2_integer(v + 2);
	int buf_len = evm_buffer_len(v + 1);
	size = size < buf_len ? size: buf_len;
	
	SPI_FLASH_BufferRead(buf, address, size);
	return EVM_VAL_UNDEFINED;
}
//Flash.write(address, buf, size)
static evm_val_t evm_module_flash_writeblocks(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 3 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_buffer(v + 1) ) EVM_ARG_TYPE_ERR;
	if( !evm_is_number(v + 2) ) EVM_ARG_TYPE_ERR;
	int address = evm_2_integer(v);
	uint8_t * buf = evm_buffer_addr(v + 1);
	int size = evm_2_integer(v + 2);
	int buf_len = evm_buffer_len(v + 1);
	
	size = size < buf_len ? size: buf_len;
	SPI_FLASH_SectorErase(address);	 	 
	SPI_FLASH_BufferWrite(buf, address, size);
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_flash(evm_t * e){
	evm_builtin_t class_flash[] = {
		{"readblocks", evm_mk_native( (intptr_t)evm_module_flash_readblocks )},
		{"writeblocks", evm_mk_native( (intptr_t)evm_module_flash_writeblocks )},
		{NULL, NULL}
	};
	return  *evm_class_create(e, (evm_native_fn)evm_module_flash, class_flash, NULL);
}
