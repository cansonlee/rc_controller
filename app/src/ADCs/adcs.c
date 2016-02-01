/**
  ******************************************************************************
  * @file    adcs.c
  * @author  skyborne leon
  * @version V1.0.0
  * @date    04/11/2015
  * @brief   遥控器控制杆、拨档开关信号处理
  ******************************************************************************
  */ 

#include "cmsis_os.h"
#include "ADCs.h"
#include "usarts.h"


#include <stdlib.h>
#include <string.h>


/////////////////////全局变量定义///////////////////////////////////////
uint16_t					g_adc_calibr_val[ADC_MODULE_NUMBER-1];				//校正值缓存
uint16_t					g_ppm_channels[PPM_CH_NUM];
MISC_SW_VALUE				g_misc_sli_sw_value;

MIXER_LANDING_MODE_t		g_landingSetting;									//L1~L6模式组合键设置
uint8_t						g_SetStickMaxMinFlag;								//设置各舵量最值标志
Param_To_Store_t 			g_Param_To_Store;									//多项设置值保存
MSG_QUEUE_t		 			g_adc_msg;											//ADC各通道值缓存
uint16_t					g_gets_adcs_val[ADC_MODULE_NUMBER];					//其它任务获取各通道值缓存
MISC_SW_VALUE				g_lastSlideSwVal;									//其它任务获取各拨杆值缓存





///////////////////函数声明/////////////////////////////////
Param_To_Store_t ReadFlash(uint32_t addr);
void WriteFlash(uint32_t addr, uint32_t *pdata, uint32_t len);
uint16_t calibrate_calc(uint16_t curADC, uint16_t calibHighValue, uint16_t calibMidValue, uint16_t calibLowValue);



/***************************************************************************************************
* @fn      Task_ADCs
*
* @brief   遥控器控制杆、拨档开关信号处理任务
* @param   NULL
* @return  null
***************************************************************************************************/ 
void Task_ADCs(void const * argument)
{
	uint8_t			 i;
	
	argument = argument;

	//------从flash中读取校正值和失效保护参数-----------------
	g_Param_To_Store = (Param_To_Store_t) ReadFlash(ParaFlashAddr);
	if(g_Param_To_Store.valid != 1)			//参数无效,恢复默认值
	{
		for(i=0; i<(ADC_MODULE_NUMBER-1); i++)
		{
			g_Param_To_Store.ADCs_Calibrate_value[i].HighValue = 4095;
			g_Param_To_Store.ADCs_Calibrate_value[i].MidValue = 2048;
			g_Param_To_Store.ADCs_Calibrate_value[i].LowValue = 0;
		}
		memset(g_Param_To_Store.channel_value, 0, sizeof(g_Param_To_Store.channel_value));
		g_Param_To_Store.failsafeMode = FAILSAFE_MODE_RTL;
	}
	
	for(;;)
	{
		xQueueReceive(xSemaphore_ForADCs, &g_adc_msg, portMAX_DELAY);

		//参数计算
		for(i=0; i<(ADC_MODULE_NUMBER-1); i++)
		{
			if(i <= STICK_LV)
			{
				 calibrate_calc(g_adc_msg.msg[i], &g_adc_calibr_val[i], g_Param_To_Store.ADCs_Calibrate_value[i].HighValue, 
								g_Param_To_Store.ADCs_Calibrate_value[i].MidValue, g_Param_To_Store.ADCs_Calibrate_value[i].LowValue);
			}
			else
			{
				calibrate_calc(g_adc_msg.msg[i], &g_adc_calibr_val[i], g_Param_To_Store.ADCs_Calibrate_value[i].HighValue, 
							  (g_Param_To_Store.ADCs_Calibrate_value[i].MidValue - g_Param_To_Store.ADCs_Calibrate_value[i].LowValue)/2,
							   g_Param_To_Store.ADCs_Calibrate_value[i].LowValue);
			}
			
		}

		//拨档开关采集
		misc_slide_value = Misc_Slide_Switch_Read();

		//分配通道
		

		//发送Task_disp显示各拨杆值


		//发送发射板


		/***********接收队列处理*************/
		//Task_disp触发的舵及拨轮校正命令处理--按HighValue、MidValue、LowValue分别保存,读出


		//Task_disp触发的失效保护模式处理-- 当前值保护模式命令记录当前油门和模式值，其它命令直接保存， 存入flash

		osDelay(2);		
	}
}

/***************************************************************************************************
* @fn      SKYBORNE_ADC_DMA_IRQHandlerCallbackHook
*
* @brief   遥控器控制杆驱动程序回调函数
* @param   adcs_value -- ADC 值序列
*          len 长度
* @return  null
***************************************************************************************************/ 
void SKYBORNE_ADC_DMA_IRQHandlerCallbackHook(uint16_t *adcs_value, uint8_t len)
{
	MSG_QUEUE_t msg;

	msg.len = len;
	memcpy(msg.msg, adcs_value, len);
	//both ISR and Task
	osMessagePut(xQueue_ToADCs, &msg, 0);
}

/***************************************************************************************************
 * @fn      ReadFlash
 *
 * @brief   读相应地址的FLASH数据
 * @param   @arg addr -- 指定的数据地址
 *   
 * @return  返回读取到的word值
 ***************************************************************************************************/
