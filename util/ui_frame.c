#include "cmsis_os.h"
#include "ui_frame.h"
#include "lcd.h"
#include "keys.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define UI_FRAME_PANEL_FLASH_TIME                (500)

UI_FRAME_SCREEN_STRU g_uiScreen;
osThreadId uiTaskHandle;

void ui_task
(
    void const * argument
);

int32_t ui_frame_panel_display
(
    UI_FRAME_PANEL_STRU *panel
);

static void ui_frame_panel_disp_state_set
(
    UI_FRAME_PANEL_STRU *panel,
    uint8_t state
);

static void ui_frame_display_update
(
    void
);

static int32_t ui_frame_page_create
(
    uint16_t page_id
);

uint32_t key_to_event_map(KEY_STATUS key_status, KEY_VAL key);


int32_t ui_frame_screen_init
(
    uint16_t width, 
    uint16_t height, 
    ui_frame_event_cb ev_cb,
    ui_frame_info_cb info_cb
)
{
    IF_PTR_IS_NULL_RET_NULLPTR_ERR(ev_cb);
    IF_PTR_IS_NULL_RET_NULLPTR_ERR(info_cb);
    
    g_uiScreen.width = width;
    g_uiScreen.height = height;
    g_uiScreen.ev_cb = ev_cb;
    g_uiScreen.info_cb = info_cb;
    g_uiScreen.num_of_panels = 0;
    g_uiScreen.cur_page_id = 0;
    g_uiScreen.cur_panel_id = 0;
    g_uiScreen.panels = NULL;
    g_uiScreen.dirty = 1;

    return OK;
}

int32_t ui_frame_panel_display
(
    UI_FRAME_PANEL_STRU *panel
)
{
    switch (panel->data_type)
    {
        case UI_FRAME_PANEL_TYPE_STRING:
            ui_frame_panel_disp_state_set(panel, panel->disp_type);
            break;
        case UI_FRAME_PANEL_TYPE_GRAPH:
            lcd_disp_bmp(panel->x, panel->y, panel->content, panel->width, panel->height);
            break;
        default:
            return -ENOSUPPORT;
    }

    return OK;
    
}

static int32_t ui_frame_page_create
(
    uint16_t page_id
)
{
    uint32_t num_of_panels;
    uint16_t panel_id;
    UI_FRAME_PANEL_STRU panel_info;
    
    g_uiScreen.info_cb(UI_FRAME_OP_NUM_OF_PANEL_GET, page_id, 0, &num_of_panels);
    if (g_uiScreen.num_of_panels != num_of_panels)
    {
        if (NULL != g_uiScreen.panels)
        {
            free(g_uiScreen.panels);
        }

        g_uiScreen.panels = malloc(sizeof(UI_FRAME_PANEL_INNEL_STRU) * num_of_panels);
        if (NULL == g_uiScreen.panels)
        {
            printf("[%s, L%d] no memory!\r\n", __func__, __LINE__);
            return -ENOMEM;
        }

        memset(g_uiScreen.panels, 0, sizeof(UI_FRAME_PANEL_INNEL_STRU) * num_of_panels);
    }

    for (panel_id = 0; panel_id < num_of_panels; panel_id++)
    {
        g_uiScreen.info_cb(UI_FRAME_OP_PANEL_INFO_GET, page_id, panel_id, &panel_info);
        memcpy(&g_uiScreen.panels[panel_id].panel_info, 
                    &panel_info, sizeof(UI_FRAME_PANEL_STRU));
        g_uiScreen.panels[panel_id].dirty = 1;
    }

    g_uiScreen.num_of_panels = num_of_panels;
    g_uiScreen.cur_page_id = page_id;
    g_uiScreen.cur_panel_id = UI_FRAME_PANEL_ID_INVALID;
    g_uiScreen.dirty = 1;

    return OK;
}

static void ui_frame_panel_disp_state_set
(
    UI_FRAME_PANEL_STRU *panel,
    uint8_t state
)
{
    printf("panel %p state %d content %s\r\n", panel, state, panel->content);
    if (UI_FRAME_PANEL_DISPLAY_STATE_NORMAL == state)
    {
        lcd_str_disp(panel->x, panel->y, panel->content);         
    }
    else
    {
        lcd_str_inv_disp(panel->x, panel->y, panel->content);
    }

    panel->disp_type = state;
    return;
}

int8_t ui_frame_panel_num_get
(
    void
)
{    
    return g_uiScreen.num_of_panels;
}

int32_t ui_frame_page_change
(
    uint16_t next_page_id
)
{
    g_uiScreen.cur_page_id = next_page_id;
    g_uiScreen.dirty = 1;
    return OK;
}

int32_t ui_frame_panel_change
(
    uint16_t next_panel_id
)
{
    IF_CONDITION_TURE_RET_PARAM_ERR(next_panel_id > g_uiScreen.num_of_panels);
    g_uiScreen.cur_panel_id = next_panel_id;
    return OK;
}

