#include "home_page.h"
#include "page_manager.h"

/* 主页屏幕对象 */
static lv_obj_t *s_home_screen = NULL;

/* 主页内部控件 */
static lv_obj_t *s_title_label = NULL;
static lv_obj_t *s_time_label = NULL;
static lv_obj_t *s_steps_label = NULL;
static lv_obj_t *s_battery_label = NULL;
static lv_obj_t *s_hr_label = NULL;


/* 主页 Menu 按钮回调 */
static void menu_btn_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        PageManager_Show(PAGE_MENU);
    }
}


/* 创建主页 */
void HomePage_Init(void)
{
    lv_obj_t *menu_btn;
    lv_obj_t *menu_btn_label;

    /* 当前方案：主页只创建一次 */
    if (s_home_screen != NULL)
    {
        return;
    }

    /*
     * 沿用之前的主页：
     * 获取 LVGL 初始化后已有的活动屏幕。
     */
    s_home_screen = lv_scr_act();

    if (s_home_screen == NULL)
    {
        return;
    }

    /* 主页背景 */
    lv_obj_set_style_bg_color(
        s_home_screen,
        lv_color_black(),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        s_home_screen,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    /* 标题 */
    s_title_label = lv_label_create(s_home_screen);

    lv_label_set_text(s_title_label, "My Watch");

    lv_obj_set_style_text_color(
        s_title_label,
        lv_color_white(),
        LV_PART_MAIN
    );

    lv_obj_align(
        s_title_label,
        LV_ALIGN_TOP_MID,
        0,
        20
    );

    /* 时间 */
    s_time_label = lv_label_create(s_home_screen);

    lv_label_set_text(s_time_label, "00:00:00");

    lv_obj_align(
        s_time_label,
        LV_ALIGN_TOP_MID,
        0,
        65
    );

    /* 步数 */
    s_steps_label = lv_label_create(s_home_screen);

    lv_label_set_text(s_steps_label, "Steps: 0");

    lv_obj_align(
        s_steps_label,
        LV_ALIGN_TOP_MID,
        0,
        105
    );

    /* 电量 */
    s_battery_label = lv_label_create(s_home_screen);

    lv_label_set_text(s_battery_label, "Battery: 0%");

    lv_obj_align(
        s_battery_label,
        LV_ALIGN_TOP_MID,
        0,
        135
    );

    /* 心率 */
    s_hr_label = lv_label_create(s_home_screen);

    lv_label_set_text(s_hr_label, "HR: 0");

    lv_obj_align(
        s_hr_label,
        LV_ALIGN_TOP_MID,
        0,
        165
    );

    /* Menu 按钮 */
    menu_btn = lv_btn_create(s_home_screen);

    lv_obj_set_size(menu_btn, 120, 45);

    lv_obj_align(
        menu_btn,
        LV_ALIGN_BOTTOM_MID,
        0,
        -15
    );

    lv_obj_add_event_cb(
        menu_btn,
        menu_btn_event_cb,
        LV_EVENT_CLICKED,
        NULL
    );

    menu_btn_label = lv_label_create(menu_btn);

    lv_label_set_text(menu_btn_label, "Menu");

    lv_obj_center(menu_btn_label);
}


/* 对外提供主页屏幕指针 */
lv_obj_t *HomePage_GetScreen(void)
{
    return s_home_screen;
}


/* 更新主页数据 */
void HomePage_Update(const WatchData_t *data)
{
    if ((data == NULL) ||
        (s_home_screen == NULL))
    {
        return;
    }

    lv_label_set_text_fmt(
        s_time_label,
        "%02u:%02u:%02u",
        (unsigned int)data->hour,
        (unsigned int)data->minute,
        (unsigned int)data->second
    );

    lv_label_set_text_fmt(
        s_steps_label,
        "Steps: %lu",
        (unsigned long)data->steps
    );

    lv_label_set_text_fmt(
        s_battery_label,
        "Battery: %u%%",
        (unsigned int)data->battery
    );

    lv_label_set_text_fmt(
        s_hr_label,
        "HR: %u",
        (unsigned int)data->heart_rate
    );
}
