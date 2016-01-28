#ifndef __HAPTIC_H
#define __HAPTIC_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_tim.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"


//-----------------¶Ë¿Ú¶¨Òå---------------- 
#define		HAPTIC_PIN				GPIO_Pin_8
#define		HAPTIC_PORT				GPIOB
#define		HAPTIC_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define		HAPTIC_PIN_SOURCE		GPIO_PinSource8
#define		HAPTIC_GPIO_AF			GPIO_AF_TIM10


#define		HAPTIC_TIMER			TIM10
#define		HAPTIC_TIMER_CLK		RCC_APB2Periph_TIM10


void HAPTIC_init(void);
void HAPTIC_on(uint8_t pwmPercent);
void HAPTIC_off(void);

#endif