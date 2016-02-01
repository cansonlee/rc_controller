#ifndef __KEYS_H
#define __KEYS_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_adc.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_dma.h"
#include "cmsis_os.h"

#include <stdint.h>



//-----------------按键开关端口定义---------------- 
//MENU按键接口
#define		KEY_MENU_PIN			GPIO_Pin_7
#define		KEY_MENU_PORT			GPIOD
#define		KEY_MENU_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		KEY_MENU_PIN_SOURCE		GPIO_PinSource7

//PAGE按键接口
#define		KEY_PAGE_PIN			GPIO_Pin_3
#define		KEY_PAGE_PORT			GPIOD
#define		KEY_PAGE_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		KEY_PAGE_PIN_SOURCE		GPIO_PinSource3

//EXIT按键接口
#define		KEY_EXIT_PIN			GPIO_Pin_2
#define		KEY_EXIT_PORT			GPIOD
#define		KEY_EXIT_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		KEY_EXIT_PIN_SOURCE		GPIO_PinSource2

//PLUS按键接口
#define		KEY_PLUS_PIN			GPIO_Pin_10
#define		KEY_PLUS_PORT			GPIOE
#define		KEY_PLUS_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		KEY_PLUS_PIN_SOURCE		GPIO_PinSource10

//MINUS按键接口
#define		KEY_MINUS_PIN			GPIO_Pin_11
#define		KEY_MINUS_PORT			GPIOE
#define		KEY_MINUS_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		KEY_MINUS_PIN_SOURCE	GPIO_PinSource11

//ENTER按键接口
#define		KEY_ENTER_PIN			GPIO_Pin_12
#define		KEY_ENTER_PORT			GPIOE
#define		KEY_ENTER_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		KEY_ENTER_PIN_SOURCE	GPIO_PinSource12


//-------------结构定义---------------------------

typedef enum key_val_e
{
	KEY_READY,
	KEY_MENU,
	KEY_PAGE,
	KEY_EXIT,
	KEY_PLUS,
	KEY_MINUS,
	KEY_ENTER
}KEY_VAL;

typedef enum key_status_e
{
	KEY_RELEASED,
	KEY_PRESSED,
	KEY_HOLD
}KEY_STATUS;

//-------------函数声明---------------------------
void keys_init(void);
KEY_VAL keys_read(KEY_STATUS *state);

#endif
