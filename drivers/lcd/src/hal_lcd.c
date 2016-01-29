#include "hal_lcd.h"
#include "delay_timer.h"

#include "font.h" 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/////////////////////全局变量定义///////////////////////////////////////
uint8_t				LcdWriteBusyFlag;					//LCD写繁忙标志
uint8_t				lcdTxBuff[LCD_H/8][LCD_W];			//LCD写缓存			



/***************************************************************************************************
 * @fn      LCD_Registe
 *
 * @brief   LCD资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/    
void LCD_Registe(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef	 DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//背光控制
	RCC_AHB1PeriphClockCmd(LCD_BLW_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_BLW_PIN |LCD_BL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_BLW_PORT, &GPIO_InitStructure);

	//SPI口配置
	RCC_AHB1PeriphClockCmd(LCD_SPI_CLK_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_CLK_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_SPI_CLK_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(LCD_SPI_CLK_PORT, LCD_SPI_CLK_PIN_SOURCE, LCD_SPI_CLK_GPIO_AF);

	RCC_AHB1PeriphClockCmd(LCD_SPI_MOSI_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_SPI_MOSI_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(LCD_SPI_MOSI_PORT, LCD_SPI_MOSI_PIN_SOURCE, LCD_SPI_MOSI_GPIO_AF);

	RCC_AHB1PeriphClockCmd(LCD_SPI_NCS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_NCS_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_SPI_NCS_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(LCD_SPI_NCS_PORT, LCD_SPI_NCS_PIN_SOURCE, LCD_SPI_NCS_GPIO_AF);

	//A0端口配置
	RCC_AHB1PeriphClockCmd(LCD_A0_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_A0_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_A0_PORT, &GPIO_InitStructure);

	//RST端口配置	
	RCC_AHB1PeriphClockCmd(LCD_RST_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

	//SPI功能配置
	RCC_APB1PeriphClockCmd(LCD_SPI_MASTER_CLK, ENABLE);
	/* SPI_MASTER configuration ------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(LCD_SPI_MASTER, &SPI_InitStructure);
//	SPI_I2S_ITConfig(LCD_SPI_MASTER,SPI_I2S_IT_TXE,DISABLE);
//	SPI_I2S_ITConfig(LCD_SPI_MASTER,SPI_I2S_IT_RXNE,DISABLE);
	/* Enable SPI_MASTER NSS output for master mode */
	SPI_SSOutputCmd(LCD_SPI_MASTER, ENABLE);
	/* Enable SPI_MASTER DMA TX */
	RCC_AHB1PeriphClockCmd(LCD_DMA_CLK, ENABLE);
	DMA_DeInit(LCD_DMA_STREAM);
	DMA_InitStructure.DMA_Channel = LCD_DMA_CHANNLE;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)LCD_SPI_MASTER->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)lcdTxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = LCD_TX_BUFF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;	
	DMA_Init(LCD_DMA_STREAM, &DMA_InitStructure);
	DMA_Cmd(LCD_DMA_STREAM, DISABLE);
	SPI_I2S_DMACmd(LCD_SPI_MASTER,SPI_I2S_DMAReq_Tx,ENABLE);
	DMA_ITConfig(LCD_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(LCD_DMA_STREAM, LCD_DMA_TC_FLAG);

	//中断控制
	NVIC_InitStructure.NVIC_IRQChannel = LCD_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LCD_DMA_NVIC_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable SPI_MASTER */
  	SPI_Cmd(LCD_SPI_MASTER, ENABLE);	
	
}

/***************************************************************************************************
 * @fn      LCD_WriteCommand
 *
 * @brief   LCD命令写入
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void LCD_WriteCommand(uint8_t cmd)
{
	GPIO_ResetBits(LCD_A0_PORT, LCD_A0_PIN);
	while ( ( LCD_SPI_MASTER->SR & SPI_SR_TXE ) == 0 ) 
	{};
  	(void)LCD_SPI_MASTER->DR ;		// Clear receive
  	LCD_SPI_MASTER->DR = cmd ;
  	while ( ( LCD_SPI_MASTER->SR & SPI_SR_RXNE ) == 0 ) 
	{};
}

/***************************************************************************************************
 * @fn      LCD_WriteData
 *
 * @brief   LCD字节写入
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void LCD_WriteData(uint8_t data)
{
	GPIO_SetBits(LCD_A0_PORT, LCD_A0_PIN);
	while ( ( LCD_SPI_MASTER->SR & SPI_SR_TXE ) == 0 ) 
	{};
  	(void)LCD_SPI_MASTER->DR ;		// Clear receive
  	LCD_SPI_MASTER->DR = data ;
  	while ( ( LCD_SPI_MASTER->SR & SPI_SR_RXNE ) == 0 ) 
	{};
}


/***************************************************************************************************
 * @fn      LCD_DataBurstWrite
 *
 * @brief   LCD数据批量写入
 * @param   buf -- 要写入的数据的缓存
 *			len -- 写入数据的长度
 * @return  0 --  busy
 *			-1 -- 长度超长
 *			1 --  成功 
 ***************************************************************************************************/  
 int8_t LCD_DataBurstWrite(uint8_t *buf, uint16_t len)
{
	if(LcdWriteBusyFlag)
		return 0;

	LcdWriteBusyFlag = 1;

	LCD_SetCursorAddr(0,0);
	
	if(len > LCD_TX_BUFF_SIZE)
		return -1;

	GPIO_SetBits(LCD_A0_PORT, LCD_A0_PIN);
	
	memcpy(lcdTxBuff, buf, len);
	DMA_SetCurrDataCounter(LCD_DMA_STREAM, len);
	DMA_Cmd(LCD_DMA_STREAM,ENABLE);
	
	return 1;
}

