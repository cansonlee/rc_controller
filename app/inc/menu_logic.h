#ifndef __MENU_LOGIC_H__
#define __MENU_LOGIC_H__

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

#endif

