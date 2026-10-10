#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <stdbool.h>
#include "lvgl.h"

/*
 * 页面编号。
 *
 * 以后增加页面时，在这里扩展。
 */
typedef enum
{
    PAGE_HOME = 0,
    PAGE_MENU,
    PAGE_TIME_SETTING,

    PAGE_COUNT
} PageId_t;


/* 初始化页面管理器 */
void PageManager_Init(void);

/* 注册页面对象 */
bool PageManager_Register(
    PageId_t id,
    lv_obj_t *screen
);

/* 切换到指定页面 */
bool PageManager_Show(PageId_t id);

/* 获取当前页面编号 */
PageId_t PageManager_GetCurrent(void);

#endif
