#include "lcd.h"
#include "delay_timer.h"

#include "font.h" 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/////////////////////全局变量定义///////////////////////////////////////
uint8_t				g_LcdWriteBusyFlag;					//LCD写繁忙标志
//uint8_t				g_lcdTxBuff[LCD_H/8][LCD_W];			//LCD写缓存	
uint8_t				g_lcdTxBuff[LCD_TX_BUFF_SIZE];			//LCD写缓存	


int lcd_cursor_addr_set(uint8_t x, uint8_t y);
int _lcd_char_disp(uint8_t x, uint8_t y, uint8_t* pattern, uint8_t w, uint8_t h, uint8_t inv);
int lcd_char_disp_5x7(uint8_t x, uint8_t y, uint8_t c, uint8_t inv);

/***************************************************************************************************
 * @fn      lcd_regist
 *
 * @brief   LCD资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/    
void lcd_regist(void)
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(LCD_SPI_NCS_PORT, &GPIO_InitStructure);
	//GPIO_PinAFConfig(LCD_SPI_NCS_PORT, LCD_SPI_NCS_PIN_SOURCE, LCD_SPI_NCS_GPIO_AF);

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
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(LCD_SPI_MASTER, &SPI_InitStructure);
//	SPI_I2S_ITConfig(LCD_SPI_MASTER,SPI_I2S_IT_TXE,DISABLE);
//	SPI_I2S_ITConfig(LCD_SPI_MASTER,SPI_I2S_IT_RXNE,DISABLE);
	/* Enable SPI_MASTER NSS output for master mode */
	SPI_SSOutputCmd(LCD_SPI_MASTER, DISABLE);
	/* Enable SPI_MASTER DMA TX */
	RCC_AHB1PeriphClockCmd(LCD_DMA_CLK, ENABLE);
	DMA_Cmd(LCD_DMA_STREAM, DISABLE);
	DMA_DeInit(LCD_DMA_STREAM);
	DMA_InitStructure.DMA_Channel = LCD_DMA_CHANNLE;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&LCD_SPI_MASTER->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_lcdTxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = LCD_TX_BUFF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;		
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(LCD_DMA_STREAM, &DMA_InitStructure);

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
 * @fn      lcd_cmd_write
 *
 * @brief   LCD命令写入
 * @param   NULL
 * @return  0 -- success
 ***************************************************************************************************/  
int lcd_cmd_write(uint8_t cmd)
{
	LCD_A0_LOW();
	LCD_NCS_LOW()
	while ( ( LCD_SPI_MASTER->SR & SPI_SR_TXE ) == 0 ) 
	{};
  	(void)LCD_SPI_MASTER->DR ;		// Clear receive
  	LCD_SPI_MASTER->DR = cmd ;
  	while ( ( LCD_SPI_MASTER->SR & SPI_SR_RXNE ) == 0 ) 
	{};
	LCD_NCS_HIGH();

	return 0;
}

/***************************************************************************************************
 * @fn      lcd_data_write
 *
 * @brief   LCD字节写入
 * @param   NULL
 * @return  0 -- success
 ***************************************************************************************************/  
void lcd_data_write(uint8_t data)
{
	LCD_A0_HIGH();
	LCD_NCS_LOW()
	while ( ( LCD_SPI_MASTER->SR & SPI_SR_TXE ) == 0 ) 
	{};
  	(void)LCD_SPI_MASTER->DR ;		// Clear receive
  	LCD_SPI_MASTER->DR = data ;
  	while ( ( LCD_SPI_MASTER->SR & SPI_SR_RXNE ) == 0 ) 
	{};
	LCD_NCS_HIGH();
	
	return 0;
}


/***************************************************************************************************
 * @fn      lcd_data_burst_write
 *
 * @brief   LCD数据批量写入
 * @param   buf -- 要写入的数据的缓存
 *			len -- 写入数据的长度
 * @return  1 --  busy
 *			-1 - failure
 *			0 --  success 
 ***************************************************************************************************/  
