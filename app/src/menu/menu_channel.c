
#include <stdio.h>

#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

#include "ADCs.h"

#include "ana_inputs.h"

uint16_t menu_page_channel_tbl_size_get(void);
void menu_page_channel_event_process
(
    uint32_t event,
    uint16_t panel_id
);
void _menu_page_channel_value_update(uint16_t panel_id, uint16_t val);


static uint8_t m_srv_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_srh_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_slv_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_slh_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_rlu_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_rld_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_rru_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_rrd_val_buf[5] = {'0', '0', '0', '0', 0};
static uint8_t m_vol_val_buf[5] = {'0', '0', '0', '0', 0};

UI_FRAME_PANEL_STRU g_page_channel_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 2"},

    {0,    16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SRV"},
    {32,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_srv_val_buf},
    {72,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SLV"},
    {104,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_slv_val_buf},
    {144,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RLU"},
    {176,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_rlu_val_buf},

    {0,    32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SRH"},
    {32,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_srh_val_buf},
    {72,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SLH"},
    {104,  32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_slh_val_buf},
    {144,  32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RLD"},
    {176,  32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_rld_val_buf},
    
    {0,    48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RRU"},
    {32,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_rru_val_buf},
    {72,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RRD"},
    {104,  48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_rrd_val_buf},
    {144,  48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "VOL"},
    {176,  48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_vol_val_buf},
};

uint16_t menu_page_channel_tbl_size_get(void){
    return sizeof(g_page_channel_tbl) / sizeof(g_page_channel_tbl[0]);
}

void menu_page_channel_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    panel_id = panel_id;
    event = event;

    ALL_STICK_INPUT_t input;
    printf("enter in @ %s, %s, L%d \r\n", __FILE__, __func__, __LINE__);
    switch (event)
    {
        case UI_FRAME_EVENT_DATA_UPDATE:
            printf("update channel adc data @ %s, %s, L%d \r\n", __FILE__, __func__, __LINE__);
                
            adc_all_in_val_get(&input);
            
            _menu_page_channel_value_update(2, input.adcs[STICK_RV]);
            _menu_page_channel_value_update(4, input.adcs[STICK_LV]);
            _menu_page_channel_value_update(6, input.adcs[ROTATE_LU]);
            _menu_page_channel_value_update(8, input.adcs[STICK_RH]);
            _menu_page_channel_value_update(10, input.adcs[STICK_LH]);
            _menu_page_channel_value_update(12, input.adcs[ROTATE_LD]);
            _menu_page_channel_value_update(14, input.adcs[ROTATE_RU]);
            _menu_page_channel_value_update(16, input.adcs[ROTATE_RD]);
            _menu_page_channel_value_update(18, input.adcs[MONITOR_VOL]);
            
            break;
        
        default:
            break;
    }
}

void _menu_page_channel_value_update(uint16_t panel_id, uint16_t val){
	menu_logic_sprintf_uint16(9999, val, 
        g_page_channel_tbl[panel_id].content);
}

