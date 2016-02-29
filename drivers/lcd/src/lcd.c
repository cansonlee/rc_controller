#include "lcd.h"
#include "delay_timer.h"

#include "font.h" 
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/////////////////////全局变量定义///////////////////////////////////////

static uint8_t g_LcdWriteBusyFlag;					//LCD写繁忙标志
static uint8_t g_lcdTxBuff[LCD_TX_BUFF_SIZE];			//LCD写缓存	


void _lcd_regist(void);
int lcd_cmd_write(uint8_t cmd);
int _lcd_cursor_addr_set(uint8_t x, uint8_t y);
int _lcd_char_disp(uint8_t x, uint8_t y, uint8_t* pattern, uint8_t w, uint8_t h, uint8_t inv);
int _lcd_char_disp_5x7(uint8_t x, uint8_t y, uint8_t c, uint8_t inv);
int _lcd_str_disp_5x7(unsigned char x,unsigned char y,unsigned char *pCharStr, uint8_t inv);
inline uint8_t _lcd_is_point_outside(uint8_t x, uint8_t y);
int _lcd_plot(uint8_t x, uint8_t y);
int _lcd_erase(uint8_t x, uint8_t y);
int _lcd_plot_x2(uint8_t x, uint8_t y);
int _lcd_erase_x2(uint8_t x, uint8_t y);
int _lcd_hline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen,
    int (*x2_fn)(uint8_t, uint8_t),
    int (*fn)(uint8_t, uint8_t));


#define LCD_MIRROR_X(x) 211 - (x)
#define GET_BUF_POINT_ADDR(x, y) &g_lcdTxBuff[ (y) / 2 * LCD_W + (x) ]

/***************************************************************************************************
 * @fn      lcd_regist
 *
 * @brief   LCD资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/    
void _lcd_regist(void)
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
 * @fn      lcd_data_burst_write
 *
 * @brief   LCD数据批量写入
 * @return  1 --  busy
 *			-1 - failure
 *			0 --  success 
 ***************************************************************************************************/  
