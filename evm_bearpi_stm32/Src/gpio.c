/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

}

void MX_KEY_INIT(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();              

    GPIO_Initure.Pin=GPIO_PIN_2;     
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_3;     
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //中断线2-PE2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
    
    //中断线3-PE3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,0);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
    
}


////中断服务函数
//void EXTI2_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);		//调用中断处理公用函数
//}

//void EXTI3_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);		//调用中断处理公用函数
//}

////中断服务程序中需要做的事情
////在HAL库中所有的外部中断服务函数都会调用此函数
////GPIO_Pin:中断引脚号
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == GPIO_PIN_2)
//	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//	}else{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//	}
//}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
