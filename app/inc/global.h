#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "cmsis_os.h"
#include "keys.h"
#include "switches.h"
#include "ana_inputs.h"


extern osThreadId 		Task_commHandle;
extern osThreadId 		Task_ADCsHandle;
extern osThreadId 		Task_dispHandle;

extern osMessageQId 	xQueue_ToComm;
extern osMessageQId 	xQueue_ToADCs;
extern osMessageQId 	xQueue_ToDisp;

extern osSemaphoreId	xSemaphore_ForComm;
extern osSemaphoreId	xSemaphore_ForADCs;


typedef enum
{
	FAILSAFE_MODE_NONE,
	FAILSAFE_MODE_RTL,
	FAILSAFE_MODE_HOVER,
	FAILSAFE_MODE_LAND,
	FAILSAFE_MODE_CURRENT_STATUS
}FAILSAFE_MODE_t;			


#define MAX_MSG_BUF_LEN			100
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	uint8_t msg[MAX_MSG_BUF_LEN];
}MSG_QUEUE_t;


typedef struct
{
	uint16_t					switchs;	//combination of SLSW_SA_MASK to SLSW_SE_MASK
	uint16_t					logic;		//AND / OR
	MISC_SW_VALUE		        value;	
}MIXER_CHANNEL_MODE_t;

typedef struct
{
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L1;
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L2;
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L3;
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L4;
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L5;
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L6;
}MIXER_LANDING_MODE_t;

#endif
