#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include "lvgl.h"

/* 创建菜单页面及其控件 */
void MenuPage_Init(void);

/* 获取菜单页面对应的 LVGL Screen */
lv_obj_t *MenuPage_GetScreen(void);

#endif
