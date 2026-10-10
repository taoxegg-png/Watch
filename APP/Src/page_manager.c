#include "page_manager.h"

/*
 * 保存各页面对应的 LVGL Screen 指针。
 *
 * 这里保存的是指针，不是复制整个 LVGL 对象。
 */
static lv_obj_t *s_pages[PAGE_COUNT];

/*
 * 当前页面编号。
 *
 * PAGE_COUNT 表示当前尚未选定有效页面。
 */
static PageId_t s_current_page = PAGE_COUNT;


void PageManager_Init(void)
{
    unsigned int i;

    for (i = 0U; i < PAGE_COUNT; i++)
    {
        s_pages[i] = NULL;
    }

    s_current_page = PAGE_COUNT;
}


bool PageManager_Register(
    PageId_t id,
    lv_obj_t *screen)
{
    if (((unsigned int)id >= PAGE_COUNT) ||
        (screen == NULL))
    {
        return false;
    }

    s_pages[id] = screen;

    return true;
}


bool PageManager_Show(PageId_t id)
{
    if ((unsigned int)id >= PAGE_COUNT)
    {
        return false;
    }

    if (s_pages[id] == NULL)
    {
        return false;
    }

    /*
     * 已经显示目标页面时，不重复加载。
     */
    if (s_current_page == id)
    {
        return true;
    }

    lv_scr_load(s_pages[id]);

    s_current_page = id;

    return true;
}


PageId_t PageManager_GetCurrent(void)
{
    return s_current_page;
}
