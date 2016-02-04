#include "communicate.h"
#include "global.h"
#include "usarts.h"
#include "comm_protocol.h"

#define RADIO_ACK_LEN 32

// note: when read, need disable interrupt
typedef struct{
    uint8_t  is_armed;
    uint8_t  rssi;            // Receive signal strength indicator, 0: 0%, 100: 100%, 255: invalid/unknown.
    uint8_t  gps_fix_type;    // 0-1: no fix, 2: 2D fix, 3: 3D fix. Some applications will
    uint8_t  gps_num;         // Number of satellites visible. If unknown, set to 255
    int8_t   battery_remain;  // Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot estimate the remaining battery
    uint16_t throttle;        // Current throttle setting in integer percent, 0 to 100
    uint16_t battery_volt;    // Battery voltage, in millivolts (1 = 1 millivolt)
    int16_t  battery_current; // Battery current, in 10*milliamperes (1 = 10 milliampere), -1: autopilot does not measure the current
    int16_t  heading;         // Current heading in degrees, in compass units (0..360, 0=north)
    uint8_t* flight_mode;     // "stab" "acro" etc.
    float    roll;            // Roll angle (rad, -pi..+pi)
    float    pitch;           // Pitch angle (rad, -pi..+pi)
    float    alt;             // Current altitude (MSL), in meters
}mav_data_t;

static uint8_t* m_copterFlightModeStr[] = {
    "stab", "acro", "alth", "auto", "guid", "loit", "retl", "circ", 
    "posi", "land", "oflo", "drif", ""    , "sprt", "flip", "atun", "hybr"};
static uint8_t* m_planFlightModeStr[] = {
    "manu", "circ", "stab", "trng", "acro", "fbwa", "fbwb", "crui",
    "atun", "",     "auto", "retl", "loit", ""    , ""    , "guid", "init"};

static mav_data_t m_mav_data;

void comm_protocol_msg_parsed_hook(comm_message_t* msg);
void mav_data_decode(void* pBuf, mav_data_t* pstData);


void Task_comm(void const * argument)
{
	//接收发射板串口信息

	//解第一层协议


	//解mavlink协议


	//队列发送Task_disp显示

}

void comm_data_send(uint8_t id, void* buf, uint8_t len){

    uint8_t ret_len;
    comm_message_t msg;
    
    ret_len = comm_protocol_msg_pack(id, buf, len, &msg);
    if (ret_len > 0){
        // send
        // addr _COMM_MSG_ADDR(&msg)
        // len ret_len
    }
}

void comm_protocol_msg_parsed_hook(comm_message_t* msg){
    switch (msg->msgid){
        case COMM_MSG_RECEIVED_ACK:
            if (msg->len == RADIO_ACK_LEN){
                mav_data_decode(msg->payload, &m_mav_data);
            }
        break;
        default:
        break;
    }
}

void uarts_sport_irq_handler_cb_hook(uint8_t *msg, uint16_t len)
{
    uint8_t i;
    
    for (i = 0; i < len; i++){
        comm_protocol_parse(msg[i], comm_protocol_msg_parsed_hook);
    }
}

void mav_data_decode(void* pBuf, mav_data_t* pstData){
    
    uint8_t type;
    uint32_t custom_mode;
   
    pstData->is_armed = _TELEMETRY_MAV_ARMED_GET(pBuf);
   
    pstData->flight_mode = "";
    type = _TELEMETRY_MAV_TYPE_GET(pBuf);
    custom_mode = _TELEMETRY_MAV_CUSTOM_MODE_GET(pBuf);
    if (type == 2){ // ArduCopter MultiRotor or ArduCopter Heli
        if (custom_mode < sizeof(m_copterFlightModeStr) / sizeof(uint8_t*)){
            pstData->flight_mode = m_copterFlightModeStr[custom_mode];
        }
    }else if (type == 1){
        if (custom_mode < sizeof(m_planFlightModeStr) / sizeof(uint8_t*)){
            pstData->flight_mode = m_planFlightModeStr[custom_mode];
        }
    }

    pstData->throttle = _TELEMETRY_MAV_THROTTLE_GET(pBuf);                 
    pstData->rssi = _TELEMETRY_MAV_RSSI_GET(pBuf);                     
    pstData->gps_fix_type = _TELEMETRY_MAV_GPS_FIX_TYPE_GET(pBuf);             
    pstData->gps_num = _TELEMETRY_MAV_GPS_NUM_GET(pBuf);                  
    _TELEMETRY_MAV_BATTERY_VOLT_CPY(pBuf, &pstData->battery_volt);      
    _TELEMETRY_MAV_BATTERY_CURRENT_CPY(pBuf, &pstData->battery_current);
    pstData->battery_remain = _TELEMETRY_MAV_BATTERY_REMAIN_GET(pBuf);           
    _TELEMETRY_MAV_ATTITUDE_ROLL_CPY(pBuf, &pstData->roll);     
    _TELEMETRY_MAV_ATTITUDE_PITCH_CPY(pBuf, &pstData->pitch);   
    _TELEMETRY_MAV_HUD_HEADING_CPY(pBuf, &pstData->heading);    
    _TELEMETRY_MAV_HUD_ALT_CPY(pBuf, &pstData->alt);            
}

//////////////end of file/////////////////////////////////////////////////////