int lcd_data_burst_write(uint8_t *buf, uint16_t len)
{
	IF_PTR_IS_NULL_RET_NULLPTR_ERR(buf);

	if(len > LCD_TX_BUFF_SIZE)
	{
		printf("lcd write lenth is too large,return @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
		return -1;
	}	
	
	if(g_LcdWriteBusyFlag)
	{
		printf("lcd is busy, return @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
		return 1;
	}		

	g_LcdWriteBusyFlag = 1;

	lcd_cursor_addr_set(0,0);		

	LCD_A0_HIGH();
	LCD_NCS_LOW();

	//启动DMA
	DMA_Cmd(LCD_DMA_STREAM, DISABLE);
	DMA_ClearITPendingBit(LCD_DMA_STREAM, LCD_DMA_TC_FLAG);

	memcpy(g_lcdTxBuff, buf, len);
	DMA_SetCurrDataCounter(LCD_DMA_STREAM, len);
	
	SPI_I2S_DMACmd(LCD_SPI_MASTER,SPI_I2S_DMAReq_Tx,ENABLE);
	DMA_ITConfig(LCD_DMA_STREAM, DMA_IT_TC, ENABLE);		
	
	DMA_Cmd(LCD_DMA_STREAM,ENABLE);

	printf("begin lcd dma write @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
	printf(" SPI3->CR1:%#x, SPI3->CR2:%#x, SPI3->SR:%#x, DMA1_Stream7->CR:%#x, DMA1->HIFCR:%#x\r\n", SPI3->CR1, SPI3->CR2, SPI3->SR, DMA1_Stream7->CR, DMA1->HIFCR);
	
	return 0;
}

/***************************************************************************************************
 * @fn      lcd_dma_irq_handler_callback
 *
 * @brief   LCD DMA写入中断回调函数，完成关闭DMA
 * @param   NULL
 * @return  NULL
 ***************************************************************************************************/  
 void lcd_dma_irq_handler_callback(void)
{
	  if(DMA_GetITStatus(LCD_DMA_STREAM, LCD_DMA_TC_FLAG) != RESET)
	  {
	 	DMA_Cmd(LCD_DMA_STREAM,DISABLE);
	    DMA_ClearITPendingBit(LCD_DMA_STREAM, LCD_DMA_TC_FLAG);
		DMA_ITConfig(LCD_DMA_STREAM, DMA_IT_TC, DISABLE);
	    SPI_I2S_DMACmd(LCD_SPI_MASTER,SPI_I2S_DMAReq_Tx,DISABLE);

		while ( LCD_SPI_MASTER->SR & SPI_SR_BSY ) 
		{
		    /* Wait for SPI to finish sending data 
		    The DMA TX End interrupt comes two bytes before the end of SPI transmission,
		    therefore we have to wait here.
		    */
		}
		LCD_NCS_HIGH();
  
		g_LcdWriteBusyFlag = 0;
		printf("enter in lcd dma irq @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
	  }
}


/***************************************************************************************************
 * @fn      lcd_cursor_addr_set
 *
 * @brief   LCD数据批量写入
 * @param   x,y坐标
 * @return  0 -- success
 ***************************************************************************************************/ 
int lcd_cursor_addr_set(uint8_t x, uint8_t y)
{
	lcd_cmd_write(x&0x0F);				//Set Column Address LSB CA[3:0]
  	lcd_cmd_write((x>>4)|0x10);			//Set Column Address MSB CA[7:4]
    
  	lcd_cmd_write((y&0x0F)|0x60);		//Set Row Address LSB RA [3:0]
  	lcd_cmd_write(((y>>4)&0x0F)|0x70);  //Set Row Address MSB RA [7:4]

	return 0;
}


/***************************************************************************************************
 * @fn      lcd_init
 *
 * @brief   LCD设备初始化
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void lcd_init(void)
{
	lcd_regist();

	
	//背光开启
	GPIO_SetBits(LCD_BL_PORT, LCD_BL_PIN);
	GPIO_SetBits(LCD_BLW_PORT, LCD_BLW_PIN);
	
	//LCD RST
	LCD_RST_LOW();	
	delay_ms(1);
	LCD_RST_HIGH();
	delay_ms(300);

	//LCD 寄存器
	lcd_cmd_write(0x2F);   //Internal pump control
	delay_ms(20);
	lcd_cmd_write(0x24);   //Temperature compensation
	lcd_cmd_write(0xE9);   //set bias=1/10
	lcd_cmd_write(0x81);   //Set Vop
	lcd_cmd_write(160+25);
	lcd_cmd_write(0xA2);   //set line rate:28KLPS
	lcd_cmd_write(0x28);   //set pannel loading
	lcd_cmd_write(0x40);   //scroll line LSB
	lcd_cmd_write(0x50);   //SCROLL LINE MSB
//	lcd_cmd_write(0x89);   //ram address control
	lcd_cmd_write(0x8B);   //ram address control, PAGE increment first
	lcd_cmd_write(0xC0);   //LCD mapping control
//	lcd_cmd_write(0x04);   //MX=0,MY=1
	lcd_cmd_write(0x01);   //MX=0,MY=0,MSF=1
	lcd_cmd_write(0xD0);   //DISPLAY PATTERN = 16-SCALE GRAY
	lcd_cmd_write(0xF1);   //SET COM end
	lcd_cmd_write(0x3F);   //64

	lcd_cmd_write(0xF8);   //Set Window Program inside.

	lcd_cmd_write(0xF5);   //starting row address of RAM program window.PAGE1
	lcd_cmd_write(0x00);
	lcd_cmd_write(0xF7);   //end row address of RAM program window.PAGE32
	lcd_cmd_write(0x1F);
	lcd_cmd_write(0xF4);   //start column address of RAM program window.
	lcd_cmd_write(0x00);
//	lcd_cmd_write(0xEF);
	lcd_cmd_write(0xF6);   //end column address of RAM program window.SEG212
	lcd_cmd_write(0xD3);
//	lcd_cmd_write(0x1B);

	lcd_cmd_write(0xAF);	//dc2=1, IC into exit SLEEP MODE, dc3=1 gray=ON, dc4=1 Green Enhanc mode disabled
  	delay_ms(20);      //needed for internal DC-DC converter startup
  	printf("lcd init over! @ %s,%s,%d\r\n", __FILE__, __func__, __LINE__);
}


/***************************************************************************************************
 * @fn      lcd_char_disp
 *
 * @brief   液晶显示一个字符
 * @param   x,y -- 液晶坐标
 *			dispByte -- 要显示的数据
 * @return  null
 ***************************************************************************************************/ 
int _lcd_char_disp(uint8_t x, uint8_t y, uint8_t* pattern, uint8_t w, uint8_t h, uint8_t inv){
    uint8_t lines = (h + 7) / 8;
    uint8_t line, col, page, trans_x, origin_y, dat_H4, dat_L4, mask, pat;

    origin_y = (y % 2) ? (y / 2 + 1) : (y / 2);
	trans_x = 211 - x;
    
    for(line = 0; line < lines; line++)
    {
        for(col = 0; col < w; col++)
        {
            lcd_cursor_addr_set(trans_x - col, origin_y + (line * 4));

            mask = 0x80;
            pat = *pattern;

            if (inv){pat = ~pat;}

            // pattern 1bit <-> lcd 1pixel
            // lcd 1pixel -> lcd disp buf 4bits
            // lcd 2pixel as a unit
            // so 1Byte -> 8pixel need draw 4 times
            for(page = 0; page < 4; page++)
            {
                dat_H4 = ((pat & mask) == 0) ? 0x00 : 0xF0; 
                            
                mask >>=1;
                dat_L4 = ((pat & mask) == 0) ? 0x00 : 0x0F;
                
                mask >>=1;
                
                lcd_data_write(dat_H4 | dat_L4);          
            }
            
            pattern++;
        }
    }

    return 0;
}

int lcd_char_disp_8x16(uint8_t x, uint8_t y, uint8_t c){
    uint8_t* pattern;
    
    if ( c > '~'){
        c = ' ';
    }

    pattern = &ascii_0816[(c - ' ') * 16];

    return _lcd_char_disp(x, y, pattern, 8, 16, 0);
}

int lcd_char_disp_5x7(uint8_t x, uint8_t y, uint8_t c, uint8_t inv){
    uint8_t* pattern;
    
    if ( c > 'z'){
        c = ' ';
    }
    
    pattern = &ascii_5x7[(c - ' ') * 5];

    return _lcd_char_disp(x, y, pattern, 5, 7, inv);
}

/***************************************************************************************************
 * @fn      lcd_str_disp
 *
 * @brief   液晶显示段连续字符
 * @param   x,y -- 液晶坐标
 *			pCharStr -- 要显示的字符串
 * @return  0 -- success
 *			-1 - failure
 ***************************************************************************************************/ 
int lcd_str_disp(unsigned char x,unsigned char y,unsigned char *pCharStr)
{
	do
	{
		IF_PTR_IS_NULL_RET_NULLPTR_ERR(pCharStr);
		
		lcd_char_disp_5x7(x, y, *pCharStr, 0);
		x+=8;
		if(x>=(LCD_W-1))													// 写满自动跳转下一行
		{
			x = 0;
			y += 16;
		}
		pCharStr++;
	}while(*pCharStr);

	return 0;
}


/***************************************************************************************************
 * @fn      lcd_disp_bmp
 *
 * @brief   LCD显示图象
 * @param   null
 * @return  0 -- success
 *			-1 - failure
 ***************************************************************************************************/ 
int lcd_disp_bmp(uint8_t x, uint8_t y,  uint8_t *p_bmp, uint8_t width, uint8_t height)
{
	uint8_t t, col, sec, page, trans_x, origin_y;
	uint8_t dat_H4, dat_L4, Shift;
	
	IF_CONDITION_TURE_RET_PARAM_ERR(width > LCD_W);
	IF_CONDITION_TURE_RET_PARAM_ERR(height > LCD_H);
	IF_CONDITION_TURE_RET_PARAM_ERR(((height%8) != 0) && (((height/8 + 1)*8)> LCD_H));

	origin_y = (y%2) ? (y/2 + 1) : (y/2);
	trans_x = 211 - x;						//该款液晶原点在右上角
	t = (height%8) ? (height/8 +1) : (height/8);

	for(sec=0; sec<t; sec++)
	{
		for(col=0; col<width; col++)
		{
			//lcd_cursor_addr_set(x+col,origin_y+(sec*4));
			lcd_cursor_addr_set(trans_x - col,origin_y+(sec*4));
			Shift=0x80;
			for(page=0; page<4; page++)
			{
				if((*p_bmp&Shift)==0)
					dat_H4=0x00;
				else 
					dat_H4=0xf0;			
				Shift>>=1;
				if((*p_bmp&Shift)==0)
					dat_L4=0x00;
				else 
					dat_L4=0x0f;
				Shift>>=1;
				lcd_data_write(dat_H4|dat_L4);			
			}
			p_bmp++;
		}
	}
	
	return 0;
}


/***************************************************************************************************
 * @fn      lcd_str_inv_disp
 *
 * @brief   反白显示段连续字符
 * @param   x,y -- 液晶坐标
 *			pCharStr -- 要显示的字符串
 * @return  0 -- success
 *			-1 - failure
 ***************************************************************************************************/
int lcd_str_inv_disp(unsigned char x,unsigned char y,unsigned char *pCharStr)
{
	do
	{
		IF_PTR_IS_NULL_RET_NULLPTR_ERR(pCharStr);
		
		lcd_char_disp_5x7(x, y, *pCharStr, 1);
		x+=8;
		if(x>=(LCD_W-1))													// 写满自动跳转下一行
		{
			x = 0;
			y += 16;
		}
		pCharStr++;
	}while(*pCharStr);

	return 0;
}


/***************************************************************************************************
 * @fn      lcd_bmp_inv_disp
 *
 * @brief   LCD显示反白图象
 * @param   null
 * @return  0 -- success
 *			-1 - failure
 ***************************************************************************************************/ 
int lcd_bmp_inv_disp(uint8_t x, uint8_t y,  uint8_t *p_bmp, uint8_t width, uint8_t height)
{
	uint8_t t, col, sec, page, trans_x, origin_y;
	uint8_t dat_H4, dat_L4, Shift;
	
	IF_CONDITION_TURE_RET_PARAM_ERR(width > LCD_W);
	IF_CONDITION_TURE_RET_PARAM_ERR(height > LCD_H);
	IF_CONDITION_TURE_RET_PARAM_ERR(((height%8) != 0) && (((height/8 + 1)*8)> LCD_H));

	origin_y = (y%2) ? (y/2 + 1) : (y/2);
	trans_x = 211 - x;						//该款液晶原点在右上角
	t = (height%8) ? (height/8 +1) : (height/8);

	for(sec=0; sec<t; sec++)
	{
		for(col=0; col<width; col++)
		{
			lcd_cursor_addr_set(trans_x - col,origin_y+(sec*4));
			Shift=0x80;
			for(page=0; page<4; page++)
			{
				if(((~(*p_bmp))&Shift)==0)
					dat_H4=0x00;
				else 
					dat_H4=0xf0;			
				Shift>>=1;
				if(((~(*p_bmp))&Shift)==0)
					dat_L4=0x00;
				else 
					dat_L4=0x0f;
				Shift>>=1;
				lcd_data_write(dat_H4|dat_L4);			
			}
			p_bmp++;
		}
	}
	
	return 0;
}


/***************************************************************************************************
 * @fn      lcd_area_clear
 *
 * @brief   清空指定区域
 * @param   x,y -- 液晶坐标
 *			width -- 区域宽度
 *			hight -- 区域高度
 * @return  0 -- success
 *			-1 - failure
 ***************************************************************************************************/ 

int lcd_area_clear(uint8_t x, uint8_t y,  uint8_t width, uint8_t height)
{
	uint8_t t, col, sec, page, trans_x, origin_y;
	
	IF_CONDITION_TURE_RET_PARAM_ERR(width > LCD_W);
	IF_CONDITION_TURE_RET_PARAM_ERR(height > LCD_H);
	IF_CONDITION_TURE_RET_PARAM_ERR(((height%8) != 0) && (((height/8 + 1)*8)> LCD_H));

	
	origin_y = (y%2) ? (y/2 + 1) : (y/2);
	trans_x = 211 - x;						//该款液晶原点在右上角
	t = (height%8) ? (height/8 +1) : (height/8);

	for(sec=0; sec<t; sec++)
	{
		for(col=0; col<width; col++)
		{
			lcd_cursor_addr_set(trans_x - col,origin_y+(sec*4));
			for(page=0; page<4; page++)
			{
				lcd_data_write(0x00);			
			}
		}
	}

	return 0;
}


/***************************************************************************************************
 * @fn      lcd_clean
 *
 * @brief   LCD清屏
 * @param   null
 * @return  0 -- success
 ***************************************************************************************************/ 
int lcd_clean(void)
{
	uint8_t	 lcdSendBuff[LCD_TX_BUFF_SIZE];
	printf("before burst write@ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
	memset(lcdSendBuff, 0, sizeof(lcdSendBuff));
	
	lcd_data_burst_write(lcdSendBuff, LCD_TX_BUFF_SIZE);
	
	return 0;
}


//////////////end of file/////////////////////////////////////////////////////
