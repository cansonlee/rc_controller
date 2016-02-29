
#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

extern UI_FRAME_PANEL_STRU g_page_splash_tbl[];
extern uint16_t menu_page_splash_tbl_size_get(void);
extern void menu_page_splash_event_process(uint32_t, uint16_t);

extern UI_FRAME_PANEL_STRU g_page_index_tbl[];
extern uint16_t menu_page_index_tbl_size_get(void);
extern void menu_page_index_event_process(uint32_t, uint16_t);

extern UI_FRAME_PANEL_STRU g_page_channel_tbl[];
extern uint16_t menu_page_channel_tbl_size_get(void);
extern void menu_page_channel_event_process(uint32_t, uint16_t);

extern UI_FRAME_PANEL_STRU g_page_radio_tbl[];
extern uint16_t menu_page_radio_tbl_size_get(void);
extern void menu_page_radio_event_process(uint32_t, uint16_t);

extern UI_FRAME_PANEL_STRU g_page_channel_sw_tbl[];
extern uint16_t menu_page_channel_sw_tbl_size_get(void);
extern void menu_page_channel_sw_event_process(uint32_t, uint16_t);

extern UI_FRAME_PANEL_STRU g_page_cal_tbl[];
extern uint16_t menu_page_cal_tbl_size_get(void);
extern void menu_page_cal_event_process(uint32_t, uint16_t);


typedef struct menu_logic_t{
    uint16_t             next_page_id;
    UI_FRAME_PANEL_STRU* panels;
    
    EVENT_FN             event_fn;
    PAGE_TBL_SIZE_GET_FN tbl_size_fn;
}MENU_LOGIC_STRU;

MENU_LOGIC_STRU g_menu_logic_tbl[] = { // correspond to page id
    {MENU_PAGE_INDEX_ID, g_page_splash_tbl,
        menu_page_splash_event_process, menu_page_splash_tbl_size_get},
    {MENU_PAGE_CHANNEL_ID, g_page_index_tbl, 
        menu_page_index_event_process, menu_page_index_tbl_size_get},
    {MENU_PAGE_CHANNEL_SW_ID, g_page_channel_tbl, 
        menu_page_channel_event_process, menu_page_channel_tbl_size_get},
    {MENU_PAGE_CAL_ID, g_page_radio_tbl,
        menu_page_radio_event_process, menu_page_radio_tbl_size_get},
    {MENU_PAGE_RADIO_ID, g_page_channel_sw_tbl,
        menu_page_channel_sw_event_process, menu_page_channel_sw_tbl_size_get},
    {MENU_PAGE_INDEX_ID, g_page_cal_tbl,
        menu_page_cal_event_process, menu_page_cal_tbl_size_get},
};


#define IS_PAGE_ID_VALID(page_id) \
    ((page_id) < sizeof(g_menu_logic_tbl) / sizeof(g_menu_logic_tbl[0]))

EVENT_FN menu_logic_event_proc_get(uint16_t page_id){
    if (IS_PAGE_ID_VALID(page_id)){
        return g_menu_logic_tbl[page_id].event_fn;
    }

    return NULL;
}

UI_FRAME_PANEL_STRU* menu_logic_page_get(uint16_t page_id){
    if (IS_PAGE_ID_VALID(page_id)){
        return g_menu_logic_tbl[page_id].panels;
    }

    return NULL;
}

uint16_t menu_logic_page_next_get(uint16_t page_id){
    if (IS_PAGE_ID_VALID(page_id)){
        return g_menu_logic_tbl[page_id].next_page_id;
    }

    return page_id;
}

uint32_t menu_logic_page_tbl_size_get(uint16_t page_id){
    PAGE_TBL_SIZE_GET_FN fn;
    
    if (IS_PAGE_ID_VALID(page_id)){
        fn = g_menu_logic_tbl[page_id].tbl_size_fn;
        if (fn != NULL){
            return fn();
        }
    }

    return 0;
}

uint8_t is_panel_id_valid(uint16_t page_id, uint16_t panel_id){
    uint32_t tbl_size = menu_logic_page_tbl_size_get(page_id);
    
    if ((uint32_t)panel_id < tbl_size){
        return 1;
    }

    return 0;
}

void menu_logic_sprintf_uint16(uint16_t max, uint16_t val, char* out){

    if (out == NULL){
        return;
    }
    
    if (val > max){
        val = max;
    }

    sprintf(out, "%d", val);

    return;
}

void menu_logic_sprintf_float(char* format, float val, char* out){

    if (format == NULL || out == NULL){
        return;
    }
    
    sprintf(out, format, val);

    return;
}


