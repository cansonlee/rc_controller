
#include "cmsis_os.h"

#include "ui_frame.h"

#include "ADCs.h"

#include "menu_logic.h"

#include "lcd.h"

uint16_t menu_page_cal_tbl_size_get(void);
void menu_page_cal_event_process
(
    uint32_t event,
    uint16_t panel_id
);

void _menu_page_cal_private_draw_update(uint16_t panel_id);

void _menu_page_cal_top_line_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel);
void _menu_page_cal_stick_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel);
void _menu_page_cal_rotate_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel);


UI_FRAME_PANEL_STRU g_page_cal_tbl[] = 
{
    /* x   y   w   h   pid               datype                      ditype                               content*/
    {6,    3,  66, 8,  MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CALIBRATION"},
    {193,  3,  18, 8,  MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "1/2"},

    {6,    12, 200,1,  MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_top_line_draw},

    {43,   15, 126,8,  MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CENTER STICKS/SLIDERS"},
    {67,   28, 78, 8,  MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "[ENTER] START"},
    
    {10,   23, 40, 40, MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_stick_draw},
    {162,  23, 40, 40, MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_stick_draw},

    {93,   40, 4,  22,MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_rotate_draw},
    {99,   40, 4,  22,MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_rotate_draw},
    {109,  40, 4,  22,MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_rotate_draw},
    {115,  40, 4,  22,MENU_PAGE_CAL_ID, UI_FRAME_PANEL_TYPE_PRIVATE,UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, _menu_page_cal_rotate_draw},
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
    static uint8_t panel_cal_status = 0;

    UI_FRAME_PANEL_STRU* panel;
    ALL_STICK_INPUT_t input;

    panel_id = panel_id;
    
    switch (event)
    {
        case UI_FRAME_EVENT_KEY_ENTER:

            if (panel_cal_status == 0){
                panel_cal_status = 1;
                
                ui_frame_panel_content_set(3, UI_FRAME_PANEL_TYPE_STRING, "");

                panel = &g_page_cal_tbl[4];
                panel->width = 17 * 6;
                panel->x = (212 - panel->width)>>1;
                ui_frame_panel_content_set(4, UI_FRAME_PANEL_TYPE_STRING, "[ENTER] WHEN DONE");

                adc_stick_cntr_val_set();
                adc_stick_most_val_set_start();
            }else {
                panel_cal_status = 1;

                ui_frame_panel_content_set(3, UI_FRAME_PANEL_TYPE_STRING, "CENTER STICKS/SLIDERS");

                panel = &g_page_cal_tbl[4];
                panel->width = 13 * 6;
                panel->x = (212 - panel->width)>>1;
                ui_frame_panel_content_set(4, UI_FRAME_PANEL_TYPE_STRING, "[ENTER] START");

                adc_stick_most_val_set_end();
            }
            break;

        case UI_FRAME_EVENT_DATA_UPDATE:
            adc_all_in_val_get(&input);

            if (menu_logic_stick_val_update(STICK_LH, input.adcs[STICK_LH])
                ||menu_logic_stick_val_update(STICK_LV, input.adcs[STICK_LV])){
                _menu_page_cal_private_draw_update(5);
            }

            if (menu_logic_stick_val_update(STICK_RH, input.adcs[STICK_RH])
                ||menu_logic_stick_val_update(STICK_RV, input.adcs[STICK_RV])){
                _menu_page_cal_private_draw_update(6);
            }

            if (menu_logic_rotate_val_update(ROTATE_LU, input.adcs[ROTATE_LU])){
                _menu_page_cal_private_draw_update(7);
            }
            if (menu_logic_rotate_val_update(ROTATE_LD, input.adcs[ROTATE_LU])){
                _menu_page_cal_private_draw_update(8);
            }
            if (menu_logic_rotate_val_update(ROTATE_RU, input.adcs[ROTATE_LU])){
                _menu_page_cal_private_draw_update(9);
            }
            if (menu_logic_rotate_val_update(ROTATE_RD, input.adcs[ROTATE_LU])){
                _menu_page_cal_private_draw_update(10);
            }
            
            break;
            
        default:
            break;
    }

    return;
}

void _menu_page_cal_private_draw_update(uint16_t panel_id){
    ui_frame_panel_content_set(panel_id, UI_FRAME_PANEL_TYPE_PRIVATE, 
                g_page_cal_tbl[panel_id].content);
}


void _menu_page_cal_top_line_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel){

    if (panel == NULL) return;

    panel_id = panel_id;

    lcd_hline_disp(panel->x, panel->y, panel->width, 1);
}

void _menu_page_cal_stick_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel){

    uint16_t h_value, v_value;
    
    if (panel == NULL) return;
    
    lcd_clear_rect(panel->x, panel->y, panel->width, panel->height);
    
    switch(panel_id){
        case 5:
            h_value = menu_logic_stick_rotate_val_get(STICK_LH);
            v_value = menu_logic_stick_rotate_val_get(STICK_LV);
            break;
        case 6:
        default:
            h_value = menu_logic_stick_rotate_val_get(STICK_RH);
            v_value = menu_logic_stick_rotate_val_get(STICK_RV);
            break;
    }

    h_value = menu_logic_channel_val_limit(h_value);
    v_value = menu_logic_channel_val_limit(v_value);

    h_value -= 1000;
    h_value /= 25;

    v_value -= 1000;
    v_value /= 25;

    menu_logic_stick_point_draw(h_value, v_value);

    lcd_hline_disp(panel->x, panel->y, panel->width, 1);
    lcd_hline_disp(panel->x, panel->y + panel->height - 1, panel->width, 1);
    lcd_vline_disp(panel->x, panel->y, panel->height, 1);
    lcd_vline_disp(panel->x + panel->width - 1, panel->y, panel->height, 1);
}

void _menu_page_cal_rotate_draw(uint16_t panel_id, UI_FRAME_PANEL_STRU* panel){
    panel_id = panel_id;
    panel = panel;
}

