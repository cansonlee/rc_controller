#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "cmsis_os.h"



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



#endif
