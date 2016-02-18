
#include <stdio.h>

#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

#include "ADCs.h"

#include "ana_inputs.h"

uint16_t menu_page_channel_sw_tbl_size_get(void);
void menu_page_channel_sw_event_process
(
    uint32_t event,
    uint16_t panel_id
);
void _menu_page_channel_sw_value_update(uint16_t panel_id, uint16_t val);

static uint8_t m_sa_val_buf[2] = {'0', 0};
static uint8_t m_sb_val_buf[2] = {'0', 0};
static uint8_t m_sc_val_buf[2] = {'0', 0};
static uint8_t m_sd_val_buf[2] = {'0', 0};
static uint8_t m_se_val_buf[2] = {'0', 0};

UI_FRAME_PANEL_STRU g_page_channel_sw_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 3"},

    {0,    16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SA"},
    {24,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_sa_val_buf},
    {72,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SB"},
    {96,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_sb_val_buf},
    {144,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SC"},
    {168,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_sc_val_buf},

    {0,    32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SD"},
    {24,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_sd_val_buf},
    {72,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "SE"},
    {96,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_se_val_buf},
};

uint16_t menu_page_channel_sw_tbl_size_get(void){
    return sizeof(g_page_channel_sw_tbl) / sizeof(g_page_channel_sw_tbl[0]);
}

void menu_page_channel_sw_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    panel_id = panel_id;
    event = event;

    ALL_STICK_INPUT_t input;
    
    switch (event)
    {
        case UI_FRAME_EVENT_DATA_UPDATE:
            printf("update channel sw data \r\n");
                
            adc_all_in_val_get(&input);

            _menu_page_channel_sw_value_update(2, input.SW.SWS.SA);
            _menu_page_channel_sw_value_update(4, input.SW.SWS.SB);
            _menu_page_channel_sw_value_update(6, input.SW.SWS.SC);
            _menu_page_channel_sw_value_update(8, input.SW.SWS.SD);
            _menu_page_channel_sw_value_update(10, input.SW.SWS.SE);
            
            break;
        
        default:
            break;
    }
}

void _menu_page_channel_sw_value_update(uint16_t panel_id, uint16_t val){
    menu_logic_sprintf_uint16(4, val, g_page_channel_sw_tbl[panel_id].content);
}

