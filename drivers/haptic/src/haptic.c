#include "haptic.h"



/***************************************************************************************************
* @fn      HAPTIC_Registe
*
* @brief   振动马达资源注册
* @param   NULL
* @return  null
***************************************************************************************************/ 
void HAPTIC_Registe(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(HAPTIC_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HAPTIC_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(HAPTIC_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(HAPTIC_PORT, HAPTIC_PIN_SOURCE, HAPTIC_GPIO_AF);

	RCC_APB2PeriphClockCmd(HAPTIC_TIMER_CLK, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock / 10000 - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100;	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(HAPTIC_TIMER, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(HAPTIC_TIMER, &TIM_OCInitStructure);

	TIM_Cmd(HAPTIC_TIMER, ENABLE);
}


/***************************************************************************************************
* @fn      HAPTIC_init
*
* @brief   振动马达初始化
* @param   NULL
* @return  null
***************************************************************************************************/ 
void HAPTIC_init(void)
{
	HAPTIC_Registe();
}


/***************************************************************************************************
* @fn      HAPTIC_on
*
* @brief   振动马达开
* @param   pwmPercent -- 振动强度
* @return  null
***************************************************************************************************/ 
void HAPTIC_on(uint8_t pwmPercent)
{
	if (pwmPercent > 100) 
	{
    	pwmPercent = 100;
  	}
	TIM_SetCompare1(HAPTIC_TIMER, pwmPercent);
}

/***************************************************************************************************
* @fn      HAPTIC_off
*
* @brief   马达开度
* @param   NULL
* @return  null
***************************************************************************************************/ 
void HAPTIC_off(void)
{
	TIM_SetCompare1(HAPTIC_TIMER, 0);
}


//////////////end of file/////////////////////////////////////////////////////
