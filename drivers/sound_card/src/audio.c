#include "audio.h"
#include "eeprom.h"

/////////////////////全局变量定义///////////////////////////////////////
uint16_t music_buf[AUDIO_BUFF_MAX_LEN];
uint8_t	 AudioWriteBusyFlag;					//Audio DMA写繁忙标志


/***************************************************************************************************
* @fn      Audio_Registe
*
* @brief   声卡资源注册
* @param   NULL
* @return  null
***************************************************************************************************/ 
void Audio_Registe(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	DAC_InitTypeDef				DAC_InitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
#if !defined(EEPROM)
	I2C_InitTypeDef 			I2C_InitStructure;
#endif

	//DAC pin
	RCC_AHB1PeriphResetCmd(AUDIO_DAC_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = AUDIO_DAC_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(AUDIO_DAC_PORT, &GPIO_InitStructure);

	//DAC
	RCC_APB1PeriphResetCmd(AUDIO_DAC_CLK, ENABLE);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(AUDIO_DAC_CHANNEL, &DAC_InitStructure);

	DAC_SetChannel1Data(DAC_Align_12b_R, 2010);

	//DMA init
	RCC_AHB1PeriphResetCmd(AUDIO_DAC_DMA_CLK, ENABLE);
	DMA_DeInit(AUDIO_DAC_DMA_STERAM);	
	DMA_InitStructure.DMA_Channel = AUDIO_DAC_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&AUDIO_DAC->DHR12R1);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)music_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = AUDIO_BUFF_MAX_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(AUDIO_DAC_DMA_STERAM, &DMA_InitStructure);	
	DMA_Cmd(AUDIO_DAC_DMA_STERAM, ENABLE);

	DAC_DMACmd(AUDIO_DAC_CHANNEL, ENABLE);

	DMA_ITConfig(AUDIO_DAC_DMA_STERAM, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(AUDIO_DAC_DMA_STERAM, AUDIO_DAC_DMA_TC_FLAG);

	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = AUDIO_DAC_DMA_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = AUDIO_DAC_NVIC_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DAC_Cmd(AUDIO_DAC_CHANNEL, ENABLE);

	//TIM6 init
	RCC_APB1PeriphResetCmd(AUDIO_DAC_TIMER_CLK, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;									//max speed
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = (SystemCoreClock/2) / 100000 - 1;		// 10 uS, 100 kHz
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(AUDIO_DAC_TIMER, &TIM_TimeBaseInitStructure);

	//TIM6  CR2 config, no st std lib to use
	AUDIO_DAC_TIMER->CR2 = 0;
  	AUDIO_DAC_TIMER->CR2 = 0x20 ;
	TIM_Cmd(AUDIO_DAC_TIMER, ENABLE);
	
#if !defined(EEPROM)
	RCC_AHB1PeriphResetCmd(IIC_EEPROM_SCL_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_EEPROM_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_EEPROM_SCL_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(IIC_EEPROM_SCL_PORT, IIC_EEPROM_SCL_PIN_SOURCE, IIC_EEPROM_SCL_GPIO_AF);

	RCC_AHB1PeriphResetCmd(IIC_EEPROM_SDA_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_EEPROM_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_EEPROM_SDA_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(IIC_EEPROM_SDA_PORT, IIC_EEPROM_SDA_PIN_SOURCE, IIC_EEPROM_SDA_GPIO_AF);

	RCC_APB1PeriphResetCmd(IIC_EEPROM_I2C_CLK, ENABLE);
	I2C_InitStructure.I2C_ClockSpeed = IIC_EEPROM_SPEED;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(IIC_EEPROM_I2C, &I2C_InitStructure);
	I2C_Cmd(IIC_EEPROM_I2C, ENABLE);

#endif

}

/***************************************************************************************************
* @fn      Audio_SetSampleRate
*
* @brief   声卡采样率设置
* @param   frequency -- 采样率值
* @return  null
***************************************************************************************************/ 
void Audio_SetSampleRate(uint32_t frequency)
{
  uint32_t timer = (SystemCoreClock/2) / frequency - 1;         // MCK/8 and 100 000 Hz

  TIM6->CR1 &= ~TIM_CR1_CEN ;
  TIM6->CNT = 0 ;
  TIM6->ARR = (2>=timer) ? 2 : ((timer<=65535) ? timer : 65535);
  TIM6->CR1 |= TIM_CR1_CEN ;
}


/***************************************************************************************************
* @fn      Audio_init
*
* @brief   声卡资源初始化
* @param   NULL
* @return  null
***************************************************************************************************/ 
void Audio_init(void)
{
	Audio_Registe();
}


/***************************************************************************************************
* @fn      Audio_Start
*
* @brief   声卡发声
* @param   str -- 声音数据
*		   len -- 长度
* @return  0 -- 失败
		   1 -- 成功
***************************************************************************************************/ 
uint8_t Audio_Start(uint16_t *str, uint16_t len)
{
	if(len > AUDIO_BUFF_MAX_LEN)
		return 0;

	if(AudioWriteBusyFlag)
		return 0;

	AudioWriteBusyFlag = 1;

	memcpy(music_buf, str, len);
	DMA_SetCurrDataCounter(AUDIO_DAC_DMA_STERAM, len);
	DMA_Cmd(AUDIO_DAC_DMA_STERAM,ENABLE);
	return 1;
}

/***************************************************************************************************
* @fn      Audio_Stop
*
* @brief   声卡停止
* @param   NULL
* @return  null
***************************************************************************************************/ 
void Audio_Stop(void)
{
	AUDIO_DAC_DMA_STERAM->CR &= ~DMA_SxCR_CIRC ;
}

/***************************************************************************************************
 * @fn      AUDIO_DAC_DMA_IRQHandlerCallback
 *
 * @brief   AUDIO DAC DMA写入中断回调函数，完成关闭DMA
 * @param   NULL
 * @return  NULL
 ***************************************************************************************************/  
void AUDIO_DAC_DMA_IRQHandlerCallback(void)
{
	  if(DMA_GetITStatus(AUDIO_DAC_DMA_STERAM, AUDIO_DAC_DMA_TC_FLAG) != RESET)
	  {
	    DMA_ClearITPendingBit(AUDIO_DAC_DMA_STERAM, AUDIO_DAC_DMA_TC_FLAG);
	    DMA_Cmd(AUDIO_DAC_DMA_STERAM,DISABLE);
		AudioWriteBusyFlag = 0;
	  }
}

/***************************************************************************************************
 * @fn      I2C_CheckEventTimeout
 *
 * @brief   I2C响应超时检查
 * @param   event -- I2C_EVENT
 * @return  NULL
 ***************************************************************************************************/ 
#if !defined(EEPROM)

uint8_t I2C_CheckEventTimeout(uint32_t event)
{
  uint32_t timeout = I2C_TIMEOUT_MAX;
  while (!I2C_CheckEvent(I2C, event)) {
    if ((timeout--) == 0) return 0;
  }
  return 1;
}

#else
extern uint8_t I2C_CheckEventTimeout(uint32_t event);

#endif

/***************************************************************************************************
* @fn      Audio_SetVolume
*
* @brief   设置音量大小
* @param   volume -- 音量值
* @return  null
***************************************************************************************************/ 

void Audio_SetVolume(uint8_t volume)
{
  if (volume > VOLUME_LEVEL_MAX) {
    volume = VOLUME_LEVEL_MAX;
  }

  while (I2C_CheckEvent(IIC_EEPROM_I2C, I2C_FLAG_BUSY));

  I2C_GenerateSTART(IIC_EEPROM_I2C, ENABLE);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_MODE_SELECT))
    return;

  I2C_Send7bitAddress(IIC_EEPROM_I2C, I2C_ADDRESS_CAT5137, I2C_Direction_Transmitter);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    return;

  I2C_SendData(IIC_EEPROM_I2C, 0);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    return;
  I2C_SendData(IIC_EEPROM_I2C, volume);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    return;

  I2C_GenerateSTOP(IIC_EEPROM_I2C, ENABLE);
}



//////////////end of file/////////////////////////////////////////////////////
