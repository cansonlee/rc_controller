
#include "cmsis_os.h"

#include <stdio.h>

#include "ui_frame.h"
#include "menu_logic.h"
#include "communicate.h"

#include "ADCs.h"
#include "lcd.h"



uint16_t menu_page_index_tbl_size_get(void);
void menu_page_index_event_process
(
    uint32_t event,
    uint16_t panel_id
);
void _menu_page_index_value_update(uint16_t panel_id, float val);
void _menu_page_index_sw_value_update(uint16_t panel_id, uint8_t* format, uint16_t val);
void _menu_page_index_heading_update(uint16_t panel_id, uint16_t val);

void _menu_page_index_rotate_switch_draw(UI_FRAME_PANEL_STRU *panel);

static uint8_t m_radio_battery[] = "L -.-v --%";
static uint8_t m_plane_battery[] = "R --.-v --%";
static uint8_t m_rssi[] = "RSSI --%";
static uint8_t m_switch_sa[] = "SA -";
static uint8_t m_switch_sb[] = "SB -";
static uint8_t m_switch_sc[] = "SC -";
static uint8_t m_switch_sd[] = "SD -";
static uint8_t m_switch_se[] = "SE -";
static uint8_t m_flight_mode[] = "DISARMED";
static uint8_t m_attitude_roll[] = "-----";
static uint8_t m_attitude_pitch[] = "-----";
static uint8_t m_attitude_head[] = "----N";


UI_FRAME_PANEL_STRU g_page_index_tbl[] = 
{
    /* x   y   w    h   pid                 datype                      ditype                               content*/
    {11,   6,  60,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_radio_battery},
    {79,   6,  66,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_plane_battery},
    {153,  6,  48,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_rssi},


    {16,   19, 24,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_switch_sa},
    {55,   19, 24,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_switch_sb},
    {94,   19, 24,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_switch_sc},
    {133,  19, 24,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_switch_sd},
    {172,  19, 24,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_switch_se},

    {82,   37, 48,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_flight_mode},

    {43,   50, 6,   8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "R"},
    {52,   50, 30,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_attitude_roll},
    {86,   50, 6,   8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "P"},
    {95,   50, 30,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_attitude_pitch},
    {129,  50, 6,   8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "H"},
    {138,  50, 30,  8,  MENU_PAGE_INDEX_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, m_attitude_head},

    {3,    3,  4,   26, 1,   UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_index_rotate_switch_draw},
};

uint16_t menu_page_index_tbl_size_get(void){
    return sizeof(g_page_index_tbl) / sizeof(g_page_index_tbl[0]);
}

void menu_page_index_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    ALL_STICK_INPUT_t input;

    panel_id = panel_id;
    
    switch(event){
        case UI_FRAME_EVENT_DATA_UPDATE:
            adc_all_in_val_get(&input);

            _menu_page_index_sw_value_update(3, (uint8_t*)"SA %d", input.SW.SWS.SA);
            _menu_page_index_sw_value_update(4, (uint8_t*)"SB %d", input.SW.SWS.SB);
            _menu_page_index_sw_value_update(5, (uint8_t*)"SC %d", input.SW.SWS.SC);
            _menu_page_index_sw_value_update(6, (uint8_t*)"SD %d", input.SW.SWS.SD);
            _menu_page_index_sw_value_update(7, (uint8_t*)"SE %d", input.SW.SWS.SE);
            
            _menu_page_index_value_update(10, comm_mav_data_roll_get());
            _menu_page_index_value_update(12, comm_mav_data_pitch_get());
            _menu_page_index_heading_update(14, comm_mav_data_heading_get());

            ui_frame_panel_content_set(15, UI_FRAME_PANEL_TYPE_PRIVATE, 
                g_page_index_tbl[15].content);
            
            break;
    }

    return;
}

void _menu_page_index_value_update(uint16_t panel_id, float val){
    if (val >= 0){
        menu_logic_sprintf_float(" %3f ", val, g_page_index_tbl[panel_id].content);
    }else{
        menu_logic_sprintf_float("-%3f ", val, g_page_index_tbl[panel_id].content);
    }

    ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, 
        g_page_index_tbl[panel_id].content);
}

void _menu_page_index_heading_update(uint16_t panel_id, uint16_t val){

    menu_logic_sprintf_float("%3f N", val, g_page_index_tbl[panel_id].content);
    

    ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, 
        g_page_index_tbl[panel_id].content);
}


void _menu_page_index_sw_value_update(uint16_t panel_id, uint8_t* format, uint16_t val){
    if (val > 9){ val = 0;}

    sprintf(g_page_index_tbl[panel_id].content, format, val);
    
    ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, 
        g_page_index_tbl[panel_id].content);
}


void _menu_page_index_rotate_switch_draw(UI_FRAME_PANEL_STRU *panel){
    if (panel == NULL) return;

    ALL_STICK_INPUT_t input;
    adc_all_in_val_get(&input);

    uint16_t rlu = input.adcs[ROTATE_LU];
    
    lcd_clear_rect(panel->x, panel->y, panel->width, panel->height);
    lcd_vline_disp(panel->x + 1, panel->y, panel->height, 2);

    if (rlu > 2000){
        rlu = 2000;
    } else if (rlu < 1000){
        rlu = 1000;
    }

    rlu -= 1000;
    rlu /= 40;
    lcd_hline_disp(panel->x, panel->y + rlu, panel->width, 2);
}

