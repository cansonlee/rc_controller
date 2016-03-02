#ifndef __LCD_H
#define __LCD_H		

#include "Stm32f2xx.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_spi.h"
#include "Stm32f2xx_dma.h"
#include "cmsis_os.h"
#include "driver_global.h"



//-----------------LCD�˿ڶ���---------------- 
//����ӿ�
#define		LCD_BLW_PIN				GPIO_Pin_13
#define		LCD_BLW_PORT			GPIOD
#define		LCD_BLW_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		LCD_BLW_PIN_SOURCE		GPIO_PinSource13

#define		LCD_BL_PIN				GPIO_Pin_15
#define		LCD_BL_PORT				GPIOD
#define		LCD_BL_GPIO_CLK			RCC_AHB1Periph_GPIOD
#define		LCD_BL_PIN_SOURCE		GPIO_PinSource15


//SPI�ӿ�
#define		LCD_SPI_CLK_PIN			GPIO_Pin_10
#define		LCD_SPI_CLK_PORT		GPIOC
#define		LCD_SPI_CLK_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define		LCD_SPI_CLK_PIN_SOURCE	GPIO_PinSource10
#define		LCD_SPI_CLK_GPIO_AF		GPIO_AF_SPI3

#define		LCD_SPI_MOSI_PIN		GPIO_Pin_12
#define		LCD_SPI_MOSI_PORT		GPIOC
#define		LCD_SPI_MOSI_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define		LCD_SPI_MOSI_PIN_SOURCE	GPIO_PinSource12
#define		LCD_SPI_MOSI_GPIO_AF	GPIO_AF_SPI3


#define		LCD_SPI_NCS_PIN			GPIO_Pin_15
#define		LCD_SPI_NCS_PORT		GPIOA
#define		LCD_SPI_NCS_GPIO_CLK	RCC_AHB1Periph_GPIOA
#define		LCD_SPI_NCS_PIN_SOURCE	GPIO_PinSource15
#define		LCD_SPI_NCS_GPIO_AF		GPIO_AF_SPI3



//A0�ӿ�
#define		LCD_A0_PIN				GPIO_Pin_11
#define		LCD_A0_PORT				GPIOC
#define		LCD_A0_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define		LCD_A0_PIN_SOURCE		GPIO_PinSource11


//RST�ӿ�
#define		LCD_RST_PIN				GPIO_Pin_12
#define		LCD_RST_PORT			GPIOD
#define		LCD_RST_GPIO_CLK		RCC_AHB1Periph_GPIOD
#define		LCD_RST_PIN_SOURCE		GPIO_PinSource12

//SPI�Ĵ���
#define		LCD_SPI_MASTER			SPI3
#define		LCD_SPI_MASTER_CLK		RCC_APB1Periph_SPI3
#define		LCD_DMA_CLK				RCC_AHB1Periph_DMA1
#define		LCD_DMA_STREAM			DMA1_Stream7
#define		LCD_DMA_CHANNLE			DMA_Channel_0
#define		LCD_DMA_IRQn			DMA1_Stream7_IRQn
#define		LCD_DMA_IRQHandler		DMA1_Stream7_IRQHandler
#define		LCD_DMA_TC_FLAG			DMA_IT_TCIF7
#define		LCD_DMA_NVIC_PRIORITY	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2


//---------------�궨��--------------------------------------

#define		LCD_A0_LOW()			{GPIO_ResetBits(LCD_A0_PORT, LCD_A0_PIN);}
#define		LCD_A0_HIGH()			{GPIO_SetBits(LCD_A0_PORT, LCD_A0_PIN);}

#define		LCD_NCS_LOW()			{GPIO_ResetBits(LCD_SPI_NCS_PORT, LCD_SPI_NCS_PIN);}
#define		LCD_NCS_HIGH()			{GPIO_SetBits(LCD_SPI_NCS_PORT, LCD_SPI_NCS_PIN);}

#define		LCD_RST_LOW()			{GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);}
#define		LCD_RST_HIGH()			{GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);}

#define		LCD_W         			212
#define 	LCD_H         			64
#define		LCD_TX_BUFF_SIZE		(LCD_W*LCD_H/8*4)				//(LCD_W*LCD_H*4/8)

//---------------�ⲿ����--------------------------------------


//---------------�������--------------------------------------
void lcd_init(void);
int lcd_data_burst_write(void);
void lcd_dma_irq_handler_callback(void);

void lcd_clean(void);

int lcd_str_disp(unsigned char x,unsigned char y,unsigned char *pCharStr);
int lcd_str_inv_disp(unsigned char x,unsigned char y,unsigned char *pCharStr);

int lcd_disp_bmp(uint8_t x, uint8_t y,  uint8_t *p_bmp, uint8_t width, uint8_t height);
int lcd_bmp_inv_disp(uint8_t x, uint8_t y,  uint8_t *p_bmp, uint8_t width, uint8_t height);

int lcd_hline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen);
int lcd_hline_erase(uint8_t x, uint8_t y, uint8_t len, uint8_t pen);
int lcd_vline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen);
int lcd_vline_erase(uint8_t x, uint8_t y, uint8_t len, uint8_t pen);
int lcd_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
int lcd_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);


#endif