Param_To_Store_t ReadFlash(uint32_t addr)
{
  Param_To_Store_t data;
  Param_To_Store_t *p = (Param_To_Store_t*)addr;
  data = *p;
  return data;	
}

/***************************************************************************************************
 * @fn      WriteFlash
 *
 * @brief   向相应地址的FLASH写入一个字的数据
 * @param   @arg addr -- 指定的数据起始地址
 *          @arg data -- 要写入的字数据
 *          @arg len  -- 要写入的字数据长度
 * @return  NULL
 ***************************************************************************************************/
void WriteFlash(uint32_t addr, uint32_t *pdata, uint32_t len)
{
	FLASH_Status FLASHStatus;
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
	              FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

	FLASHStatus = FLASH_EraseSector(ADDR_FLASH_SECTOR_3, VoltageRange_3);  

	while(len--)
	{
		if(FLASHStatus == FLASH_COMPLETE)
		{
	  		FLASHStatus = FLASH_ProgramWord(addr, *pdata);
		}
		addr += 4;
		pdata++;
	}
}

/***************************************************************************************************
 * @fn      calibrate_calc
 *
 * @brief   ADC采样数据线性校正
 * @param   curADC -- 当前采样值
 *          *pGet --  校正值返回
 *          calibHighValue  -- 第一校正点
 *			calibMidValue  --  第二校正点
 *			calibLowValue  --  第三校正点
 * @return  0 -- success
 *			-1 -- failure
 ***************************************************************************************************/

int adc_calib_calc(uint16_t curADC, uint16_t *pGet, uint16_t calibHighValue, uint16_t calibMidValue, uint16_t calibLowValue)
{
	int16_t x1,x2,x3,y1,y2,y3;

	x1 = 0;
	x2 = 2048;
	x3 = 4096;

	y1 = calibLowValue;
	y2 = calibMidValue;
	y3 = calibHighValue;


	*pGet = 
	
	return 0;
}


/***************************************************************************************************
 * @fn      adc_mixer_set
 *
 * @brief   设置飞控端L1~L6对应通道的拨档组合
 * @param   landingValue -- 指定的L1~L6通道的组合键及键值
 * @return  0 -- success
 ***************************************************************************************************/
int adc_mixer_set(MIXER_LANDING_MODE_t landingValue)
{
	portENTER_CRITICAL();
	g_landingSetting = landingValue;
	portEXIT_CRITICAL();

	return 0;
}

/***************************************************************************************************
 * @fn      adc_mixer_get
 *
 * @brief   取得飞控端L1~L6对应通道的拨档组合
 * @param   *p_landingValue -- 获取指定的L1~L6通道的组合键及键值
 * @return  0 -- success
 *			1 -- fail
 ***************************************************************************************************/
int adc_mixer_get(MIXER_LANDING_MODE_t *p_landingValue)
{	
	if(*p_landingValue == NULL)
	{
		return -1;
	}
	
	portENTER_CRITICAL();
	*p_landingValue = g_landingSetting;
	portEXIT_CRITICAL();

	return 0;
}

/***************************************************************************************************
 * @fn      adc_stick_cntr_val_set
 *
 * @brief   设置各舵及拨钮中值
 * @param   null
 * @return  0 -- success
 ***************************************************************************************************/
int adc_stick_cntr_val_set(void)
{
	uint8_t i;
	
	portENTER_CRITICAL();

	for(i=0; i<ADC_MODULE_NUMBER; i++)
	{
		g_Param_To_Store.ADCs_Calibrate_value[i].MidValue = g_adc_msg.msg[i];
	}	
	
	portEXIT_CRITICAL();

	return 0;
}

/***************************************************************************************************
 * @fn      adc_stick_most_val_set_start
 *
 * @brief   开始设置各舵及拨钮最大最小值
 * @param   null
 * @return  0 -- success
 ***************************************************************************************************/
int adc_stick_most_val_set_start(void)
{	
	portENTER_CRITICAL();

	g_SetStickMaxMinFlag = 1;	
	
	portEXIT_CRITICAL();

	return 0;
}

/***************************************************************************************************
 * @fn      adc_stick_most_val_set_end
 *
 * @brief   设置各舵及拨钮最大最小值结束
 * @param   null
 * @return  0 -- success
 ***************************************************************************************************/
int adc_stick_most_val_set_end(void)
{	
	portENTER_CRITICAL();

	g_SetStickMaxMinFlag = 0;	
	
	portEXIT_CRITICAL();

	return 0;
}


/***************************************************************************************************
 * @fn      get_AllInputsValue
 *
 * @brief   获取所有舵、拨钮及拨档开关的校正后值
 * @param   null
 * @return  0 -- success
 *			1 -- fail
 ***************************************************************************************************/
int adc_all_in_val_get(ALL_STICK_INPUT_t *stickValue)
{
	if(stickValue == NULL)
	{
		return -1;
	}

	portENTER_CRITICAL();
	
	memcpy(*stickValue->adcs,  g_gets_adcs_val, ADC_MODULE_NUMBER);
			
	*stickValue.SLSW = g_lastSlideSwVal;
	
	portEXIT_CRITICAL();

	return 0;		
}


//////////////end of file/////////////////////////////////////////////////////
