#include "epolling_module.h"

uint32_t epolling_number_of_registers;

uint32_t epolling_number_of_polls;
uint32_t epolling_count;

evm_val_t * epolling_register_list;
evm_val_t * epolling_callback_list;
evm_val_t * epolling_args_list;

int epolling_add(evm_t *e, evm_val_t callback, evm_val_t args){
	if( !evm_is_script(&callback) ) return ec_type;
	if( !evm_is_list(&args) && !evm_is_null(&args) && !evm_is_undefined(&args) ) return ec_type;
	if( epolling_count >= epolling_number_of_polls ) return ec_index;
	
	evm_list_set(e, epolling_callback_list, epolling_count, callback);
	evm_list_set(e, epolling_args_list, epolling_count, args);
	epolling_count++;
	return ec_ok;
}

evm_val_t epolling_register_get(evm_t *e, int index){
	if( index < 0 ) return EVM_VAL_UNDEFINED;
	if( index >= epolling_number_of_registers ) return EVM_VAL_UNDEFINED;
	
	return *evm_list_get(e, epolling_register_list, index);
}

int epolling_register(evm_t *e, evm_val_t callback){
	if( !evm_is_script(&callback) ) return -1;
	
	for(uint32_t i = 0; i < epolling_number_of_registers; i++){
		evm_val_t * v = evm_list_get(e, epolling_register_list, i);
		if( evm_is_undefined(v) ){
			evm_list_set(e, epolling_register_list, i, callback);
			return i;
		}
	}
	return -1;
}

int epolling_register_replace(evm_t *e, int index, evm_val_t callback){
	if( !evm_is_script(&callback) ) return -1;
	if( index >= epolling_number_of_registers ) return -1;
	
	evm_val_t * v = evm_list_get(e, epolling_register_list, index);
	evm_list_set(e, epolling_register_list, index, callback);
	return index;
}

void epolling_poll(evm_t *e){
	if( epolling_count == 0) return;
	while( epolling_count-- > 0 ){
		evm_val_t * args = evm_list_get(e, epolling_args_list, epolling_count);
		evm_val_t * cb = evm_list_get(e, epolling_callback_list, epolling_count);
		if( evm_is_undefined(args) || evm_is_null(args) ){
			evm_run_callback(e, cb, &e->scope, NULL, 0);
		} else {
			uint32_t args_len = evm_list_len(args);
			if( args_len == 0 ) {
				evm_run_callback(e, cb, &e->scope, NULL, 0);
			} else {
				evm_val_t * sp = e->sp;
				for(uint32_t i = 0; i < args_len; i++){
					*(++e->sp) = *evm_list_get(e, args, i);
				}
				evm_run_callback(e, cb, &e->scope, sp + 1, args_len);
			}
		} 
	}
	epolling_count = 0;
}

static evm_val_t epolling_native_add(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
	if( argc >= 2 ){
		if( epolling_add(e, *v, *(v + 1)) == ec_ok ) return EVM_VAL_TRUE;
	}
	return EVM_VAL_FALSE;
}

static evm_val_t epolling_native_register(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
	if( argc >= 1 ){
		return evm_mk_number( epolling_register(e, *v) );
	}
	return EVM_VAL_UNDEFINED;
}

static evm_val_t epolling_native_register_replace(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
	if( argc >= 2 ){
		return evm_mk_number( epolling_register_replace(e, evm_2_integer(v) ,*(v + 1) ) );
	}
	return EVM_VAL_UNDEFINED;
}

static evm_val_t epolling_native_register_get(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
	if( argc >= 1 ){
		return epolling_register_get(e, evm_2_integer(v) );
	}
	return EVM_VAL_UNDEFINED;
}

int epolling_module(evm_t * e, uint32_t number_of_polls, uint32_t number_of_registers){
	epolling_count = 0;
	epolling_number_of_polls = number_of_polls;
	epolling_number_of_registers = number_of_registers;
	
	epolling_callback_list = evm_list_create(e, GC_LIST, number_of_polls);
	if( !epolling_callback_list ) return e->err;
	
	epolling_args_list = evm_list_create(e, GC_LIST, number_of_polls);
	if( !epolling_args_list ) return e->err;
	
	epolling_register_list = evm_list_create(e, GC_LIST, number_of_registers);
	if( !epolling_register_list ) return e->err;
	
	for(uint32_t i = 0; i < epolling_number_of_registers; i++){
		evm_list_set(e, epolling_register_list, i, EVM_VAL_UNDEFINED);
	}
	
	evm_builtin_t natives[] = {
        {"epolling_add", evm_mk_native( (intptr_t)epolling_native_add )},
		{"epolling_register", evm_mk_native( (intptr_t)epolling_native_register )},
		{"epolling_register_replace", evm_mk_native( (intptr_t)epolling_native_register_replace )},
		{"epolling_register_get", evm_mk_native( (intptr_t)epolling_native_register_get )},
        {NULL, EVM_VAL_UNDEFINED}
    };
    return evm_native_add(e, natives);
}
