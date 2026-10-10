#include "time_setting_page.h"
#include "page_manager.h"
#include "watch_rtc.h"
#include "lvgl.h"
#include <stdio.h>

static lv_obj_t *time_label;

static uint8_t edit_hour;
static uint8_t edit_minute;
static uint8_t edit_second;

/* 更新界面上的时间 */
static void TimeSetting_UpdateLabel(void)
{
    lv_label_set_text_fmt(
        time_label,
        "%02u:%02u:%02u",
        (unsigned int)edit_hour,
        (unsigned int)edit_minute,
        (unsigned int)edit_second
    );
}


/* 小时 + */
static void HourPlus_Callback(lv_event_t *e)
{
    (void)e;

    if (edit_hour >= 23U)
    {
        edit_hour = 0U;
    }
    else
    {
        edit_hour++;
    }

    TimeSetting_UpdateLabel();
}


/* 小时 - */
static void HourMinus_Callback(lv_event_t *e)
{
    (void)e;

    if (edit_hour == 0U)
    {
        edit_hour = 23U;
    }
    else
    {
        edit_hour--;
    }

    TimeSetting_UpdateLabel();
}


/* 保存 */
static void Save_Callback(lv_event_t *event)
{
    WatchRTC_SetTime(
        edit_hour,
        edit_minute,
        edit_second
    );
	
	if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        PageManager_Show(PAGE_MENU);
    }
}


/* 只有本文件可以直接访问菜单屏幕指针 */
static lv_obj_t *time_setting_screen = NULL;


/* 创建菜单页面 */
void TimeSettingPage_Init(void)
{
    WatchRTC_Time_t current_time;

    /*
     * 读取 RTC 当前时间
     */
    if (WatchRTC_GetTime(&current_time) == HAL_OK)
    {
        edit_hour   = current_time.hour;
        edit_minute = current_time.minute;
        edit_second = current_time.second;
    }
    else
    {
        edit_hour   = 0U;
        edit_minute = 0U;
        edit_second = 0U;
    }

    /*
     * 第一次创建 Screen
     */
    if (time_setting_screen == NULL)
    {
        time_setting_screen = lv_obj_create(NULL);
    }
    else
    {
        lv_obj_clean(time_setting_screen);
    }


    /* 标题 */
    lv_obj_t *title =
        lv_label_create(time_setting_screen);

    lv_label_set_text(title, "Set Time");

    lv_obj_align(
        title,
        LV_ALIGN_TOP_MID,
        0,
        15
    );


    /* 时间显示 */
    time_label =
        lv_label_create(time_setting_screen);

    TimeSetting_UpdateLabel();

    lv_obj_set_style_text_font(
        time_label,
        &lv_font_montserrat_24,
        0
    );

    lv_obj_align(
        time_label,
        LV_ALIGN_CENTER,
        0,
        -20
    );


    /* Hour - */
    lv_obj_t *minus_button =
        lv_btn_create(time_setting_screen);

    lv_obj_set_size(
        minus_button,
        70,
        45
    );

    lv_obj_align(
        minus_button,
        LV_ALIGN_CENTER,
        -80,
        40
    );

    lv_obj_t *minus_label =
        lv_label_create(minus_button);

    lv_label_set_text(
        minus_label,
        "Hour -"
    );

    lv_obj_center(minus_label);

    lv_obj_add_event_cb(
        minus_button,
        HourMinus_Callback,
        LV_EVENT_CLICKED,
        NULL
    );


    /* Hour + */
    lv_obj_t *plus_button =
        lv_btn_create(time_setting_screen);

    lv_obj_set_size(
        plus_button,
        70,
        45
    );

    lv_obj_align(
        plus_button,
        LV_ALIGN_CENTER,
        80,
        40
    );

    lv_obj_t *plus_label =
        lv_label_create(plus_button);

    lv_label_set_text(
        plus_label,
        "Hour +"
    );

    lv_obj_center(plus_label);

    lv_obj_add_event_cb(
        plus_button,
        HourPlus_Callback,
        LV_EVENT_CLICKED,
        NULL
    );


    /* Save */
    lv_obj_t *save_button =
        lv_btn_create(time_setting_screen);

    lv_obj_set_size(
        save_button,
        100,
        40
    );

    lv_obj_align(
        save_button,
        LV_ALIGN_BOTTOM_MID,
        0,
        -20
    );

    lv_obj_t *save_label =
        lv_label_create(save_button);

    lv_label_set_text(
        save_label,
        "Save"
    );

    lv_obj_center(save_label);

    lv_obj_add_event_cb(
        save_button,
        Save_Callback,
        LV_EVENT_CLICKED,
        NULL
    );
}


/* 返回菜单屏幕指针 */
lv_obj_t *TimeSettingPage_GetScreen(void)
{
    return time_setting_screen;
}
