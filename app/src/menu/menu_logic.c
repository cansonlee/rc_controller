
#include "cmsis_os.h"

#include "ui_frame.h"
#include "menu_logic.h"

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



EVENT_FN g_event_proc_tbl[] = { // correspond to page id
    menu_page_index_event_process,
    menu_page_channel_event_process,
    menu_page_radio_event_process,
    menu_page_channel_sw_event_process,
};

UI_FRAME_PANEL_STRU* g_page_tbl[] = { // correspond to page id
    g_page_index_tbl,
    g_page_channel_tbl,
    g_page_radio_tbl,
    g_page_channel_sw_tbl,
};

PAGE_TBL_SIZE_GET_FN g_page_tbl_size[] = { // correspond to page id
    menu_page_index_tbl_size_get,
    menu_page_channel_tbl_size_get,
    menu_page_radio_tbl_size_get,
    menu_page_channel_sw_tbl_size_get,
};

uint16_t g_page_next_tbl[] = { // correspond to page id
    MENU_PAGE_CHANNEL_ID,      // index -> channel
    MENU_PAGE_CHANNEL_SW_ID,   // channel -> channel sw
    MENU_PAGE_RADIO_ID,        // channel sw -> radio
    MENU_PAGE_INDEX_ID,        // radio -> index
};

EVENT_FN menu_logic_event_proc_get(uint16_t page_id){
    if (page_id < sizeof(g_event_proc_tbl) / sizeof(g_event_proc_tbl[0])){
        return g_event_proc_tbl[page_id];
    }

    return NULL;
}

UI_FRAME_PANEL_STRU* menu_logic_page_get(uint16_t page_id){
    if (page_id < sizeof(g_page_tbl) / sizeof(g_page_tbl[0])){
        return g_page_tbl[page_id];
    }

    return NULL;
}

uint16_t menu_logic_page_next_get(uint16_t page_id){
    if (page_id < sizeof(g_page_next_tbl) / sizeof(g_page_next_tbl[0])){
        return g_page_next_tbl[page_id];
    }

    return page_id;
}

uint32_t menu_logic_page_tbl_size_get(uint16_t page_id){
    PAGE_TBL_SIZE_GET_FN fn;
    
    if (page_id < sizeof(g_page_tbl_size) / sizeof(g_page_tbl_size[0])){
        fn = g_page_tbl_size[page_id];
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

