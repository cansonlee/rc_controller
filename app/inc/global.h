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


typedef enum mixer_logic_e
{
    NONE,
    AND,
    OR
}MIXER_LOGIC_ENUM;

typedef struct mixer_chan_mode_s
{
	uint16_t					switchs;	//combination of SLSW_SA_MASK to SLSW_SE_MASK
	MIXER_LOGIC_ENUM			logic;		//AND / OR
	MISC_SW_VALUE		        value;	
}MIXER_CHANNEL_MODE_t;

typedef struct mixer_l_chan_s
{
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L1;      //PWM 0~1230        middle-615
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L2;      //PWM 1231~1360     middle-1295
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L3;      //PWM 1361~1490     middle-1425
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L4;      //PWM 1491~1620     middle-1555
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L5;      //PWM 1621~1749     middle-1685
	MIXER_CHANNEL_MODE_t	Mixer_Mode_L6;      //PWM 1750+         middle-1815
}MIXER_LANDING_MODE_t;

typedef enum flight_mode_val_e
{
    FLIGHT_MODE_NONE,
    FLIGHT_MODE_L1      = 1000,
    FLIGHT_MODE_L2      = 1295,
    FLIGHT_MODE_L3      = 1425,
    FLIGHT_MODE_L4      = 1555,
    FLIGHT_MODE_L5      = 1685,
    FLIGHT_MODE_L6      = 1815
}FLIGHT_MODE_VAL_ENUM;


#endif
