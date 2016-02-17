#ifndef __UI_FRAME_H__
#define __UI_FRAME_H__

#define UI_FRAME_PANEL_ID_INVALID    0xffff

#define UI_FRAME_OP_NUM_OF_PANEL_GET 0
#define UI_FRAME_OP_PANEL_INFO_GET   1

typedef enum
{
    UI_FRAME_EVENT_KEY_UP = 1,
    UI_FRAME_EVENT_KEY_DOWN,
    UI_FRAME_EVENT_KEY_ENTER,
    UI_FRAME_EVENT_KEY_EXIT,
    UI_FRAME_EVENT_KEY_MENU,
    UI_FRAME_EVENT_KEY_PAGE,    
    UI_FRAME_EVENT_KEY_INVALID,

    UI_FRAME_EVENT_DATA_UPDATE,
} UI_FRAME_EVENT_ENUM;

typedef enum
{
    UI_FRAME_PANEL_TYPE_STRING = 0,
    UI_FRAME_PANEL_TYPE_GRAPH,
    UI_FRAME_PANEL_TYPE_INVALID
} UI_FRAME_PANEL_TYPE_ENUM;

typedef enum 
{
    UI_FRAME_PANEL_DISPLAY_STATE_NORMAL = 0,
    UI_FRAME_PANEL_DISPLAY_STATE_INV,
    UI_FRAME_PANEL_DISPLAY_STATE_INVALID
} UI_FRAME_PANEL_DISPLAY_STATE_ENUM;

typedef void (*ui_frame_event_cb)
(
    uint32_t event, 
    uint16_t page_id, 
    uint16_t panel_id
);

typedef void (*ui_frame_info_cb)
(
    uint32_t op,
    uint16_t page_id, 
    uint16_t panel_id,
    void *info    
);

typedef struct ui_frame_panel_relation_s
{
    uint16_t up;
    uint16_t down;
    uint16_t left;
    uint16_t right;
} UI_FRAME_PANEL_RELATION_STRU;

typedef struct ui_frame_panel_s
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t page_id;
    uint8_t  data_type;
    uint8_t  disp_type;
    void *content;
} UI_FRAME_PANEL_STRU;

typedef struct ui_frame_panel_innel_s
{
    UI_FRAME_PANEL_STRU panel_info;
    uint8_t  dirty;
    uint8_t  reserve1;
    uint16_t reserve2;
} UI_FRAME_PANEL_INNEL_STRU;

typedef struct ui_frame_screen_s
{
    uint16_t width;
    uint16_t height;
    uint16_t num_of_panels;
    uint16_t cur_page_id;
    uint16_t cur_panel_id;
    uint16_t dirty;
    ui_frame_event_cb ev_cb;
    ui_frame_info_cb  info_cb;
    UI_FRAME_PANEL_INNEL_STRU *panels;
} UI_FRAME_SCREEN_STRU;

int32_t ui_frame_screen_init
(
    uint16_t width, 
    uint16_t height, 
    ui_frame_event_cb ev_cb,
    ui_frame_info_cb info_cb
);

int32_t ui_frame_page_change
(
    uint16_t next_page_id
);

int32_t ui_frame_panel_change
(
    uint16_t next_panel_id
);

int32_t ui_frame_panel_disp_type_set
(
    uint16_t panel_id,
    uint8_t disp_type
);

uint32_t ui_frame_panel_content_set
(
    uint16_t panel_id,
    uint8_t data_type,
    void *content         
);

int8_t ui_frame_panel_num_get
(
    void
);

int32_t ui_frame_task_start
(
    void
);

#endif