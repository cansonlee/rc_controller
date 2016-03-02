#ifndef __MENU_LOGIC_H__
#define __MENU_LOGIC_H__

#include "lcd.h"

typedef void (*EVENT_FN)(uint32_t, uint16_t); // evevnt process function
typedef uint16_t (*PAGE_TBL_SIZE_GET_FN)(void);

#define MENU_PAGE_SPLASH_ID     0
#define MENU_PAGE_INDEX_ID      1
#define MENU_PAGE_CHANNEL_ID    2
#define MENU_PAGE_RADIO_ID      3
#define MENU_PAGE_CHANNEL_SW_ID 4
#define MENU_PAGE_CAL_ID        5


EVENT_FN menu_logic_event_proc_get(uint16_t page_id);
UI_FRAME_PANEL_STRU* menu_logic_page_get(uint16_t page_id);
uint16_t menu_logic_page_next_get(uint16_t page_id);
uint32_t menu_logic_page_tbl_size_get(uint16_t page_id);
uint8_t is_panel_id_valid(uint16_t page_id, uint16_t panel_id);
void menu_logic_sprintf_uint16(uint16_t max, uint16_t val, char* out);
void menu_logic_sprintf_float(char* format, float val, char* out);

uint8_t menu_logic_rotate_val_update(uint8_t type, uint16_t val);
uint8_t menu_logic_stick_val_update(uint8_t type, uint16_t val);
uint8_t menu_logic_sw_val_update(uint16_t val);
uint16_t menu_logic_sw_val_get(void);
uint16_t menu_logic_stick_rotate_val_get(uint8_t type);
inline uint16_t menu_logic_channel_val_limit(uint16_t val);
inline void menu_logic_stick_point_draw(uint16_t x, uint16_t y);

inline uint16_t menu_logic_channel_val_limit(uint16_t val){
    if (val > 2000){        
        val = 2000;         
    } else if (val < 1000){ 
        val = 1000;         
    }

    return val;
}

// center point (x, y)
inline void menu_logic_stick_point_draw(uint16_t x, uint16_t y){

    lcd_hline_erase(x, y, 1, 1);
    
    lcd_hline_disp(x - 1, y - 2, 3, 1);
    lcd_hline_disp(x, y - 1, 1, 1);
    lcd_hline_disp(x, y + 1, 1, 1);
    lcd_hline_disp(x - 1, y + 2, 3, 1);
    
    lcd_vline_disp(x + 1, y, 1, 1);
    lcd_vline_disp(x + 2, y - 1, 3, 1);
    lcd_vline_disp(x - 1, y, 1, 1);
    lcd_vline_disp(x - 2, y - 1, 3, 1);
}


#endif

