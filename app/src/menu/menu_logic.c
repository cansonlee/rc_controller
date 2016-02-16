
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

EVENT_FN g_event_proc_tbl[] = { // correspond to page id
    menu_page_index_event_process,
    menu_page_channel_event_process,
    menu_page_radio_event_process,
};

UI_FRAME_PANEL_STRU* g_page_tbl[] = { // correspond to page id
    g_page_index_tbl,
    g_page_channel_tbl,
    g_page_radio_tbl,
};

PAGE_TBL_SIZE_GET_FN g_page_tbl_size[] = { // correspond to page id
    menu_page_channel_tbl_size_get,
    menu_page_channel_tbl_size_get,
    menu_page_channel_tbl_size_get,
};

uint16_t g_page_next_tbl[] = { // correspond to page id
    MENU_PAGE_INDEX_ID,        // index -> index
    MENU_PAGE_RADIO_ID,        // channel -> radio
    MENU_PAGE_CHANNEL_ID,      // radio -> channel
};

EVENT_FN menu_logic_event_proc_get(uint16_t page_id){
    if (page_id < sizeof(g_event_proc_tbl) / sizeof(EVENT_FN)){
        return g_event_proc_tbl[page_id];
    }

    return NULL;
}

UI_FRAME_PANEL_STRU* menu_logic_page_get(uint16_t page_id){
    if (page_id < sizeof(g_page_tbl) / sizeof(UI_FRAME_PANEL_STRU*)){
        return g_page_tbl[page_id];
    }

    return NULL;
}

uint16_t menu_logic_page_next_get(uint16_t page_id){
    if (page_id < sizeof(g_page_next_tbl)){
        return g_page_next_tbl[page_id];
    }

    return page_id;
}

uint32_t menu_logic_page_tbl_size_get(uint16_t page_id){
    PAGE_TBL_SIZE_GET_FN fn;
    
    if (page_id < sizeof(g_page_tbl_size)){
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