/***************************************************************************************************
 * @fn      LCD_DMA_IRQHandlerCallback
 *
 * @brief   LCD DMA写入中断回调函数，完成关闭DMA
 * @param   NULL
 * @return  NULL
 ***************************************************************************************************/  
 void LCD_DMA_IRQHandlerCallback(void)
{
	  if(DMA_GetITStatus(LCD_DMA_STREAM, LCD_DMA_TC_FLAG) != RESET)
	  {
	    DMA_ClearITPendingBit(LCD_DMA_STREAM, LCD_DMA_TC_FLAG);
	    DMA_Cmd(LCD_DMA_STREAM,DISABLE);
		LcdWriteBusyFlag = 0;
	  }
}


/***************************************************************************************************
 * @fn      LCD_SetCursorAddr
 *
 * @brief   LCD数据批量写入
 * @param   x,y坐标
 * @return  null
 ***************************************************************************************************/ 
 void LCD_SetCursorAddr(uint8_t x, uint8_t y)
{
	LCD_WriteCommand(x&0x0F);				//Set Column Address LSB CA[3:0]
  	LCD_WriteCommand((x>>4)|0x10);			//Set Column Address MSB CA[7:4]
    
  	LCD_WriteCommand((y&0x0F)|0x60);		//Set Row Address LSB RA [3:0]
  	LCD_WriteCommand(((y>>4)&0x0F)|0x70);  //Set Row Address MSB RA [7:4]
}


/***************************************************************************************************
 * @fn      LCD_Init
 *
 * @brief   LCD设备初始化
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
 void LCD_Init(void)
{
	LCD_Registe();

	//背光开启
	GPIO_SetBits(LCD_BL_PORT, LCD_BL_PIN);
	GPIO_SetBits(LCD_BLW_PORT, LCD_BLW_PIN);
	
	//LCD RST
	GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);	
	Delay_ms(1);
	GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);
	Delay_ms(300);

	//LCD 寄存器
	LCD_WriteCommand(0x2F);   //Internal pump control
	Delay_ms(20);
	LCD_WriteCommand(0x24);   //Temperature compensation
	LCD_WriteCommand(0xE9);   //set bias=1/10
	LCD_WriteCommand(0x81);   //Set Vop
	LCD_WriteCommand(160+25);
	LCD_WriteCommand(0xA2);   //set line rate:28KLPS
	LCD_WriteCommand(0x28);   //set pannel loading
	LCD_WriteCommand(0x40);   //scroll line LSB
	LCD_WriteCommand(0x50);   //SCROLL LINE MSB
	LCD_WriteCommand(0x89);   //ram address control
	LCD_WriteCommand(0xC0);   //LCD mapping control
	LCD_WriteCommand(0x04);   //MX=0,MY=1
	LCD_WriteCommand(0xD0);   //DISPLAY PATTERN = 16-SCALE GRAY
	LCD_WriteCommand(0xF1);   //SET COM end
	LCD_WriteCommand(0x3F);   //64

	LCD_WriteCommand(0xF8);   //Set Window Program Disable.

	LCD_WriteCommand(0xF5);   //starting row address of RAM program window.PAGE1
	LCD_WriteCommand(0x00);
	LCD_WriteCommand(0xF7);   //end row address of RAM program window.PAGE32
	LCD_WriteCommand(0x1F);
	LCD_WriteCommand(0xF4);   //start column address of RAM program window.
	LCD_WriteCommand(0x00);
	LCD_WriteCommand(0xF6);   //end column address of RAM program window.SEG212
	LCD_WriteCommand(0xD3);

	LCD_WriteCommand(0xAF);	//dc2=1, IC into exit SLEEP MODE, dc3=1 gray=ON, dc4=1 Green Enhanc mode disabled
  	Delay_ms(20);      //needed for internal DC-DC converter startup
}

//////////////end of file/////////////////////////////////////////////////////
