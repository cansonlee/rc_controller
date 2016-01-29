#include "keys_n_ADCs.h"
#include "delay_timer.h"

#include <stdlib.h>
#include <string.h>


/////////////////////全局变量定义///////////////////////////////////////
uint16_t	adcs_value[MODULE_NUMBER];						//各通道值缓存


/***************************************************************************************************
 * @fn      KEY_Registe
 *
 * @brief   KEY资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void KEY_Registe(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//MENU KEY
	RCC_AHB1PeriphResetCmd(KEY_MENU_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_MENU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_MENU_PORT, &GPIO_InitStructure);

	//PAGE KEY
	RCC_AHB1PeriphResetCmd(KEY_PAGE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_PAGE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_PAGE_PORT, &GPIO_InitStructure);

	//EXIT KEY
	RCC_AHB1PeriphResetCmd(KEY_EXIT_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_EXIT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_EXIT_PORT, &GPIO_InitStructure);

	//PLUS KEY
	RCC_AHB1PeriphResetCmd(KEY_PLUS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_PLUS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_PLUS_PORT, &GPIO_InitStructure);

	//MINUS KEY
	RCC_AHB1PeriphResetCmd(KEY_MINUS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_MINUS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_MINUS_PORT, &GPIO_InitStructure);

	//ENTER KEY
	RCC_AHB1PeriphResetCmd(KEY_ENTER_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_ENTER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_ENTER_PORT, &GPIO_InitStructure);

	//拨动开关
	//SA
	RCC_AHB1PeriphResetCmd(SLD_SA_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SA_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SA_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphResetCmd(SLD_SA_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SA_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SA_L_PORT, &GPIO_InitStructure);

	//SB
	RCC_AHB1PeriphResetCmd(SLD_SB_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SB_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SB_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphResetCmd(SLD_SB_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SB_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SB_L_PORT, &GPIO_InitStructure);

	//SC
	RCC_AHB1PeriphResetCmd(SLD_SC_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SC_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SC_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphResetCmd(SLD_SC_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SC_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SC_L_PORT, &GPIO_InitStructure);

	//SD
	RCC_AHB1PeriphResetCmd(SLD_SD_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SD_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SD_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphResetCmd(SLD_SD_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SD_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SD_L_PORT, &GPIO_InitStructure);

	//SE
	RCC_AHB1PeriphResetCmd(SLD_SE_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SE_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SE_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphResetCmd(SLD_SE_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SLD_SE_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SLD_SE_L_PORT, &GPIO_InitStructure);
}


/***************************************************************************************************
* @fn 	 STICK_n_RATATE_Registe
*
* @brief	 舵、拨轮及电压监测资源注册
* @param	 NULL
* @return  null
***************************************************************************************************/
void STICK_n_RATATE_Registe(void)
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
	ADC_InitStructure.ADC_NbrOfConversion = MODULE_NUMBER;
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
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adcs_value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = MODULE_NUMBER;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(SKYBORNE_ADC_DMA_STREAM, &DMA_InitStructure);	
	DMA_Cmd(SKYBORNE_ADC_DMA_STREAM, ENABLE);

	ADC_DMARequestAfterLastTransferCmd(SKYBORNE_ADC_DMA_STREAM, ENABLE);
	ADC_DMACmd(SKYBORNE_ADC_DMA_STREAM, ENABLE);
	DMA_ITConfig(SKYBORNE_ADC_DMA_STREAM, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(SKYBORNE_ADC_DMA_STREAM, SKYBORNE_ADC_DMA_TC_FLAG);
//	ADC_ITConfig(SKYBORNE_ADC, ADC_IT_EOC, ENABLE);
//	ADC_ClearITPendingBit(SKYBORNE_ADC, ADC_IT_EOC);	

	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = SKYBORNE_ADC_DMA_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SKYBORNE_ADC_NVIC_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_Cmd(SKYBORNE_ADC, ENABLE);
	ADC_SoftwareStartConv(SKYBORNE_ADC);
}


/***************************************************************************************************
* @fn 	 SKYBORNE_ADC_IRQHandlerCallback
*
* @brief	 SKYBORNE_ADC转换完中断回调函数
* @param	 NULL
* @return  NULL
***************************************************************************************************/	
extern void SKYBORNE_ADC_DMA_IRQHandlerCallbackHook(uint16_t *adcs_value, uint8_t len);
void SKYBORNE_ADC_DMA_IRQHandlerCallback(void)
{
	if(DMA_GetITStatus(SKYBORNE_ADC_DMA_CHANNEL, SKYBORNE_ADC_DMA_TC_FLAG) != RESET)
	{
		ADC_ClearFlag(SKYBORNE_ADC, ADC_FLAG_EOC | ADC_FLAG_STRT | ADC_FLAG_OVR);
		DMA_ClearITPendingBit(SKYBORNE_ADC_DMA_CHANNEL, SKYBORNE_ADC_DMA_TC_FLAG);
		DMA_Cmd(SKYBORNE_ADC_DMA_CHANNEL,DISABLE);		

		SKYBORNE_ADC_DMA_IRQHandlerCallbackHook(adcs_value, MODULE_NUMBER);

		//开启下一次转换和传送
		DMA_SetCurrDataCounter(SKYBORNE_ADC_DMA_CHANNEL, MODULE_NUMBER);
		DMA_Cmd(SKYBORNE_ADC_DMA_CHANNEL,ENABLE);
		ADC_SoftwareStartConv(SKYBORNE_ADC);
	}
}

