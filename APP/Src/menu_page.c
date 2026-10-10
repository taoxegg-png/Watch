#include "menu_page.h"
#include "page_manager.h"

/* 只有本文件可以直接访问菜单屏幕指针 */
static lv_obj_t *s_menu_screen = NULL;


/* Back 按钮的事件回调 */
static void back_btn_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        PageManager_Show(PAGE_HOME);
    }
}

/* settime 按钮的事件回调 */
static void settime_btn_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        PageManager_Show(PAGE_TIME_SETTING);
    }
}

/* 创建菜单页面 */
void MenuPage_Init(void)
{
    lv_obj_t *menu_title;
    lv_obj_t *back_btn;
    lv_obj_t *back_label;
	lv_obj_t *settime_btn;
    lv_obj_t *settime_label;


    /*
     * 防止重复初始化时，
     * 再创建一套相同的菜单页面。
     */
    if (s_menu_screen != NULL)
    {
        return;
    }

    /* 创建独立的屏幕对象 */
    s_menu_screen = lv_obj_create(NULL);

    if (s_menu_screen == NULL)
    {
        return;
    }

    /* 设置背景 */
    lv_obj_set_style_bg_color(
        s_menu_screen,
        lv_color_black(),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        s_menu_screen,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    /* 创建菜单标题 */
    menu_title = lv_label_create(s_menu_screen);

    lv_label_set_text(menu_title, "Menu");

    lv_obj_set_style_text_color(
        menu_title,
        lv_color_white(),
        LV_PART_MAIN
    );

    lv_obj_align(
        menu_title,
        LV_ALIGN_TOP_MID,
        0,
        30
    );

    /* 创建返回按钮 */
    back_btn = lv_btn_create(s_menu_screen);

    lv_obj_set_size(back_btn, 120, 45);

    lv_obj_align(
        back_btn,
        LV_ALIGN_BOTTOM_MID,
        0,
        -15
    );

    /* 注册按钮事件 */
    lv_obj_add_event_cb(
        back_btn,
        back_btn_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    /* 创建按钮文字 */
    back_label = lv_label_create(back_btn);

    lv_label_set_text(back_label, "Back");

    lv_obj_center(back_label);
	
	/* 创建设置时间按钮 */
    settime_btn = lv_btn_create(s_menu_screen);

    lv_obj_set_size(settime_btn, 120, 45);

    lv_obj_align(
        settime_btn,
        LV_ALIGN_BOTTOM_MID,
        0,
        -100
    );

    /* 注册按钮事件 */
    lv_obj_add_event_cb(
        settime_btn,
        settime_btn_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    /* 创建按钮文字 */
    settime_label = lv_label_create(settime_btn);

    lv_label_set_text(settime_label, "SetTime");

    lv_obj_center(settime_label);
}


/* 返回菜单屏幕指针 */
lv_obj_t *MenuPage_GetScreen(void)
{
    return s_menu_screen;
}
