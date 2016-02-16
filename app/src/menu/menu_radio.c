
#include "cmsis_os.h"

#include "ui_frame.h"

uint16_t menu_page_radio_tbl_size_get(void);
void menu_page_radio_event_process
(
    uint32_t event,
    uint16_t panel_id
);


UI_FRAME_PANEL_STRU g_page_radio_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 3"},
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "pairing"},
    {96,   16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "OFF"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "Failsafe"},
    {96,   32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RTL "},
    
};

uint16_t menu_page_radio_tbl_size_get(void){
    return sizeof(g_page_radio_tbl) / sizeof(g_page_radio_tbl[0]);
}

void menu_page_radio_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    uint16_t next_panel_id;
    static uint8_t panel_pairing_status = 0;
    static uint8_t panel_failsafe_mode = 0;
    
    switch (event)
    {
        case UI_FRAME_KEY_UP:
            switch (panel_id)
            {
                case UI_FRAME_PANEL_ID_INVALID:
                    next_panel_id = 4;
                    break;
                case 4:
                    next_panel_id = 2;
                    break;
                case 2:
                    next_panel_id = 4;
                    break;
                default:
                    next_panel_id = 4;
                    break;
            }
            ui_frame_panel_disp_type_set(panel_id, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL);
            ui_frame_panel_disp_type_set(next_panel_id, UI_FRAME_PANEL_DISPLAY_STATE_INV);
            ui_frame_panel_change(next_panel_id);
            break;
        case UI_FRAME_KEY_DOWN:
            switch (panel_id)
            {
                case UI_FRAME_PANEL_ID_INVALID:
                    next_panel_id = 2;
                    break;
                case 4:
                    next_panel_id = 2;
                    break;
                case 2:
                    next_panel_id = 4;
                    break;
                default:
                    next_panel_id = 2;
                    break;
            }
            ui_frame_panel_disp_type_set(panel_id, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL);
            ui_frame_panel_disp_type_set(next_panel_id, UI_FRAME_PANEL_DISPLAY_STATE_INV);
            ui_frame_panel_change(next_panel_id);
            break;
        case UI_FRAME_KEY_ENTER:
            switch (panel_id)
            {
                case 2:
                    if (panel_pairing_status == 0)
                    {
                        panel_pairing_status = 1;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "ON ");
                    }
                    else
                    {
                        panel_pairing_status = 0;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "OFF");
                    }
                    break;
                case 4:
                    if (panel_failsafe_mode == 0)
                    {
                        panel_failsafe_mode = 1;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "RTL ");
                    }
                    else if (panel_failsafe_mode == 1)
                    {
                        panel_failsafe_mode = 2;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "KEEP");
                    }
                    else
                    {
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "LAND");
                        panel_failsafe_mode = 0;
                    }
                    break;
                default:
                    break;
                    
            }
        case UI_FRAME_KEY_EXIT:
        case UI_FRAME_KEY_MENU:
        default:
            break;
    }

    return;
}