/***************************************************************************************************
* @fn 	 keys_n_ADCs_init
*
* @brief   按键和ADC模块初始化函数
* @param	 NULL
* @return  NULL
***************************************************************************************************/
void keys_n_ADCs_init(void)
{
	KEY_Registe();
	STICK_n_RATATE_Registe();
}

/***************************************************************************************************
* @fn 	 KEY_Scan
*
* @brief	 KEY读取key值
* @param	 NULL
* @return  key_value
***************************************************************************************************/	
KEY_VALUE KEY_Scan(void)
{
	if(GPIO_ReadInputDataBit(KEY_MENU_PORT, KEY_MENU_PIN) == RESET)
		return KEY_MENU;
	if(GPIO_ReadInputDataBit(KEY_PAGE_PORT, KEY_PAGE_PIN) == RESET)
		return KEY_PAGE;
	if(GPIO_ReadInputDataBit(KEY_EXIT_PORT, KEY_EXIT_PIN) == RESET)
		return KEY_EXIT;
	if(GPIO_ReadInputDataBit(KEY_PLUS_PORT, KEY_PLUS_PIN) == RESET)
		return KEY_PLUS;
	if(GPIO_ReadInputDataBit(KEY_MINUS_PORT, KEY_MINUS_PIN) == RESET)
		return KEY_MINUS;
	if(GPIO_ReadInputDataBit(KEY_ENTER_PORT, KEY_ENTER_PIN) == RESET)
		return KEY_ENTER;

	return KEY_READY;
}


 /***************************************************************************************************
 * @fn	  KEY_Read
 *
 * @brief	  按键滤抖，>10ms运行一次
 * @param	  state -- 返回按键当前状态
 * @return	key_value
 ***************************************************************************************************/	 
KEY_VALUE KEY_Read(KEY_STATUS *state)
{
	KEY_VALUE			currentKeyValue;
	static KEY_VALUE 	lastKeyVaule = KEY_READY;
	static uint8_t		keyTimer = 0;
	

	currentKeyValue = KEY_Scan();
	if(currentKeyValue == KEY_READY)
	{
		keyTimer = 0;
		*state = KEY_RELEASED;
		return KEY_READY;
	}
	else
	{
		if((lastKeyVaule == KEY_READY) || (lastKeyVaule != currentKeyValue))
		{
			lastKeyVaule = currentKeyValue;
			keyTimer = 0;
			*state = KEY_PRESSED;
			return KEY_READY;
		}
		else
		{
			keyTimer++;
			if(keyTimer > 1)
			{
				*state = KEY_PRESSED;
				return currentKeyValue;
			}
			else
			{
				*state = KEY_HOLD;
				return currentKeyValue;
			}
		}
			
	}
	
}
 

 /***************************************************************************************************
 * @fn	  Misc_Slide_Switch_Read
 *
 * @brief	拨档开关组合读取
 * @param	NULL
 * @return	key value
 ***************************************************************************************************/	
MISC_SLIDE_SWITCH_VALUE Misc_Slide_Switch_Read(void)
{
	MISC_SLIDE_SWITCH_VALUE misc_slide_switch;

	//SA
	if(GPIO_ReadInputDataBit(SLD_SA_H_PORT, SLD_SA_H_PIN) == RESET)
		misc_slide_switch.SA = SLIDE_UP;
	else if(GPIO_ReadInputDataBit(SLD_SA_L_PORT, SLD_SA_L_PIN) == RESET)
		misc_slide_switch.SA = SLIDE_DOWN;
	else
		misc_slide_switch.SA = SLIDE_MID;

	//SB
	if(GPIO_ReadInputDataBit(SLD_SB_H_PORT, SLD_SB_H_PIN) == RESET)
		misc_slide_switch.SB= SLIDE_UP;
	else if(GPIO_ReadInputDataBit(SLD_SB_L_PORT, SLD_SB_L_PIN) == RESET)
		misc_slide_switch.SB = SLIDE_DOWN;
	else
		misc_slide_switch.SB = SLIDE_MID;

	//SC
	if(GPIO_ReadInputDataBit(SLD_SC_H_PORT, SLD_SC_H_PIN) == RESET)
		misc_slide_switch.SC = SLIDE_UP;
	else if(GPIO_ReadInputDataBit(SLD_SC_L_PORT, SLD_SC_L_PIN) == RESET)
		misc_slide_switch.SC = SLIDE_DOWN;
	else
		misc_slide_switch.SC = SLIDE_MID;

	//SD
	if(GPIO_ReadInputDataBit(SLD_SD_H_PORT, SLD_SD_H_PIN) == RESET)
		misc_slide_switch.SD = SLIDE_UP;
	else if(GPIO_ReadInputDataBit(SLD_SD_L_PORT, SLD_SD_L_PIN) == RESET)
		misc_slide_switch.SD = SLIDE_DOWN;
	else
		misc_slide_switch.SD = SLIDE_MID;

	//SE
	if(GPIO_ReadInputDataBit(SLD_SE_H_PORT, SLD_SE_H_PIN) == RESET)
		misc_slide_switch.SE = SLIDE_UP;
	else if(GPIO_ReadInputDataBit(SLD_SE_L_PORT, SLD_SE_L_PIN) == RESET)
		misc_slide_switch.SE = SLIDE_DOWN;
	else
		misc_slide_switch.SE = SLIDE_MID;

	return misc_slide_switch;
}


 //////////////end of file/////////////////////////////////////////////////////
