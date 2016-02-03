#ifndef __ADCS_H
#define __ADCS_H


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
	ADC_Value_Calibration_t  ADCs_Calibrate_value[ADC_MODULE_NUMBER-1];
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
	CH5_MIX,					//飞行模式
	CH6_LANDING_MODE,			//降落/RTL模式
	CH7_CR_PITCH,				//云台pitch
	CH8_CR_ROLL,				//云台ROLL
	CH9_CR_YAW,					//云台YAW
	CH10_CR_BACKUP,				//云台backup
	CH11_PHOTO,					//像机/录相启停
	CH12_CAM_VIDEO,				//cam/vedio切换
	CH13_TRAING,				//训练--不用占通道
	CH14_FAILSAFE_MODE,			//失效模式  RTL  悬停  landing  预置位置
	CH15_FAILSAFE_THROTTLE,		//预置位置--油门
	CH16_FAILSAFE_MIX,			//各保护模式对应的5通道通道值  -- 走mavlink, 不用了
	PPM_CH_NUM
}PPM_CH_FORMAT_t;


typedef struct
{
	uint16_t					adcs[ADC_MODULE_NUMBER];		//包含电压值
	MISC_SW_VALUE		        SW;						        //各拨档开关值	
}ALL_STICK_INPUT_t;

void Task_ADCs(void const * argument);



uint8_t set_mixer(MIXER_LANDING_MODE_t landingValue);

MIXER_LANDING_MODE_t get_mixer(void);

uint8_t set_stickCenterValue(void);

uint8_t set_stickMaxMinStart(void);

uint8_t set_stickMaxMinEnd(void);

ALL_STICK_INPUT_t get_AllInputsValue(void);


#endif
