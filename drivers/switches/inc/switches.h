#ifndef __SWITCHES_H
#define __SWITCHES_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "cmsis_os.h"

#include <stdint.h>



//-----------------拨动开关端口定义---------------- 
//SA拨钮接口
#define		SW_SA_H_PIN			GPIO_Pin_5
#define		SW_SA_H_PORT			GPIOB
#define		SW_SA_H_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SW_SA_H_PIN_SOURCE		GPIO_PinSource5

#define		SW_SA_L_PIN			GPIO_Pin_0
#define		SW_SA_L_PORT			GPIOE
#define		SW_SA_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SA_L_PIN_SOURCE		GPIO_PinSource0

//SB拨钮接口
#define		SW_SB_H_PIN			GPIO_Pin_2
#define		SW_SB_H_PORT			GPIOE
#define		SW_SB_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SB_H_PIN_SOURCE		GPIO_PinSource2

#define		SW_SB_L_PIN			GPIO_Pin_1
#define		SW_SB_L_PORT			GPIOE
#define		SW_SB_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SB_L_PIN_SOURCE		GPIO_PinSource1

//SC拨钮接口
#define		SW_SC_H_PIN			GPIO_Pin_15
#define		SW_SC_H_PORT			GPIOE
#define		SW_SC_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SC_H_PIN_SOURCE		GPIO_PinSource15

#define		SW_SC_L_PIN			GPIO_Pin_5
#define		SW_SC_L_PORT			GPIOA
#define		SW_SC_L_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		SW_SC_L_PIN_SOURCE		GPIO_PinSource5

//SD拨钮接口
#define		SW_SD_H_PIN			GPIO_Pin_7
#define		SW_SD_H_PORT			GPIOE
#define		SW_SD_H_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SD_H_PIN_SOURCE		GPIO_PinSource7

#define		SW_SD_L_PIN			GPIO_Pin_13
#define		SW_SD_L_PORT			GPIOE
#define		SW_SD_L_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define		SW_SD_L_PIN_SOURCE		GPIO_PinSource13

//SE拨钮接口
#define		SW_SE_H_PIN			GPIO_Pin_4
#define		SW_SE_H_PORT			GPIOB
#define		SW_SE_H_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SW_SE_H_PIN_SOURCE		GPIO_PinSource4

#define		SW_SE_L_PIN			GPIO_Pin_3
#define		SW_SE_L_PORT			GPIOB
#define		SW_SE_L_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SW_SE_L_PIN_SOURCE		GPIO_PinSource3

//-------------宏定义---------------------------
#define		SW_SA_MASK				0x0003
#define		SW_SB_MASK				0x000C
#define		SW_SC_MASK				0x0030
#define		SW_SD_MASK				0x00C0
#define		SW_SE_MASK				0x0300

#define		SW_SA_L_SHIFT			0
#define		SW_SB_L_SHIFT			2
#define		SW_SC_L_SHIFT			4
#define		SW_SD_L_SHIFT			6
#define		SW_SE_L_SHIFT			8

#define		GET_SW_SA_VAL(x)		(x & SW_SA_MASK)			//取得SA拨档开关值
#define		GET_SW_SB_VAL(x)		(x & SW_SB_MASK)			//取得SB拨档开关值
#define		GET_SW_SC_VAL(x)		(x & SW_SC_MASK)			//取得SC拨档开关值
#define		GET_SW_SD_VAL(x)		(x & SW_SD_MASK)			//取得SD拨档开关值
#define		GET_SW_SE_VAL(x)		(x & SW_SE_MASK)			//取得SE拨档开关值


//-------------结构定义---------------------------

typedef enum sw_val_e
{
	SLIDE_MID,
	SLIDE_UP,
	SLIDE_DOWN
}SW_VAL;

typedef union misc_sw_val_e
{
    uint16_t sws_value;
    struct misc_sw_value_s
    {
    	uint16_t SA : 2;
    	uint16_t SB : 2;
    	uint16_t SC : 2;
    	uint16_t SD : 2;
    	uint16_t SE : 2;
    	uint16_t RES: 6;
    }SWS;
}MISC_SW_VALUE;


//-------------函数声明---------------------------

void swithes_init(void);
MISC_SW_VALUE switches_misc_sw_read(void);

#endif
