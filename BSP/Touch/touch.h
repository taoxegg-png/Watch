#ifndef __TOUCH_H
#define __TOUCH_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

/* 一次触摸采样的结果 */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint8_t pressed;
    uint8_t event;
} TouchPoint_t;


/* 读取芯片 ID */
HAL_StatusTypeDef Touch_ReadChipID(uint8_t *chip_id);

/* 读取当前触摸状态和原始坐标 */
HAL_StatusTypeDef Touch_ReadPoint(TouchPoint_t *point);

#endif
