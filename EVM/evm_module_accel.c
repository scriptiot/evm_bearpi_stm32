#include "evm_module.h"
#include "./mpu6050/mpu6050.h"

//Accel()
static evm_val_t evm_module_accel(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;
	MPU6050_Init();
	return EVM_VAL_UNDEFINED;
}
//Accel.filtered_xyz()()
static evm_val_t evm_module_accel_filtered_xyz(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;
	uint32_t acc_xyz[3] = {0};
	short acc_xyz_buff[3] = {0};

	for(int i=0;i<4;i++)
	{
		MPU6050ReadAcc(acc_xyz_buff);
		acc_xyz[0] += acc_xyz_buff[0];
		acc_xyz[1] += acc_xyz_buff[1];
		acc_xyz[2] += acc_xyz_buff[2];
	}
	
	evm_val_t *ebuff = evm_list_create(e,GC_LIST,8); 
	evm_list_set(e,ebuff,0,acc_xyz[0]);
	evm_list_set(e,ebuff,1,acc_xyz[1]);
	evm_list_set(e,ebuff,2,acc_xyz[2]);
	
	return *ebuff;
}
//Accel.tilt()
static evm_val_t evm_module_accel_tilt(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_UNDEFINED;
}
//Accel.x()
static evm_val_t evm_module_accel_x(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;

	short acc_xyz[3] = {0};
	MPU6050ReadAcc(acc_xyz);
	
	return evm_mk_number(acc_xyz[0]);
}
//Accel.y()
static evm_val_t evm_module_accel_y(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;

	short acc_xyz[3] = {0};
	MPU6050ReadAcc(acc_xyz);
	
	return evm_mk_number(acc_xyz[1]);
}
//Accel.z()
static evm_val_t evm_module_accel_z(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc != 0 ) EVM_ARG_LENGTH_ERR;

	short acc_xyz[3] = {0};
	MPU6050ReadAcc(acc_xyz);
	
	return evm_mk_number(acc_xyz[2]);
}

evm_val_t evm_class_accel(evm_t * e){
	evm_builtin_t class_accel[] = {
		{"filtered_xyz", evm_mk_native( (intptr_t)evm_module_accel_filtered_xyz )},
		{"tilt", evm_mk_native( (intptr_t)evm_module_accel_tilt )},
		{"x", evm_mk_native( (intptr_t)evm_module_accel_x )},
		{"y", evm_mk_native( (intptr_t)evm_module_accel_y )},
		{"z", evm_mk_native( (intptr_t)evm_module_accel_z )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_accel, class_accel, NULL);
}
