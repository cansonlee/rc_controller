
#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

uint16_t menu_page_index_tbl_size_get(void);
void menu_page_index_event_process
(
    uint32_t event,
    uint16_t panel_id
);


UI_FRAME_PANEL_STRU g_page_index_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 1"},
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "battery"},
    {96,   16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "00.00"},
    {136,  16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "v"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "current"},
    {96,   32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "00.00"},
    {136,  32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "A"},
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
    event = event;

    return;
}


