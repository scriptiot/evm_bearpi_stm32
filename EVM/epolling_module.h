#ifndef EPOLLING_MODULE_H
#define EPOLLING_MODULE_H

#include "evm.h"

void epolling_poll(evm_t *e);
int epolling_add(evm_t *e, evm_val_t callback, evm_val_t args);

evm_val_t epolling_register_get(evm_t *e, int index);
int epolling_register(evm_t *e, evm_val_t callback);
int epolling_register_replace(evm_t *e, int index, evm_val_t callback);
evm_val_t epolling_register_get(evm_t *e, int index);
int epolling_module(evm_t * e, uint32_t number_of_polls, uint32_t number_of_registers);

#endif

