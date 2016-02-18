
#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"
#include "communicate.h"

uint16_t menu_page_index_tbl_size_get(void);
void menu_page_index_event_process
(
    uint32_t event,
    uint16_t panel_id
);
void _menu_page_index_value_update(uint16_t panel_id, uint16_t val);


UI_FRAME_PANEL_STRU g_page_index_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 1"},
    
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "ROLL"},
    {64,   16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "----.-"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "PITCH"},
    {64,   32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "----.-"},
    {0,    48, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "ALT"},
    {64,   48, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "----.-"},
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
    panel_id = panel_id;
    
    switch(event){
        case UI_FRAME_EVENT_DATA_UPDATE:
            _menu_page_index_value_update(2, comm_mav_data_roll_get());
            _menu_page_index_value_update(2, comm_mav_data_pitch_get());
            _menu_page_index_value_update(2, comm_mav_data_alt_get());
            break;
    }

    return;
}

void _menu_page_index_value_update(uint16_t panel_id, uint16_t val){
    if (val > 0){
        menu_logic_sprintf_float(" 3.1f", val, g_page_index_tbl[panel_id].content);
    }else{
        menu_logic_sprintf_float("-3.1f", val, g_page_index_tbl[panel_id].content);
    }
}


