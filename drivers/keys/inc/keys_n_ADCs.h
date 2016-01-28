#ifndef __KEYS_N_ADCS_H
#define __KEYS_N_ADCS_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_adc.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_dma.h"
#include "cmsis_os.h"

#include <stdint.h>

//-----------------拨动开关端口定义---------------- 
//SA拨钮接口
#define		SLD_SA_H_PIN			GPIO_Pin_5
#define		SLD_SA_H_PORT			GPIOB
#define		SLD_SA_H_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SLD_SA_H_PIN_SOURCE		GPIO_PinSource5

#define		SLD_SA_L_PIN			GPIO_Pin_0
#define		SLD_SA_L_PORT			GPIOE
#define		SLD_SA_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SA_L_PIN_SOURCE		GPIO_PinSource0

//SB拨钮接口
#define		SLD_SB_H_PIN			GPIO_Pin_2
#define		SLD_SB_H_PORT			GPIOE
#define		SLD_SB_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SB_H_PIN_SOURCE		GPIO_PinSource2

#define		SLD_SB_L_PIN			GPIO_Pin_1
#define		SLD_SB_L_PORT			GPIOE
#define		SLD_SB_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SB_L_PIN_SOURCE		GPIO_PinSource1

//SC拨钮接口
#define		SLD_SC_H_PIN			GPIO_Pin_15
#define		SLD_SC_H_PORT			GPIOE
#define		SLD_SC_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SC_H_PIN_SOURCE		GPIO_PinSource15

#define		SLD_SC_L_PIN			GPIO_Pin_5
#define		SLD_SC_L_PORT			GPIOA
#define		SLD_SC_L_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		SLD_SC_L_PIN_SOURCE		GPIO_PinSource5

//SD拨钮接口
#define		SLD_SD_H_PIN			GPIO_Pin_7
#define		SLD_SD_H_PORT			GPIOE
#define		SLD_SD_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SD_H_PIN_SOURCE		GPIO_PinSource7

#define		SLD_SD_L_PIN			GPIO_Pin_13
#define		SLD_SD_L_PORT			GPIOE
#define		SLD_SD_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SLD_SD_L_PIN_SOURCE		GPIO_PinSource13

//SE拨钮接口
#define		SLD_SE_H_PIN			GPIO_Pin_4
#define		SLD_SE_H_PORT			GPIOB
#define		SLD_SE_H_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SLD_SE_H_PIN_SOURCE		GPIO_PinSource4

#define		SLD_SE_L_PIN			GPIO_Pin_3
#define		SLD_SE_L_PORT			GPIOB
#define		SLD_SE_L_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SLD_SE_L_PIN_SOURCE		GPIO_PinSource3

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

//-----------------舵、拨轮端口定义---------------- 
//右舵上下
#define		STICK_RV_PIN			GPIO_Pin_0
#define		STICK_RV_PORT			GPIOA
#define		STICK_RV_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		STICK_RV_PIN_SOURCE		GPIO_PinSource0
#define		STICK_RV_ADC			ADC1
#define		STICK_RV_ADC_CLK		RCC_APB2Periph_ADC1
#define		STICK_RV_ADC_CHANNEL	ADC_Channel_0

//右舵左右
#define		STICK_RH_PIN			GPIO_Pin_1
#define		STICK_RH_PORT			GPIOA
#define		STICK_RH_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		STICK_RH_PIN_SOURCE		GPIO_PinSource1
#define		STICK_RH_ADC			ADC1
#define		STICK_RH_ADC_CLK		RCC_APB2Periph_ADC1
#define		STICK_RH_ADC_CHANNEL	ADC_Channel_1

//左舵左右
#define		STICK_LH_PIN			GPIO_Pin_2
#define		STICK_LH_PORT			GPIOA
#define		STICK_LH_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		STICK_LH_PIN_SOURCE		GPIO_PinSource2
#define		STICK_LH_ADC			ADC1
#define		STICK_LH_ADC_CLK		RCC_APB2Periph_ADC1
#define		STICK_LH_ADC_CHANNEL	ADC_Channel_2

//左舵上下
#define		STICK_LV_PIN			GPIO_Pin_3
#define		STICK_LV_PORT			GPIOA
#define		STICK_LV_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		STICK_LV_PIN_SOURCE		GPIO_PinSource3
#define		STICK_LV_ADC			ADC1
#define		STICK_LV_ADC_CLK		RCC_APB2Periph_ADC1
#define		STICK_LV_ADC_CHANNEL	ADC_Channel_3

