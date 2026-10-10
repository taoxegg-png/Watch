#include "watch_touch.h"

#include "lvgl.h"
#include "touch.h"

#define WATCH_LCD_WIDTH   240U
#define WATCH_LCD_HEIGHT  280U

static lv_indev_drv_t touch_indev_drv;
static lv_indev_t *touch_indev;

/*
 * 把原始触摸坐标转换成
 * 当前 LVGL 屏幕坐标。
 *
 * 先按两个坐标轴反向处理。
 */
static void WatchTouch_MapCoordinate(
    uint16_t raw_x,
    uint16_t raw_y,
    lv_point_t *point)
{
    int32_t x;
    int32_t y;

    x = (int32_t)(WATCH_LCD_WIDTH - 1U) - raw_x;
    y = (int32_t)(WATCH_LCD_HEIGHT - 1U) - raw_y;

    /*
     * 初步限制在 LVGL 屏幕坐标范围内。
     * 后续根据实测结果完善校准。
     */
    if (x < 0)
    {
        x = 0;
    }
    else if (x >= WATCH_LCD_WIDTH)
    {
        x = WATCH_LCD_WIDTH - 1U;
    }

    if (y < 0)
    {
        y = 0;
    }
    else if (y >= WATCH_LCD_HEIGHT)
    {
        y = WATCH_LCD_HEIGHT - 1U;
    }

    point->x = (lv_coord_t)x;
    point->y = (lv_coord_t)y;
}


/*
 * LVGL 调用的触摸读取回调。
 */
static void touch_read_cb(
    lv_indev_drv_t *drv,
    lv_indev_data_t *data)
{
    static lv_point_t last_point = {0, 0};

    TouchPoint_t raw_point;

    (void)drv;

    if (Touch_ReadPoint(&raw_point) == HAL_OK)
    {
        if (raw_point.pressed)
        {
            WatchTouch_MapCoordinate(
                raw_point.x,
                raw_point.y,
                &last_point
            );

            data->point = last_point;
            data->state = LV_INDEV_STATE_PRESSED;

            return;
        }
    }

    /*
     * 没有检测到触摸，或者本次读取失败：
     * 先按释放处理，避免界面一直处于按下状态。
     *
     * 保留最后一次有效坐标。
     */
    data->point = last_point;
    data->state = LV_INDEV_STATE_RELEASED;
}


/*
 * 注册 LVGL 输入设备。
 */
void WatchTouch_Init(void)
{
    lv_indev_drv_init(&touch_indev_drv);

    touch_indev_drv.type = LV_INDEV_TYPE_POINTER;

    touch_indev_drv.read_cb = touch_read_cb;

    touch_indev = lv_indev_drv_register(
        &touch_indev_drv
    );

    (void)touch_indev;
}
