#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include "lvgl.h"
#include "watch_data.h"

/* 创建主页及其控件 */
void HomePage_Init(void);

/* 获取主页对应的 LVGL Screen */
lv_obj_t *HomePage_GetScreen(void);

/* 更新主页上显示的手表数据 */
void HomePage_Update(const WatchData_t *data);

#endif
