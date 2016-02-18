
#include "cmsis_os.h"

#include "ui_frame.h"

#include "ADCs.h"

uint16_t menu_page_cal_tbl_size_get(void);
void menu_page_cal_event_process
(
    uint32_t event,
    uint16_t panel_id
);


UI_FRAME_PANEL_STRU g_page_cal_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 5"},
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "STICK CENTER CAL"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "LIMIT CAL"},
    {136,  32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "START"},
    
};

uint16_t menu_page_cal_tbl_size_get(void){
    return sizeof(g_page_cal_tbl) / sizeof(g_page_cal_tbl[0]);
}

void menu_page_cal_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    uint16_t next_panel_id;
    static uint8_t panel_limit_cal_status = 0;
    
    switch (event)
    {
        case UI_FRAME_EVENT_KEY_UP:
        case UI_FRAME_EVENT_KEY_DOWN:
            switch (panel_id)
            {
                case 1:
                    next_panel_id = 3;
                    break;
                case 3:
                    next_panel_id = 1;
                    break;
                case UI_FRAME_PANEL_ID_INVALID:
                    next_panel_id = 3;
                    break;
                default:
                    next_panel_id = 3;
                    break;
            }
            ui_frame_panel_disp_type_set(panel_id, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL);
            ui_frame_panel_disp_type_set(next_panel_id, UI_FRAME_PANEL_DISPLAY_STATE_INV);
            ui_frame_panel_change(next_panel_id);
            break;
        
        case UI_FRAME_EVENT_KEY_ENTER:
            switch (panel_id)
            {
                case 1:
                    adc_stick_cntr_val_set();
                    break;
                case 3:
                    if (panel_limit_cal_status == 0){
                        panel_limit_cal_status = 1;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "END");
                        adc_stick_most_val_set_start();
                    } else {
                        panel_limit_cal_status = 0;
                        ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_STRING, "START");
                        adc_stick_most_val_set_end();
                    }
                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }

    return;
}


