#include "evm_module.h"

//Servo(id)
static evm_val_t evm_module_servo(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Servo.angle([angle, time=0])
static evm_val_t evm_module_servo_angle(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Servo.speed([speed, time=0])
static evm_val_t evm_module_servo_speed(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Servo.pulse_width([value])
static evm_val_t evm_module_servo_pulse_width(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Servo.calibration([pulse_min, pulse_max, pulse_centre[, pulse_angle_90, pulse_speed_100]])
static evm_val_t evm_module_servo_calibration(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_servo(evm_t * e){
	evm_builtin_t class_servo[] = {
		{"angle", evm_mk_native( (intptr_t)evm_module_servo_angle )},
		{"speed", evm_mk_native( (intptr_t)evm_module_servo_speed )},
		{"pulse_width", evm_mk_native( (intptr_t)evm_module_servo_pulse_width )},
		{"calibration", evm_mk_native( (intptr_t)evm_module_servo_calibration )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_servo, class_servo, NULL);
}

