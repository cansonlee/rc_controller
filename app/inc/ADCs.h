#ifndef __ADCS_H
#define __ADCS_H

#include "keys_n_ADCs.h"
#include "global.h"
#include "stm32f2xx_flash.h"

#define     ParaFlashAddr                     0x0800C000        //定义参数存放地点
#define 	ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */



typedef struct
{
	uint16_t HighValue;
	uint16_t MidValue;
	uint16_t LowValue;
}ADC_Value_Calibration_t;

typedef struct
{
	ADC_Value_Calibration_t  ADCs_Calibrate_value[MODULE_NUMBER-1];
	uint16_t 				 failsafeMode;
	uint16_t				 channel_value[5];
	uint16_t				 valid;
	uint16_t				 align;
}Param_To_Store_t;


typedef enum
{
	CH1_PITCH,
	CH2_ROLL,
	CH3_THROTTLE,
	CH4_YAW,
	CH5_MIX,
	CH6_LANDING_MODE,
	CH7_CR_PITCH,
	CH8_CR_ROLL,
	CH9_CR_YAW,
	CH10_CR_BACKUP,
	CH11_PHOTO,
	CH12_CAM_VIDEO,
	CH13_TRAING,
	CH14_FAILSAFE_MODE,
	CH15_FAILSAFE_THROTTLE,
	CH16_FAILSAFE_MIX,	
	PPM_CH_NUM
}PPM_CH_FORMAT_t;


void Task_ADCs(void const * argument);


#endif
