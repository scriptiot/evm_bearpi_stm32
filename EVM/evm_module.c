#include "evm_module.h"


//pyb.delay(ms)
static evm_val_t evm_module_delay(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v)) EVM_ARG_TYPE_ERR;

	HAL_Delay(evm_2_integer(v));
	return EVM_VAL_INFINITE;
}

//pyb.udelay(us)
static evm_val_t evm_module_udelay(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;
	if( !evm_is_number(v)) EVM_ARG_TYPE_ERR;

//	delayus(evm_2_integer(v));
	return EVM_VAL_INFINITE;
}

//pyb.millis()
static evm_val_t evm_module_millis(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.micros()
static evm_val_t evm_module_micros(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.elapsed_millis(start)
static evm_val_t evm_module_elapsed_millis(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.elapsed_micros(start)
static evm_val_t evm_module_elapsed_micros(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.hard_reset()
static evm_val_t evm_module_hard_reset(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
	
	return EVM_VAL_INFINITE;
}

//pyb.bootloader()
static evm_val_t evm_module_bootloader(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.fault_debug(value)
static evm_val_t evm_module_fault_debug(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.disable_irq()
static evm_val_t evm_module_disable_irq(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;
    __set_FAULTMASK(1);

	return EVM_VAL_INFINITE;
}

//pyb.enable_irq(state=True)
static evm_val_t evm_module_enable_irq(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;
    __set_FAULTMASK(0);

	return EVM_VAL_INFINITE;
}

//pyb.freq([sysclk[, hclk[, pclk1[, pclk2]]]])
static evm_val_t evm_module_freq(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc > 4 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.wfi()
static evm_val_t evm_module_wfi(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.stop()
static evm_val_t evm_module_stop(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.standby()
static evm_val_t evm_module_standby(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;
	

	return EVM_VAL_INFINITE;
}

//pyb.have_cdc()
static evm_val_t evm_module_have_cdc(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.hid((buttons, x, y, z))
static evm_val_t evm_module_hid(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.info([dump_alloc_table])
static evm_val_t evm_module_info(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.main(filename)
static evm_val_t evm_module_main(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.mount(device, mountpoint, *, readonly=False, mkfs=False)
static evm_val_t evm_module_mount(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc >4 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.repl_uart(uart)
static evm_val_t evm_module_repl_uart(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 1 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.rng()
static evm_val_t evm_module_rng(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.sync()
static evm_val_t evm_module_sync(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.unique_id()
static evm_val_t evm_module_unique_id(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 0 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

//pyb.usb_mode([modestr, ]port=-1, vid=0xf055, pid=-1, msc=(), hid=pyb.hid_mouse, high_speed=False)
static evm_val_t evm_module_usb_mode(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc >7 ) EVM_ARG_LENGTH_ERR;

	return EVM_VAL_INFINITE;
}

int evm_module(evm_t * e){
	evm_builtin_t module[] = {
		{"delay", evm_mk_native( (intptr_t)evm_module_delay )},
		{"udelay", evm_mk_native( (intptr_t)evm_module_udelay )},
		{"millis", evm_mk_native( (intptr_t)evm_module_millis )},
		{"micros", evm_mk_native( (intptr_t)evm_module_micros )},
		{"elapsed_millis", evm_mk_native( (intptr_t)evm_module_elapsed_millis )},
		{"elapsed_micros", evm_mk_native( (intptr_t)evm_module_elapsed_micros )},
		{"hard_reset", evm_mk_native( (intptr_t)evm_module_hard_reset )},
		{"bootloader", evm_mk_native( (intptr_t)evm_module_bootloader )},
		{"fault_debug", evm_mk_native( (intptr_t)evm_module_fault_debug )},
		{"disable_irq", evm_mk_native( (intptr_t)evm_module_disable_irq )},
		{"enable_irq", evm_mk_native( (intptr_t)evm_module_enable_irq )},
		{"freq", evm_mk_native( (intptr_t)evm_module_freq )},
		{"wfi", evm_mk_native( (intptr_t)evm_module_wfi )},
		{"stop", evm_mk_native( (intptr_t)evm_module_stop )},
		{"standby", evm_mk_native( (intptr_t)evm_module_standby )},
		{"have_cdc", evm_mk_native( (intptr_t)evm_module_have_cdc )},
		{"hid", evm_mk_native( (intptr_t)evm_module_hid )},
		{"info", evm_mk_native( (intptr_t)evm_module_info )},
		{"main", evm_mk_native( (intptr_t)evm_module_main )},
		{"mount", evm_mk_native( (intptr_t)evm_module_mount )},
		{"repl_uart", evm_mk_native( (intptr_t)evm_module_repl_uart )},
		{"rng", evm_mk_native( (intptr_t)evm_module_rng )},
		{"sync", evm_mk_native( (intptr_t)evm_module_sync )},
		{"unique_id", evm_mk_native( (intptr_t)evm_module_unique_id )},
		
        {"ADC", evm_class_adc(e)},
		{"DAC", evm_class_dac(e)},
		{"ExtInt", evm_class_extint(e)},
//		{"Flash", evm_class_flash(e)},
//		{"I2C", evm_class_i2c(e)},
		{"LCD", evm_class_lcd(e)},
		{"LED", evm_class_led(e)},
		{"Pin", evm_class_pin(e)},
		{"RTC", evm_class_rtc(e)},
//		{"SPI", evm_class_spi(e)},
//		{"Switch", evm_class_switch(e)},
		{"Timer", evm_class_timer(e)},
//		{"TimerChannel", evm_class_timerchannel(e)},
		{"UART", evm_class_uart(e)},
		{"E53SC1",evm_class_e53sc1(e)},
		{"E53SF1",evm_class_e53sf1(e)},
		{"E53ST1",evm_class_e53st1(e)},
		{"E53IA1",evm_class_e53ia1(e)},

        {NULL, NULL}
    };
    evm_module_create(e, "evm", module);
	return e->err;
}