int lcd_data_burst_write(void)
{
	if(g_LcdWriteBusyFlag)
	{
		printf("lcd is busy, return @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
		return 1;
	}		

	g_LcdWriteBusyFlag = 1;

	_lcd_cursor_addr_set(0,0);		

	LCD_A0_HIGH();
	LCD_NCS_LOW();

	//启动DMA
	DMA_Cmd(LCD_DMA_STREAM, DISABLE);
	DMA_ClearITPendingBit(LCD_DMA_STREAM, LCD_DMA_TC_FLAG);

	DMA_SetCurrDataCounter(LCD_DMA_STREAM, LCD_TX_BUFF_SIZE);
	
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
int _lcd_cursor_addr_set(uint8_t x, uint8_t y)
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
	_lcd_regist();

	
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
	lcd_cmd_write(0x89);   //ram address control, CA increment first
//	lcd_cmd_write(0x8B);   //ram address control, PAGE increment first
	lcd_cmd_write(0xC0);   //LCD mapping control
//	lcd_cmd_write(0x04);   //MX=0,MY=1
	lcd_cmd_write(0x01);   //MY=0,MX=0,MSF=1
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
    uint8_t line, row, col, mask, pat;

    x = LCD_MIRROR_X(x); // 屏幕左右调换

    // 列扫描模式字体
    for(line = 0; line < lines; line++){
        
        for (col = 0; col < w; col++){
            pat = *pattern;

            if (inv){pat = ~pat;}

            for (row = 0; row < 8; row++){ // 8 rows = 1 line
                mask = 0x80 >> row;  
                if ((pat & mask) == 0){
                    _lcd_erase(x-col, y + row);
                }else{
                    _lcd_plot(x-col, y + row);
                }
            }
            pattern++;
        }   
		y += 8;
    }

    return 0;
}

int _lcd_char_disp_5x7(uint8_t x, uint8_t y, uint8_t c, uint8_t inv){
    uint8_t* pattern;
    
    if ( c > '~'){
        c = ' ';
    }
    
    pattern = &ascii_5x7[(c - ' ') * FONT_5x7_WIDTH];

    return _lcd_char_disp(x, y, pattern, FONT_5x7_WIDTH, FONT_5x7_HEIGHT, inv);
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
	return _lcd_str_disp_5x7(x, y, pCharStr, 0);
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
	return _lcd_char_disp(x, y, p_bmp, width, height, 0);
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
	return _lcd_str_disp_5x7(x, y, pCharStr, 1);
}

int _lcd_str_disp_5x7(unsigned char x,unsigned char y,unsigned char *pCharStr, uint8_t inv){
    do
	{
		IF_PTR_IS_NULL_RET_NULLPTR_ERR(pCharStr);
		
		_lcd_char_disp_5x7(x, y, *pCharStr, inv);
		x += FONT_5x7_WIDTH;
		if(x>=(LCD_W-1))													// 写满自动跳转下一行
		{
			x = 0;
			y += FONT_5x7_HEIGHT;
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
	return _lcd_char_disp(x, y, p_bmp, width, height, 1);
}


/***************************************************************************************************
 * @fn      lcd_clean
 *
 * @brief   LCD清屏
 * @param   null
 * @return  0 -- success
 ***************************************************************************************************/ 
void lcd_clean(void)
{
	memset(g_lcdTxBuff, 0, sizeof(g_lcdTxBuff));
}

inline uint8_t _lcd_is_point_outside(uint8_t x, uint8_t y){
    return (x>=LCD_W || y>=LCD_H);
}

int _lcd_plot(uint8_t x, uint8_t y){

    IF_CONDITION_TURE_RET_PARAM_ERR(_lcd_is_point_outside(x, y));

    uint8_t *p = GET_BUF_POINT_ADDR(x, y);

    *p = (y & 1) ? (*p | 0x0F) : (*p | 0xF0);

    return 0;
}

int _lcd_plot_x2(uint8_t x, uint8_t y){
    IF_CONDITION_TURE_RET_PARAM_ERR(_lcd_is_point_outside(x, y));

    uint8_t *p = GET_BUF_POINT_ADDR(x, y);

    *p = 0xFF;

    return 0;
}


int _lcd_erase(uint8_t x, uint8_t y){
    
    IF_CONDITION_TURE_RET_PARAM_ERR(_lcd_is_point_outside(x, y));
    
    uint8_t *p = GET_BUF_POINT_ADDR(x, y);

    *p = (y & 1) ? (*p & 0xF0) : (*p & 0x0F);

    return 0;
}

int _lcd_erase_x2(uint8_t x, uint8_t y){
    
    IF_CONDITION_TURE_RET_PARAM_ERR(_lcd_is_point_outside(x, y));
    
    uint8_t *p = GET_BUF_POINT_ADDR(x, y);

    *p = 0x00;

    return 0;
}

int _lcd_hline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen,
    int (*x2_fn)(uint8_t, uint8_t),
    int (*fn)(uint8_t, uint8_t)){
    
    IF_CONDITION_TURE_RET_PARAM_ERR(_lcd_is_point_outside(x, y));
    IF_CONDITION_TURE_RET_PARAM_ERR(x2_fn == NULL || fn == NULL);

    if (x + len > LCD_W){
        len = LCD_W - x;
    }

    if (y + pen > LCD_H){
        pen = LCD_H - y;
    }

    x = LCD_MIRROR_X(x);

    uint8_t i, j;
    uint8_t plot_x2 = pen>>1;
    
    for (i = 0; i < len; i++){
        for (j = 0; j < plot_x2; j++){
            x2_fn(x - i, y + j * 2);
        }

        if (pen != (plot_x2<<1)){
            fn(x - i, y + j * 2);
        }
    }

    return 0;
}



int lcd_hline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen){
    return _lcd_hline_disp(x, y, len, pen, _lcd_plot_x2, _lcd_plot);
}

int lcd_hline_erase(uint8_t x, uint8_t y, uint8_t len, uint8_t pen){
    return _lcd_hline_disp(x, y, len, pen, _lcd_erase_x2, _lcd_erase);
}


int lcd_vline_disp(uint8_t x, uint8_t y, uint8_t len, uint8_t pen){
    return lcd_hline_disp(x, y, pen, len);
}

int lcd_vline_erase(uint8_t x, uint8_t y, uint8_t len, uint8_t pen){
    return lcd_hline_erase(x, y, pen, len);
}


int lcd_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    return lcd_hline_disp(x, y, w, h);
}

int lcd_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
    return lcd_hline_erase(x, y, w, h);
}


//////////////end of file/////////////////////////////////////////////////////