//左上拨轮
#define		ROTATE_LU_PIN			GPIO_Pin_6
#define		ROTATE_LU_PORT			GPIOA
#define		ROTATE_LU_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		ROTATE_LU_PIN_SOURCE	GPIO_PinSource6
#define		ROTATE_LU_ADC			ADC1
#define		ROTATE_LU_ADC_CLK		RCC_APB2Periph_ADC1
#define		ROTATE_LU_ADC_CHANNEL	ADC_Channel_6

//左下拨轮
#define		ROTATE_LD_PIN			GPIO_Pin_4
#define		ROTATE_LD_PORT			GPIOC
#define		ROTATE_LD_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define		ROTATE_LD_PIN_SOURCE	GPIO_PinSource4
#define		ROTATE_LD_ADC			ADC1
#define		ROTATE_LD_ADC_CLK		RCC_APB2Periph_ADC1
#define		ROTATE_LD_ADC_CHANNEL	ADC_Channel_14

//右上拨轮
#define		ROTATE_RU_PIN			GPIO_Pin_0
#define		ROTATE_RU_PORT			GPIOB
#define		ROTATE_RU_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		ROTATE_RU_PIN_SOURCE	GPIO_PinSource0
#define		ROTATE_RU_ADC			ADC1
#define		ROTATE_RU_ADC_CLK		RCC_APB2Periph_ADC1
#define		ROTATE_RU_ADC_CHANNEL	ADC_Channel_8

//右下拨轮
#define		ROTATE_RD_PIN			GPIO_Pin_5
#define		ROTATE_RD_PORT			GPIOC
#define		ROTATE_RD_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define		ROTATE_RD_PIN_SOURCE	GPIO_PinSource5
#define		ROTATE_RD_ADC			ADC1
#define		ROTATE_RD_ADC_CLK		RCC_APB2Periph_ADC1
#define		ROTATE_RD_ADC_CHANNEL	ADC_Channel_15

//-----------------电压监测端口定义----------------
#define		MONITOR_VOL_PIN			GPIO_Pin_0
#define		MONITOR_VOL_PORT		GPIOC
#define		MONITOR_VOL_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define		MONITOR_VOL_PIN_SOURCE	GPIO_PinSource0
#define		MONITOR_VOL_ADC			ADC1
#define		MONITOR_VOL_ADC_CLK		RCC_APB2Periph_ADC1
#define		MONITOR_VOL_ADC_CHANNEL	ADC_Channel_10

#define		SKYBORNE_ADC				ADC1
#define		SKYBORNE_ADC_CLK			RCC_APB2Periph_ADC1
#define		SKYBORNE_ADC_DMA_STREAM		DMA2_Stream0
#define		SKYBORNE_ADC_DMA_CHANNEL 	DMA_Channel_0
#define		SKYBORNE_ADC_DMA_IRQHandler	DMA2_Stream0_IRQHandler
#define		SKYBORNE_ADC_DMA_TC_FLAG	DMA_IT_TCIF0
#define		SKYBORNE_ADC_DMA_CLK		RCC_AHB1Periph_DMA2
#define		SKYBORNE_ADC_NVIC_PRIORITY	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1

//-------------宏定义---------------------------
typedef enum
{
	STICK_RV,
	STICK_RH,
	STICK_LH,
	STICK_LV,
	ROTATE_LU,
	ROTATE_LD,
	ROTATE_RU,
	ROTATE_RD,
	MONITOR_VOL,
	MODULE_NUMBER
}SKYBORNE_ADC_MODULES;

typedef enum
{
	KEY_READY,
	KEY_MENU,
	KEY_PAGE,
	KEY_EXIT,
	KEY_PLUS,
	KEY_MINUS,
	KEY_ENTER
}KEY_VALUE;

typedef enum
{
	KEY_RELEASED,
	KEY_PRESSED,
	KEY_HOLD
}KEY_STATUS;

typedef enum
{
	SLIDE_MID,
	SLIDE_UP,
	SLIDE_DOWN
}SLIDE_SWITCH_VALUE;

typedef struct
{
	SLIDE_SWITCH_VALUE	SA;
	SLIDE_SWITCH_VALUE	SB;
	SLIDE_SWITCH_VALUE	SC;
	SLIDE_SWITCH_VALUE	SD;
	SLIDE_SWITCH_VALUE	SE;
}MISC_SLIDE_SWITCH_VALUE;



//-------------函数声明---------------------------
void SKYBORNE_ADC_DMA_IRQHandlerCallback(void);
void keys_n_ADCs_init(void);
KEY_VALUE KEY_Read(KEY_STATUS *state);
MISC_SLIDE_SWITCH_VALUE Misc_Slide_Switch_Read(void);

#endif