#include "evm_module.h"
#include "evm_main.h"
#include "epolling_module.h"

#define TIMER_ID		".id"
#define TIMER_PRESCALER	".scaler"
#define TIMER_PERIOD	".period"

TIM_HandleTypeDef    TimHandle;
int timer_cd_id = -1;

void TIM_Init(uint32_t arr,uint32_t psc)
{  
    TimHandle.Instance=TIM2;                          //通用定时器3
    TimHandle.Init.Prescaler=psc;                     //分频系数
    TimHandle.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TimHandle.Init.Period=arr;                        //自动装载值
    TimHandle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TimHandle);
    
    HAL_TIM_Base_Start_IT(&TimHandle); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle);
}



static evm_val_t evm_module_timer_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v);

//Timer(id, ...)
static evm_val_t evm_module_timer(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc==0 || argc>3) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v)) EVM_ARG_TYPE_ERR;
	uint8_t id = evm_2_integer(v);
	if(id >11) return EVM_VAL_UNDEFINED;

	evm_module_timer_init(e,p,argc-1,v+1);
	evm_prop_set_value(e, p, TIMER_ID, *v);
	
	return EVM_VAL_UNDEFINED;
}

//Timer.init(*, prescaler, period)
static evm_val_t evm_module_timer_init(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc==0 || argc>3) EVM_ARG_LENGTH_ERR;
	if(!evm_is_number(v)) EVM_ARG_TYPE_ERR;
	if(!evm_is_number(v+1)) EVM_ARG_TYPE_ERR;
	
	uint32_t prescaler = evm_2_integer(v);
	uint32_t period = evm_2_integer(v+1);

	TIM_Init(prescaler,period);
	
	evm_prop_set_value(e, p, TIMER_PRESCALER, *v);
	evm_prop_set_value(e, p, TIMER_PERIOD, *(v+1));
	
	return EVM_VAL_UNDEFINED;
}

//Timer.deinit()
static evm_val_t evm_module_timer_deinit(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	HAL_TIM_Base_DeInit(&TimHandle);
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	return EVM_VAL_UNDEFINED;
}

//Timer.callback(fun)
static evm_val_t evm_module_timer_callback(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc!=1) EVM_ARG_LENGTH_ERR;
	
	if( evm_is_script(v) ) {
		if( timer_cd_id == -1){
			timer_cd_id = epolling_register(e, *v);
		} else {
			timer_cd_id = epolling_register_replace(e, timer_cd_id, *v);
		}
	}
	else if(evm_is_null(v)) timer_cd_id = -1;
 	return EVM_VAL_UNDEFINED;
}

//Timer.channel(channel, mode, ...)
static evm_val_t evm_module_timer_channel(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timer.counter([value])
static evm_val_t evm_module_timer_counter(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timer.freq([value])
static evm_val_t evm_module_timer_freq(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timer.period([value])
static evm_val_t evm_module_timer_period(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if(argc>1) EVM_ARG_LENGTH_ERR;
	if(argc == 0) return *(evm_prop_get(e, p, TIMER_PERIOD, 0));
	else {
		
	}
 	return EVM_VAL_UNDEFINED;
}

//Timer.prescaler([value])
static evm_val_t evm_module_timer_prescaler(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//Timer.source_freq()
static evm_val_t evm_module_timer_source_freq(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}


evm_val_t evm_class_timer(evm_t * e){
	evm_builtin_t class_timer[] = {
		{"init", evm_mk_native( (intptr_t)evm_module_timer_init )},
		{"deinit", evm_mk_native( (intptr_t)evm_module_timer_deinit )},
		{"callback", evm_mk_native( (intptr_t)evm_module_timer_callback )},
//		{"channel", evm_mk_native( (intptr_t)evm_module_timer_channel )},
//		{"counter", evm_mk_native( (intptr_t)evm_module_timer_counter )},
//		{"freq", evm_mk_native( (intptr_t)evm_module_timer_freq )},
//		{"period", evm_mk_native( (intptr_t)evm_module_timer_period )},
//		{"prescaler", evm_mk_native( (intptr_t)evm_module_timer_prescaler )},
//		{"source_freq", evm_mk_native( (intptr_t)evm_module_timer_source_freq )},
		
//		{"UP",evm_mk_number(TIM_COUNTERMODE_UP)},
//		{"DOWN",evm_mk_number(TIM_COUNTERMODE_DOWN)},
//		{"CENTER",evm_mk_number(TIM_COUNTERMODE_CENTERALIGNED3)},
//		{"PWM",evm_mk_number(TIM_OCMODE_PWM1)},
//		{"PWM_INVERTED ",evm_mk_number(TIM_OCMODE_PWM2)},
//		{"OC_TIMING ",evm_mk_number(TIM_OCMODE_TIMING)},
//		{"OC_ACTIVE",evm_mk_number(TIM_OCMODE_ACTIVE)},
//		{"OC_INACTIVE ",evm_mk_number(TIM_OCMODE_INACTIVE)},
//		{"OC_TOGGLE ",evm_mk_number(TIM_OCMODE_TOGGLE)},
//		{"OC_FORCED_ACTIVE ",evm_mk_number(TIM_OCMODE_FORCED_ACTIVE)},
//		{"OC_FORCED_INACTIVE ",evm_mk_number(TIM_OCMODE_FORCED_INACTIVE)},
//		{"IC",evm_mk_number(0)},
//		{"ENC_A",evm_mk_number(0)},
//		{"ENC_B",evm_mk_number(0)},
//		{"ENC_AB",evm_mk_number(0)},
//		{"HIGH",evm_mk_number(TIM_OCPOLARITY_HIGH)},
//		{"LOW",evm_mk_number(TIM_OCPOLARITY_LOW)},
//		{"RISING",evm_mk_number(TIM_INPUTCHANNELPOLARITY_RISING)},
//		{"FALLING",evm_mk_number(TIM_INPUTCHANNELPOLARITY_FALLING)},
//		{"BOTH",evm_mk_number(TIM_INPUTCHANNELPOLARITY_BOTHEDGE)},
		
		{TIMER_ID,evm_mk_number(0)},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_timer, class_timer, NULL);
}
