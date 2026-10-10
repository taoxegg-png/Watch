#include "watch_app.h"

#include "lvgl.h"

#include "watch_display.h"
#include "watch_data.h"
#include "watch_ui.h"
#include "watch_tasks.h"
#include "watch_touch.h"
#include "page_manager.h"
#include "i2c_bus.h"

void WatchApp_Init(void)
{
    /* LVGL 核心 */
    lv_init();
	
	I2C1_Bus_Init();

    /* 显示与触摸适配 */
    Watch_Display_Init();
    WatchTouch_Init();

    /* 应用状态 */
    WatchData_Init();

    /* 页面管理器 */
    PageManager_Init();

    /* 创建并注册页面 */
    WatchUI_Init();

    /* 创建业务任务 */
    WatchTasks_Init();
}
