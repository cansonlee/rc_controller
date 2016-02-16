
//#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "ui_frame.h"
#include "menu.h"
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

void menu_page1_event_process
(
    uint32_t event,
    uint16_t panel_id
);

void menu_page2_event_process
(
    uint32_t event,
    uint16_t panel_id
);

void menu_page3_event_process
(
    uint32_t event,
    uint16_t panel_id
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

UI_FRAME_PANEL_STRU g_page_1_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 1"},
    {0,    16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH1"},
    {32,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH4"},
    {112,  16, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    
    {0,    32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH2"},
    {32,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH5"},
    {112,  32, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},

    {0,    48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH3"},
    {32,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
    {80,   48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "CH6"},
    {112,  48, 0,  16, 0,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "0000"},
};

UI_FRAME_PANEL_STRU g_page_2_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 2"},
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "pairing"},
    {96,   16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "OFF"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "Failsafe"},
    {96,   32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "RTL "},
    
};

UI_FRAME_PANEL_STRU g_page_3_tbl[] = 
{
    /* x   y   w   h   pid  datype                      ditype                               content*/
    {0,    0,  0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_INV,    "PAGE 3"},
    {0,    16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "battery"},
    {96,   16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "00.00"},
    {136,  16, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "v"},
    {0,    32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "current"},
    {96,   32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "00.00"},
    {136,  32, 0,  16, 1,   UI_FRAME_PANEL_TYPE_STRING, UI_FRAME_PANEL_DISPLAY_STATE_NORMAL, "A"},
};

uint16_t g_page_tbl_size[] = 
{
    (sizeof(g_page_1_tbl) / sizeof(g_page_1_tbl[0])),
    (sizeof(g_page_2_tbl) / sizeof(g_page_2_tbl[0])),
    (sizeof(g_page_3_tbl) / sizeof(g_page_3_tbl[0])),
};

void menu_page1_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    panel_id = panel_id;
    event = event;
    
    switch (event)
    {
        case UI_FRAME_KEY_UP:
        case UI_FRAME_KEY_DOWN:
        case UI_FRAME_KEY_ENTER:
        case UI_FRAME_KEY_EXIT:
        case UI_FRAME_KEY_MENU:
            
        default:
            break;
    }
}

void menu_page2_event_process
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
            printf("panel_id %d\r\n", panel_id);
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

void menu_page3_event_process
(
    uint32_t event,
    uint16_t panel_id
)
{
    panel_id = panel_id;
    event = event;
    
    switch (event)
    {
        case UI_FRAME_KEY_UP:
        case UI_FRAME_KEY_DOWN:
        case UI_FRAME_KEY_ENTER:
        case UI_FRAME_KEY_EXIT:
        case UI_FRAME_KEY_MENU:
        default:
            break;
    }

    return;
}

void menu_page_change
(
    uint16_t page_id    
)
{
    uint16_t next_page_id;

    switch (page_id)
    {
        case 0:
            next_page_id = 1;
            break;
        case 1:
            next_page_id = 2;
            break;
        case 2:
            next_page_id = 0;
            break;
        default:
            next_page_id = 0;
            break;
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

    switch (page_id)
    {
        case 0:
            menu_page1_event_process(event, panel_id);
            break;
        case 1:
            menu_page2_event_process(event, panel_id);
            break;
        case 2:
            menu_page3_event_process(event, panel_id);
            break;
        default:
            break;
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
    *(uint32_t*)info = g_page_tbl_size[page_id];
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
    
    switch (page_id)
    {
        case 0:
            page = g_page_1_tbl;
            break;
        case 1:
            page = g_page_2_tbl;
            break;
        case 2:
            page = g_page_3_tbl;
            break;        
        default:
            return -EPARAM;
            break;
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

