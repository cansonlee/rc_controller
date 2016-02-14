#ifndef __POWER_H
#define __POWER_H

#include "Stm32f2xx.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_gpio.h"

//-----------------端口定义---------------- 
//自动关机接口
#define		PWR_MCU_PIN				GPIO_Pin_0
#define		PWR_MCU_PORT			GPIOD
#define		PWR_MCU_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		PWR_MCU_PIN_SOURCE		GPIO_PinSource0

//内部RF模块接口
#define		PWR_INT_RF_PIN			GPIO_Pin_6
#define		PWR_INT_RF_PORT			GPIOC
#define		PWR_INT_RF_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define		PWR_INT_RF_PIN_SOURCE	GPIO_PinSource6

//外部RF模块接口
#define		PWR_EXT_RF_PIN			GPIO_Pin_8
#define		PWR_EXT_RF_PORT			GPIOD
#define		PWR_EXT_RF_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		PWR_EXT_RF_PIN_SOURCE	GPIO_PinSource8


typedef enum pwr_module_e
{
    PWR_MODULE_MAIN,
    PWR_MODULE_INT_RF,
    PWR_MODULE_EXT_RF
}PWR_MODULE_ENUM;

typedef enum pwr_action_e
{
    PWR_OFF,
    PWR_ON
}PWR_ACTION_ENUM;


void pwr_init(void);
void pwr_on_off(PWR_MODULE_ENUM mod, PWR_ACTION_ENUM act);

#endif