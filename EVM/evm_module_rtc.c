#include "evm_module.h"

static RTC_HandleTypeDef Rtc_Handle;
static uint32_t rtc_info = 0;

//STM32F1 RTC LSE 最低计时单位S
void PVD_Init(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	PWR_PVDTypeDef PWR_PVDStruct;
	PWR_PVDStruct.PVDLevel = PWR_PVDLEVEL_4;// 2.8V
	PWR_PVDStruct.Mode = PWR_PVD_MODE_IT_RISING;//降至阈值电压时触发

	HAL_PWR_ConfigPVD(&PWR_PVDStruct);
	HAL_PWR_EnablePVD();
	
	HAL_NVIC_SetPriority(PVD_PVM_IRQn,4,0);
    HAL_NVIC_EnableIRQ(PVD_PVM_IRQn);
}

/* 重写回调函数 */
void HAL_PWR_PVDCallback(void)
{
	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DR1,0x0001);
}

void PVD_IRQHandler(void)
{
	HAL_PWR_PVD_PVM_IRQHandler();
}


//RTC
static evm_val_t evm_module_rtc(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	// 配置RTC外设
	Rtc_Handle.Instance = RTC;

	/*使能 PWR 时钟*/
	__HAL_RCC_RTC_ENABLE();
	/* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
	HAL_PWR_EnableBkUpAccess();
	  HAL_PWR_EnableBkUpAccess();

	/* 初始化LSE */ 
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* 选择LSE做为RTC的时钟源 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	/* Configures the External Low Speed oscillator (LSE) drive capability */
	__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);  //
	
	/* 使能RTC时钟 */
	__HAL_RCC_RTC_ENABLE(); 

	/* 等待 RTC APB 寄存器同步 */
	HAL_RTC_WaitForSynchro(&Rtc_Handle);

	/*=====================初始化同步/异步预分频器的值======================*/
	/* 驱动日历的时钟ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */

	Rtc_Handle.Init.HourFormat = RTC_HOURFORMAT_24;

	/* 设置异步预分频器的值 */
	Rtc_Handle.Init.AsynchPrediv = 127;
	
	/* 设置同步预分频器的值 */
	Rtc_Handle.Init.SynchPrediv  = 255;	
	Rtc_Handle.Init.OutPut   = RTC_OUTPUT_DISABLE; 
	
	Rtc_Handle.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	Rtc_Handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	Rtc_Handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	/* 用RTC_InitStructure的内容初始化RTC寄存器 */
	uint32_t time_start = HAL_GetTick();
	HAL_RTC_Init(&Rtc_Handle);
	rtc_info = rtc_info | ((HAL_GetTick() - time_start)&0xFFFF);
	
	uint32_t resetflag = HAL_RTCEx_BKUPRead(&Rtc_Handle,RTC_BKP_DR1);
	if(resetflag == 0) resetflag = 2;

	rtc_info = rtc_info | (resetflag<<16);
	
	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DR1,0x0000);

	PVD_Init();
	return EVM_VAL_UNDEFINED;
}

//RTC.datetime([datetimetuple])
static evm_val_t evm_module_rtc_datetime(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	if( argc != 8 && argc != 0) EVM_ARG_LENGTH_ERR;
	if(argc == 0) 
	{
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		// 获取日历
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
	
		evm_val_t *ebuff = evm_list_create(e,GC_LIST,8); 
		uint8_t index = 0;
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_DateStructure.Year+2000));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_DateStructure.Month));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_DateStructure.Date));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_DateStructure.WeekDay));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_TimeStructure.Hours));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_TimeStructure.Minutes));
		evm_list_set(e,ebuff,index++,evm_mk_number(RTC_TimeStructure.Seconds));
		evm_list_set(e,ebuff,index++,evm_mk_number(0));

		return *ebuff;
	}
	else if(argc == 8){		
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		
		RTC_DateStructure.Year	  = evm_2_integer(v) - 2000;
		RTC_DateStructure.Month   = evm_2_integer(v+1);
		RTC_DateStructure.Date    = evm_2_integer(v+2);
		RTC_DateStructure.WeekDay = evm_2_integer(v+3);
		RTC_TimeStructure.Hours	  = evm_2_integer(v+4);
		RTC_TimeStructure.Minutes = evm_2_integer(v+5);
		RTC_TimeStructure.Seconds = evm_2_integer(v+6);
		//STM32f1 rtc不支持 subseconds、最后一位省去
		
		HAL_RTC_SetTime(&Rtc_Handle,&RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&Rtc_Handle,&RTC_DateStructure, RTC_FORMAT_BIN);
	}
	
	return EVM_VAL_UNDEFINED;
}

//RTC.wakeup(timeout, callback=None)		F1不支持毫秒定时
static evm_val_t evm_module_rtc_wakeup(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

//RTC.info()
static evm_val_t evm_module_rtc_info(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	
	return evm_mk_number(rtc_info);
}

//RTC.calibration(cal)
static evm_val_t evm_module_rtc_calibration(evm_t *e, evm_val_t *p, int argc, evm_val_t *v)
{
	return EVM_VAL_UNDEFINED;
}

evm_val_t evm_class_rtc(evm_t * e){
	evm_builtin_t class_rtc[] = {
		{"datetime", evm_mk_native( (intptr_t)evm_module_rtc_datetime )},
//		{"wakeup", evm_mk_native( (intptr_t)evm_module_rtc_wakeup )},
//		{"info", evm_mk_native( (intptr_t)evm_module_rtc_info )},
//		{"calibration", evm_mk_native( (intptr_t)evm_module_rtc_calibration )},
		{NULL, NULL}
	};
	return *evm_class_create(e, (evm_native_fn)evm_module_rtc, class_rtc, NULL);
}

