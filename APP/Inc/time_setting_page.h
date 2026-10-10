#ifndef TIME_SETTING_PAGE_H
#define TIME_SETTING_PAGE_H

#include "lvgl.h"

/* 创建菜单页面及其控件 */
void TimeSettingPage_Init(void);

/* 获取菜单页面对应的 LVGL Screen */
lv_obj_t *TimeSettingPage_GetScreen(void);

#endif
