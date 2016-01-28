#ifndef __AUDIO_H
#define __AUDIO_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_dac.h"
#include "Stm32f2xx_i2c.h"
#include "Stm32f2xx_tim.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_dma.h"
#include "cmsis_os.h"


//-----------------端口定义---------------- 
#define		AUDIO_DAC_PIN				GPIO_Pin_4
#define		AUDIO_DAC_PORT				GPIOA
#define		AUDIO_DAC_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define		AUDIO_DAC_PIN_SOURCE		GPIO_PinSource4

//DAC定义
#define		AUDIO_DAC					DAC
#define		AUDIO_DAC_CHANNEL			DAC_Channel_1
#define		AUDIO_DAC_CLK				RCC_APB1Periph_DAC
#define		AUDIO_DAC_DMA_STERAM		DMA1_Stream5
#define		AUDIO_DAC_DMA_CHANNEL		DMA_Channel_7
#define		AUDIO_DAC_DMA_IRQHandler	DMA1_Stream5_IRQHandler
#define		AUDIO_DAC_DMA_TC_FLAG		DMA_IT_TCIF7
#define		AUDIO_DAC_DMA_CLK			RCC_AHB1Periph_DMA1
#define		AUDIO_DAC_NVIC_PRIORITY		configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3

#define		AUDIO_DAC_TIMER				TIM6
#define		AUDIO_DAC_TIMER_CLK			RCC_APB1Periph_TIM6

#if !defined(EEPROM)
// I2C SCL
#define		IIC_EEPROM_SCL_PIN			GPIO_Pin_6
#define		IIC_EEPROM_SCL_PORT			GPIOB
#define		IIC_EEPROM_SCL_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		IIC_EEPROM_SCL_PIN_SOURCE	GPIO_PinSource6
#define		IIC_EEPROM_SCL_GPIO_AF		GPIO_AF_I2C1

//I2C SDA
#define		IIC_EEPROM_SDA_PIN			GPIO_Pin_7
#define		IIC_EEPROM_SDA_PORT			GPIOB
#define		IIC_EEPROM_SDA_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		IIC_EEPROM_SDA_PIN_SOURCE	GPIO_PinSource7
#define		IIC_EEPROM_SDA_GPIO_AF		GPIO_AF_I2C1

//I2C定义
#define		IIC_EEPROM_I2C				I2C1
#define		IIC_EEPROM_I2C_CLK			RCC_APB1Periph_I2C1

#define		IIC_EEPROM_SPEED			400000
#define 	I2C_TIMEOUT_MAX 			1000
#endif


#define		AUDIO_BUFF_MAX_LEN			1024

#define 	I2C_ADDRESS_CAT5137         0x5C
#define 	VOLUME_LEVEL_MAX			23



void Audio_SetSampleRate(uint32_t frequency);
void Audio_init(void);
uint8_t Audio_Start(uint16_t *str, uint16_t len);
void Audio_Stop(void);
void AUDIO_DAC_DMA_IRQHandlerCallback(void);
void Audio_SetVolume(uint8_t volume);

#endif