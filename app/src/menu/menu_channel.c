
#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

uint16_t menu_page_channel_tbl_size_get(void);
void menu_page_channel_event_process
(
    uint32_t event,
    uint16_t panel_id
);


UI_FRAME_PANEL_STRU g_page_channel_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 2"},
    {0,    16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH1"},
    {32,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH4"},
    {112,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    
    {0,    32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH2"},
    {32,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH5"},
    {112,  32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},

    {0,    48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH3"},
    {32,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH6"},
    {112,  48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
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
    
    switch (event)
    {
        case UI_FRAME_KEY_UP:
        case UI_FRAME_KEY_DOWN:
        case UI_FRAME_KEY_ENTER:
            break;

        case UI_FRAME_KEY_EXIT:
            // return to index
            ui_frame_page_change(MENU_PAGE_INDEX_ID);
            break;
        
        default:
            break;
    }
}


