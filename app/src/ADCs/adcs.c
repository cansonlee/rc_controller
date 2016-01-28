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
uint16_t					gets_adcs_value[MODULE_NUMBER];						//各通道值缓存
uint16_t					adcs_value_buf[MODULE_NUMBER];						//各通道值第二缓存
uint16_t					adcs_calibrate_value[MODULE_NUMBER-1];				//校正值缓存
uint16_t					ppm_channels[PPM_CH_NUM];
MISC_SLIDE_SWITCH_VALUE		misc_slide_value;


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
	Param_To_Store_t Param_To_Store;
	uint8_t			 i;
	
	argument = argument;

	//------从flash中读取校正值和失效保护参数-----------------
	Param_To_Store = (Param_To_Store_t) ReadFlash(ParaFlashAddr);
	if(Param_To_Store.valid != 1)			//参数无效,恢复默认值
	{
		for(i=0; i<(MODULE_NUMBER-1); i++)
		{
			Param_To_Store.ADCs_Calibrate_value[i].HighValue = 4095;
			Param_To_Store.ADCs_Calibrate_value[i].MidValue = 2048;
			Param_To_Store.ADCs_Calibrate_value[i].LowValue = 0;
		}
		memset(Param_To_Store.channel_value, 0, sizeof(Param_To_Store.channel_value));
		Param_To_Store.failsafeMode = FAILSAFE_MODE_RTL;
	}
	
	for(;;)
	{
		osSemaphoreWait(xSemaphore_ForADCs, osWaitForever);

		memcpy(adcs_value_buf, gets_adcs_value, MODULE_NUMBER);

		//参数计算
		for(i=0; i<(MODULE_NUMBER-1); i++)
		{
			if(i <= STICK_LV)
			{
				adcs_calibrate_value[i] = calibrate_calc(adcs_value_buf[i], Param_To_Store.ADCs_Calibrate_value[i].HighValue, 
													 Param_To_Store.ADCs_Calibrate_value[i].MidValue, Param_To_Store.ADCs_Calibrate_value[i].LowValue);
			}
			else
			{
				adcs_calibrate_value[i] = calibrate_calc(adcs_value_buf[i], Param_To_Store.ADCs_Calibrate_value[i].HighValue, 
													 (Param_To_Store.ADCs_Calibrate_value[i].MidValue - Param_To_Store.ADCs_Calibrate_value[i].LowValue)/2,
													 Param_To_Store.ADCs_Calibrate_value[i].LowValue);
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
	memcpy(gets_adcs_value, adcs_value, len);
	//both ISR and Task
	osSemaphoreRelease(xSemaphore_ForADCs);
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
 * @brief   向相应地址的FLASH写入一个字的数据
 * @param   @arg addr -- 指定的数据起始地址
 *          @arg data -- 要写入的字数据
 *          @arg len  -- 要写入的字数据长度
 * @return  NULL
 ***************************************************************************************************/

uint16_t calibrate_calc(uint16_t curADC, uint16_t calibHighValue, uint16_t calibMidValue, uint16_t calibLowValue)
{
	return 0;
}
	

//////////////end of file/////////////////////////////////////////////////////
