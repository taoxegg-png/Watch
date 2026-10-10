#include "watch_display.h"
#include "lvgl.h"
#include "lcd_min.h"

#define WATCH_HOR_RES 240
#define WATCH_VER_RES 280

/* 只分配 20 行的绘图缓冲区 */
static lv_color_t lv_buf1[WATCH_HOR_RES * 20];

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

/* LVGL 画完一块区域后，会调用这个函数 */
static void my_disp_flush(
    lv_disp_drv_t *disp_drv,
    const lv_area_t *area,
    lv_color_t *color_p)
{
    LCD_MIN_DrawArea(
        (uint16_t)area->x1,
        (uint16_t)area->y1,
        (uint16_t)area->x2,
        (uint16_t)area->y2,
        (const uint16_t *)color_p
    );

    /* 阻塞式 SPI 发送已完成，可以归还绘图缓冲区 */
    lv_disp_flush_ready(disp_drv);
}

void Watch_Display_Init(void)
{
    lv_disp_draw_buf_init(
        &draw_buf,
        lv_buf1,
        NULL,
        WATCH_HOR_RES * 20
    );

    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = WATCH_HOR_RES;
    disp_drv.ver_res = WATCH_VER_RES;

    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = my_disp_flush;

    lv_disp_drv_register(&disp_drv);
}
