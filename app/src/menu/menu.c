
//#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "ui_frame.h"
#include "menu.h"
#include "menu_logic.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

typedef struct menu_info_s
{
    UI_FRAME_PANEL_STRU panel;
    UI_FRAME_PANEL_RELATION_STRU *rela;
} MENU_INFO_STRU;
 
void menu_event_process
(
    uint32_t event, 
    uint16_t page_id, 
    uint16_t panel_id
);

void menu_info_cb
(
    uint32_t op,
    uint16_t page_id, 
    uint16_t panel_id,
    void *info
);

int32_t menu_num_of_panel_get
(
    uint16_t page_id,
    void *info
);

int32_t menu_panel_info_get
(
    uint16_t page_id, 
    uint16_t panel_id,
    UI_FRAME_PANEL_STRU *info    
);

void menu_page_change
(
    uint16_t page_id    
);

void menu_panel_event_proc
(
    uint32_t event, 
    uint16_t page_id, 
    uint16_t panel_id
);

void menu_page_change
(
    uint16_t page_id    
)
{
    uint16_t next_page_id = menu_logic_page_next_get(page_id);
    printf("menu page change %d\r\n", next_page_id);
    if (next_page_id == page_id){
        return;
    }
    
    ui_frame_page_change(next_page_id);

    return;
}

void menu_panel_event_proc
(
    uint32_t event, 
    uint16_t page_id, 
    uint16_t panel_id
)
{
    EVENT_FN fn = menu_logic_event_proc_get(page_id);
    if (fn != NULL){
        fn(event, panel_id);
    }

    return;
}

void menu_event_process
(
    uint32_t event, 
    uint16_t page_id, 
    uint16_t panel_id
)
{    
    printf("panel event page %d panel %d event 0x%x\n", page_id, panel_id, (unsigned int)event);

    switch (event)
    {
        case UI_FRAME_KEY_MENU:
            // enter setup page
            break;
        case UI_FRAME_KEY_PAGE:
            menu_page_change(page_id);
            break;
        default:
            menu_panel_event_proc(event, page_id, panel_id);
            break;
    }
    
    return;
}

int32_t menu_num_of_panel_get
(
    uint16_t page_id,
    void *info
)
{
    *(uint32_t*)info = menu_logic_page_tbl_size_get(page_id);
    return OK;
}

int32_t menu_panel_info_get
(
    uint16_t page_id, 
    uint16_t panel_id,
    UI_FRAME_PANEL_STRU *info    
)
{
    UI_FRAME_PANEL_STRU *page;

    page = menu_logic_page_get(page_id);
    if (page == NULL){
        return -EPARAM;
    }
    
    if (!is_panel_id_valid(page_id,panel_id)){
        return -EPARAM;
    }
    
    memcpy(info, &page[panel_id], sizeof(UI_FRAME_PANEL_STRU));

    return OK;
}

void menu_info_cb
(
    uint32_t op,
    uint16_t page_id, 
    uint16_t panel_id,
    void *info
)
{
    
    switch (op)
    {
        case UI_FRAME_OP_NUM_OF_PANEL_GET:
            menu_num_of_panel_get(page_id, info);
            break;
        case UI_FRAME_OP_PANEL_INFO_GET:
            menu_panel_info_get(page_id, panel_id, info);
            break;
        default:
            break;
        
    }

    return;
    
}

void menu_init(void)
{
    int32_t ret;
    
    ret = ui_frame_screen_init(212, 64, menu_event_process, menu_info_cb);
    if (ret != 0)
    {
        printf("call ui_frame_screen_init return %d.\n", (int)ret);
        return; 
    }

    ui_frame_task_start();
    
    return;

}

