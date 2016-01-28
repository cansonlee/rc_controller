#ifndef __EEPROM_H
#define __EEPROM_H

#include "Stm32f2xx.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_i2c.h"
#include "cmsis_os.h"

#define		EEPROM

//-----------------端口定义----------------
// eeprom wp
#define		IIC_EEPROM_WP_PIN			GPIO_Pin_9
#define		IIC_EEPROM_WP_PORT			GPIOB
#define		IIC_EEPROM_WP_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		IIC_EEPROM_WP_PIN_SOURCE	GPIO_PinSource9

//I2C SCL
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


#define		I2C_ADDRESS_EEPROM			0xA2
#define		I2C_FLASH_PAGESIZE			64


void eepromInit(void);
void eepromReadBlock(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void eepromWriteBlock(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_WaitEepromStandbyState(void);


#endif