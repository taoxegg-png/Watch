#ifndef WATCH_RTC_H
#define WATCH_RTC_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} WatchRTC_Time_t;


/* 读取当前 RTC 时间 */
HAL_StatusTypeDef WatchRTC_GetTime(
    WatchRTC_Time_t *time
);

/* 设置 RTC 时间 */
HAL_StatusTypeDef WatchRTC_SetTime(
    uint8_t hour,
    uint8_t minute,
    uint8_t second
);

#endif