int32_t ui_frame_panel_disp_type_set
(
    uint16_t panel_id,
    uint8_t disp_type
)
{
    UI_FRAME_PANEL_INNEL_STRU *panel;

    IF_CONDITION_TURE_RET_PARAM_ERR(panel_id > g_uiScreen.num_of_panels);
    IF_CONDITION_TURE_RET_PARAM_ERR(disp_type >= UI_FRAME_PANEL_DISPLAY_STATE_INVALID);
    
    panel = &g_uiScreen.panels[panel_id];
    panel->panel_info.disp_type = disp_type;
    panel->dirty = 1;

    return OK;
}

uint32_t ui_frame_panel_content_set
(
    uint16_t panel_id,
    uint8_t data_type,
    void *content         
)
{
    UI_FRAME_PANEL_INNEL_STRU *panel;
    
    IF_CONDITION_TURE_RET_PARAM_ERR(panel_id > g_uiScreen.num_of_panels);
    
    panel = &g_uiScreen.panels[panel_id];
    panel->panel_info.data_type = data_type;
    panel->panel_info.content = content;
    panel->dirty = 1;

    return OK;
}

static void ui_frame_display_update
(
    void
)
{
    uint16_t panel_id;
    int32_t ret;
    UI_FRAME_PANEL_INNEL_STRU *panel;
    
    if (g_uiScreen.dirty)
    {
        ret = ui_frame_page_create(g_uiScreen.cur_page_id);
        if (OK != ret)
        {
            printf("[%s, L%d] call ui_frame_page_fresh ret %d\n", __func__, (int)__LINE__, (int)ret);
            return;
        }
        lcd_clean();
		osDelay(50);
        for (panel_id = 0; panel_id < g_uiScreen.num_of_panels; panel_id++)
        {
            panel = &g_uiScreen.panels[panel_id];
            ui_frame_panel_display(&panel->panel_info);
            panel->dirty = 0;
        }
		printf("lcd display @ %s, %s, %d\r\n", __FILE__, __func__, __LINE__);
        g_uiScreen.dirty = 0;		
    }
    else
    {
        for (panel_id = 0; panel_id < g_uiScreen.num_of_panels; panel_id++)
        {
            panel = &g_uiScreen.panels[panel_id];
            if (panel->dirty)
            {
                ui_frame_panel_display(&panel->panel_info);
                panel->dirty = 0;
            }
        }
    }
}

int32_t ui_frame_task_start
(
    void
)
{
    osThreadDef(uiTask, ui_task, osPriorityNormal, 0, 2048);
    uiTaskHandle = osThreadCreate(osThread(uiTask), NULL);
    if (NULL == uiTaskHandle)
    {
        printf("osThreadCreate failed!\n");
        return -EOS;
    }

    return OK;
}

void ui_task
(
    void const * argument
)
{
    uint32_t event;
    ui_frame_event_cb fn;
    KEY_STATUS key_status;
    KEY_VAL key;
    argument = argument;

    for (;;)
    {
        osDelay(50);
        ui_frame_display_update();

        key = keys_read(&key_status);

        event = key_to_event_map(key_status, key);
        
        fn = g_uiScreen.ev_cb;

        if (fn == NULL){
            continue;
        }

        fn(UI_FRAME_EVENT_DATA_UPDATE, g_uiScreen.cur_page_id, g_uiScreen.cur_panel_id);
		printf("g_uiScreen.cur_page_id=%d @ %s, %s, L%d \r\n",g_uiScreen.cur_page_id, __FILE__, __func__, __LINE__);
        
        if (0 != event)
        {
            fn(event, g_uiScreen.cur_page_id, g_uiScreen.cur_panel_id);
        }
    }
}

uint32_t key_to_event_map(KEY_STATUS key_status, KEY_VAL key){

    uint32_t event = 0;

    if (key == KEY_READY){
        return event;
    }

    switch(key_status){
        case KEY_PRESSED:
        case KEY_HOLD:
            break;
        default:
            return event;
    }
    
    switch(key)
    {
        case KEY_MENU:
            event = UI_FRAME_EVENT_KEY_MENU;
            break;
        case KEY_PAGE:
            event = UI_FRAME_EVENT_KEY_PAGE;
            break;
        case KEY_EXIT:
            event = UI_FRAME_EVENT_KEY_EXIT;
            break;
        case KEY_PLUS:
            event = UI_FRAME_EVENT_KEY_UP;
            break;
        case KEY_MINUS:
            event = UI_FRAME_EVENT_KEY_DOWN;
            break;
        case KEY_ENTER:
            event = UI_FRAME_EVENT_KEY_ENTER;
            break;
        default:
            break;
    }
             
    return event;
}

