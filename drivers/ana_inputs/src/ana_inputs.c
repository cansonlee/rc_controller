#include "ana_inputs.h"

#include <stdlib.h>
#include <string.h>


/////////////////////全局变量定义///////////////////////////////////////
uint16_t	g_adcs_value[ADC_MODULE_NUMBER];						//各通道值缓存



/***************************************************************************************************
* @fn 	 ana_inputs_regist
*
* @brief	 遥杆、拨轮及电压监测资源注册
* @param	 NULL
* @return    null
***************************************************************************************************/
void ana_inputs_regist(void)
{
	 GPIO_InitTypeDef 		GPIO_InitStructure;
	 ADC_InitTypeDef  		ADC_InitStructure;
	 ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	 DMA_InitTypeDef  		DMA_InitStructure;
	 NVIC_InitTypeDef 		NVIC_InitStructure;

	//GPIO init
	//STICK RV
	RCC_AHB1PeriphClockCmd(STICK_RV_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = STICK_RV_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(STICK_RV_PORT, &GPIO_InitStructure);

	//STICK RH
	RCC_AHB1PeriphClockCmd(STICK_RH_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = STICK_RH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(STICK_RH_PORT, &GPIO_InitStructure);

	//STICK LH
	RCC_AHB1PeriphClockCmd(STICK_LH_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = STICK_LH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(STICK_LH_PORT, &GPIO_InitStructure);

	//STICK LV
	RCC_AHB1PeriphClockCmd(STICK_LV_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = STICK_LV_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(STICK_LV_PORT, &GPIO_InitStructure);

	//ROTATE_LU
	RCC_AHB1PeriphClockCmd(ROTATE_LU_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROTATE_LU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(ROTATE_LU_PORT, &GPIO_InitStructure);

	//ROTATE_LD
	RCC_AHB1PeriphClockCmd(ROTATE_LD_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROTATE_LD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(ROTATE_LD_PORT, &GPIO_InitStructure);

	//ROTATE_RU
	RCC_AHB1PeriphClockCmd(ROTATE_RU_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROTATE_RU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(ROTATE_RU_PORT, &GPIO_InitStructure);

	//ROTATE_RD
	RCC_AHB1PeriphClockCmd(ROTATE_RD_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROTATE_RD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(ROTATE_RD_PORT, &GPIO_InitStructure);

	//MONITOR_VOL
	RCC_AHB1PeriphClockCmd(MONITOR_VOL_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = MONITOR_VOL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(MONITOR_VOL_PORT, &GPIO_InitStructure);

	//--------------ADC init---------------------------------
	RCC_APB2PeriphClockCmd(SKYBORNE_ADC_CLK, ENABLE);
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = ADC_MODULE_NUMBER;
	ADC_Init(SKYBORNE_ADC, &ADC_InitStructure);

	//采样排序
	ADC_RegularChannelConfig(SKYBORNE_ADC, STICK_RV_ADC_CHANNEL, STICK_RV+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, STICK_RH_ADC_CHANNEL, STICK_RH+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, STICK_LH_ADC_CHANNEL, STICK_LH+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, STICK_LV_ADC_CHANNEL, STICK_LV+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, ROTATE_LU_ADC_CHANNEL, ROTATE_LU+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, ROTATE_LD_ADC_CHANNEL, ROTATE_LD+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, ROTATE_RU_ADC_CHANNEL, ROTATE_RU+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, ROTATE_RD_ADC_CHANNEL, ROTATE_RD+1, ADC_SampleTime_28Cycles);
	ADC_RegularChannelConfig(SKYBORNE_ADC, MONITOR_VOL_ADC_CHANNEL, MONITOR_VOL+1, ADC_SampleTime_28Cycles);

	//DMA init
	RCC_AHB1PeriphClockCmd(SKYBORNE_ADC_DMA_CLK, ENABLE);
	DMA_DeInit(SKYBORNE_ADC_DMA_STREAM);	
	DMA_InitStructure.DMA_Channel = SKYBORNE_ADC_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SKYBORNE_ADC->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_adcs_value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADC_MODULE_NUMBER;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(SKYBORNE_ADC_DMA_STREAM, &DMA_InitStructure);				

	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = SKYBORNE_ADC_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SKYBORNE_ADC_NVIC_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_Cmd(SKYBORNE_ADC, ENABLE);


	printf("SKYBORNE_ADC->SQR3:%#x, SQR2:%#x @ %s, %s, %d\r\n",SKYBORNE_ADC->SQR3, SKYBORNE_ADC->SQR2, __FILE__, __func__, __LINE__);
//	printf("SKYBORNE_ADC_DMA_STREAM->CR:%#x, NDTR:%#x, HISR:%#x, LISR:%#x , FCR:%#x @ %s, %s, %d\r\n",DMA2_Stream0->CR, DMA2_Stream0->NDTR, DMA2->HISR, DMA2->LISR, DMA2_Stream0->FCR, __FILE__, __func__, __LINE__);
}


/***************************************************************************************************
* @fn 	 ana_inputs_init
*
* @brief   摇杆、拨轮及电压监测初始化函数
* @param   NULL
* @return  NULL
***************************************************************************************************/
void ana_inputs_init(void)
{
	ana_inputs_regist();
}

/***************************************************************************************************
* @fn 	 ana_inputs_sample_start
*
* @brief   摇杆、拨轮及电压监测采样开始
* @param   NULL
* @return  NULL
***************************************************************************************************/
void ana_inputs_sample_start(void)
{
	DMA_Cmd(SKYBORNE_ADC_DMA_STREAM,DISABLE);
	ADC_ClearFlag(SKYBORNE_ADC, ADC_FLAG_EOC | ADC_FLAG_STRT | ADC_FLAG_OVR);
	DMA_ClearITPendingBit(SKYBORNE_ADC_DMA_STREAM, SKYBORNE_ADC_DMA_TC_FLAG);

	DMA_SetCurrDataCounter(SKYBORNE_ADC_DMA_STREAM, ADC_MODULE_NUMBER);
	ADC_DMARequestAfterLastTransferCmd(SKYBORNE_ADC, ENABLE);
	ADC_DMACmd(SKYBORNE_ADC, ENABLE);
	DMA_ITConfig(SKYBORNE_ADC_DMA_STREAM, DMA_IT_TC, ENABLE);	
	
	DMA_Cmd(SKYBORNE_ADC_DMA_STREAM, ENABLE);	
	ADC_SoftwareStartConv(SKYBORNE_ADC);
}

/***************************************************************************************************
* @fn 	 ana_inputs_adc_dma_irq_handler_callback
*
* @brief	 摇杆、拨轮及电压监测转换完中断回调函数
* @param	 NULL
* @return  NULL
***************************************************************************************************/	
extern void ana_inputs_adc_dma_irq_handler_cb_hook(uint16_t *adcs_value, uint8_t len);
void ana_inputs_adc_dma_irq_handler_callback(void)	
{
	uint16_t adc_buf[ADC_MODULE_NUMBER];

    printf("go into adc dma interrupt!at %d\r\n", __LINE__);
	if(DMA_GetITStatus(SKYBORNE_ADC_DMA_STREAM, SKYBORNE_ADC_DMA_TC_FLAG) != RESET)
	{
		DMA_Cmd(SKYBORNE_ADC_DMA_STREAM,DISABLE);
		DMA_ClearITPendingBit(SKYBORNE_ADC_DMA_STREAM, SKYBORNE_ADC_DMA_TC_FLAG);
		ADC_ClearFlag(SKYBORNE_ADC, ADC_FLAG_EOC | ADC_FLAG_STRT | ADC_FLAG_OVR);	

		printf("RV val=%d at %d\r\n",g_adcs_value[0], __LINE__);
		ana_inputs_adc_dma_irq_handler_cb_hook(g_adcs_value, ADC_MODULE_NUMBER);	
	
#if 0
		memcpy(adc_buf, g_adcs_value, ADC_MODULE_NUMBER);
        printf("RV val=%d at %d\r\n",g_adcs_value[0], __LINE__);
		
		//开启下一次转换和传送
		DMA_SetCurrDataCounter(SKYBORNE_ADC_DMA_STREAM, ADC_MODULE_NUMBER);

		ADC_DMARequestAfterLastTransferCmd(SKYBORNE_ADC, ENABLE);
		ADC_DMACmd(SKYBORNE_ADC, ENABLE);
		DMA_ITConfig(SKYBORNE_ADC_DMA_STREAM, DMA_IT_TC, ENABLE);
		
		DMA_Cmd(SKYBORNE_ADC_DMA_STREAM,ENABLE);
		ADC_SoftwareStartConv(SKYBORNE_ADC);
#endif					
	}
}



