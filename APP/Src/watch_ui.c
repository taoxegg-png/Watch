#include "watch_ui.h"

#include "lvgl.h"

#include <stdio.h>
#include "page_manager.h"
#include "menu_page.h"
#include "home_page.h"
#include "time_setting_page.h"
/* =========================================================
 * Private UI Objects
 * =========================================================
 */

void WatchUI_Init(void)
{
    /* 先创建主页 */
    HomePage_Init();

    /* 再创建菜单页 */
    MenuPage_Init();
	
	TimeSettingPage_Init();

    /* 向页面管理器注册两个页面 */
    PageManager_Register(
        PAGE_HOME,
        HomePage_GetScreen()
    );

    PageManager_Register(
        PAGE_MENU,
        MenuPage_GetScreen()
    );
	
	PageManager_Register(
        PAGE_TIME_SETTING,
        TimeSettingPage_GetScreen()
    );

    /* 启动后显示主页 */
    PageManager_Show(PAGE_HOME);
}


void WatchUI_Update(const WatchData_t *data)
{
    HomePage_Update(data);
}
