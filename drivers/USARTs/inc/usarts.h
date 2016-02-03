#ifndef __USARTS_H
#define __USARTS_H

#include "Stm32f2xx.h"
#include "Stm32f2xx_gpio.h"
#include "Stm32f2xx_usart.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_dma.h"
#include "cmsis_os.h"
#include "driver_global.h"



#define USART_S_PORT               	   USART2
#define USART_S_PORT_GPIO              GPIOD
#define USART_S_PORT_CLK               RCC_APB1Periph_USART2
#define USART_S_PORT_GPIO_CLK          RCC_AHB1Periph_GPIOD
#define USART_S_PORT_RxPin             GPIO_Pin_6
#define USART_S_PORT_Rx_Pinsource	   GPIO_PinSource6
#define USART_S_PORT_TxPin             GPIO_Pin_5
#define USART_S_PORT_Tx_Pinsource	   GPIO_PinSource5
#define	USART_S_PORT_GPIO_AF		   GPIO_AF_USART2

#define	USART_S_PORT_DMA_CLK		   RCC_AHB1Periph_DMA1
#define USART_S_PORT_Tx_DMA_STREAM	   DMA1_Stream6
#define USART_S_PORT_Tx_DMA_Channel    DMA_Channel_4
#define USART_S_PORT_Tx_DMA_FLAG       DMA_IT_TCIF4   
#define USART_S_PORT_Tx_DMA_IRQn       DMA1_Stream6_IRQn 
#define USART_S_PORT_Tx_DMA_NVIC_PRIORITY	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3

#define USART_S_PORT_Rx_DMA_STREAM	   DMA1_Stream5
#define USART_S_PORT_Rx_DMA_Channel    DMA_Channel_4
#define USART_S_PORT_Rx_DMA_FLAG       DMA_IT_TCIF4    
#define USART_S_PORT_Rx_DMA_IRQn       DMA1_Stream5_IRQn  
#define USART_S_PORT_IRQn              USART2_IRQn
#define USART_S_PORT_Rx_NVIC_PRIORITY  configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 4



#define USARTdbg                   		USART3
#define USARTdbg_GPIO              		GPIOB
#define USARTdbg_CLK               		RCC_APB1Periph_USART3
#define USARTdbg_GPIO_CLK          		RCC_AHB1Periph_GPIOB
#define USARTdbg_RxPin             		GPIO_Pin_11
#define USARTdbg_Rx_Pinsource	   		GPIO_PinSource11
#define USARTdbg_TxPin             		GPIO_Pin_10
#define USARTdbg_Tx_Pinsource	   		GPIO_PinSource10
#define	USARTdbg_GPIO_AF		   		GPIO_AF_USART3

#define	USARTdbg_DMA_CLK		   		RCC_AHB1Periph_DMA1
#define USARTdbg_Tx_DMA_STREAM			DMA1_Stream3
#define USARTdbg_Tx_DMA_Channel    		DMA_Channel_4
#define USARTdbg_Tx_DMA_FLAG       		DMA_IT_TCIF4   
#define USARTdbg_Tx_DMA_IRQn       		DMA1_Stream3_IRQn
#define USARTdbg_Tx_DMA_NVIC_PRIORITY	configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 5

#define USARTdbg_Rx_DMA_STREAM			DMA1_Stream1
#define USARTdbg_Rx_DMA_Channel    		DMA_Channel_4
#define USARTdbg_Rx_DMA_FLAG       		DMA_IT_TCIF4  
#define USARTdbg_Rx_DMA_IRQn       		DMA1_Stream1_IRQn  
#define USARTdbg_IRQn              		USART3_IRQn
#define USARTdbg_Rx_NVIC_PRIORITY		configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 6


#define	S_PORT_TX_BUFF_LEN				200
#define S_PORT_RX_BUFF_LEN				200

#define DBG_TX_BUF_LEN					300
#define DBG_RX_BUF_LEN					300



void uarts_init(void);
int uarts_sport_send(uint8_t *Str, uint8_t len);
void uarts_sport_tx_dma_irq_handler_callback(void);
void uarts_sport_irq_handler_callback(void);
int uarts_dbg_send(uint8_t *Str, uint16_t len);
void uarts_dbg_tx_dma_irq_handler_callback(void);
void uarts_dbg_irq_handler_callback(void);



#endif
