#ifndef __COMMUNICAT_H
#define __COMMUNICAT_H



#include "global.h"


#define _TELEMETRY_MAV_DATA_GET(pBuf, offset)             (*((uint8_t*)(pBuf) + (offset)))
#define _TELEMETRY_MAV_ARMED_GET(pBuf)                    (_TELEMETRY_MAV_DATA_GET(pBuf, 0) & ((uint8_t)1<<6))
#define _TELEMETRY_MAV_TYPE_GET(pBuf)                     (_TELEMETRY_MAV_DATA_GET(pBuf, 0) & 0x7F)
#define _TELEMETRY_MAV_CUSTOM_MODE_GET(pBuf)              (uint32_t)_TELEMETRY_MAV_DATA_GET(pBuf, 1)
#define _TELEMETRY_MAV_THROTTLE_GET(pBuf)                 (uint16_t)_TELEMETRY_MAV_DATA_GET(pBuf, 2)
#define _TELEMETRY_MAV_RSSI_GET(pBuf)                     _TELEMETRY_MAV_DATA_GET(pBuf, 3)
#define _TELEMETRY_MAV_GPS_FIX_TYPE_GET(pBuf)             _TELEMETRY_MAV_DATA_GET(pBuf, 4)
#define _TELEMETRY_MAV_GPS_NUM_GET(pBuf)                  _TELEMETRY_MAV_DATA_GET(pBuf, 5)
#define _TELEMETRY_MAV_BATTERY_VOLT_CPY(pBuf, pVolt)       memcpy((pVolt), ((uint8_t*)(pBuf) + 6), sizeof(uint16_t))
#define _TELEMETRY_MAV_BATTERY_CURRENT_CPY(pBuf, pCurrent) memcpy((pCurrent), ((uint8_t*)(pBuf) + 8), sizeof(int16_t))
#define _TELEMETRY_MAV_BATTERY_REMAIN_GET(pBuf)            _TELEMETRY_MAV_DATA_GET(pBuf, 10)
#define _TELEMETRY_MAV_ATTITUDE_ROLL_CPY(pBuf, pRoll)      memcpy((pRoll), ((uint8_t*)(pBuf) + 11), sizeof(float))
#define _TELEMETRY_MAV_ATTITUDE_PITCH_CPY(pBuf, pPitch)    memcpy((pPitch), ((uint8_t*)(pBuf) + 15), sizeof(float))
#define _TELEMETRY_MAV_HUD_HEADING_CPY(pBuf, pHeading)     memcpy((pHeading), ((uint8_t*)(pBuf) + 19), sizeof(int16_t))
#define _TELEMETRY_MAV_HUD_ALT_CPY(pBuf, pAlt)             memcpy((pAlt), ((uint8_t*)(pBuf) + 21), sizeof(float))






void Task_comm(void const * argument);



#endif
