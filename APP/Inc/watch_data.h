#ifndef __WATCH_DATA_H
#define __WATCH_DATA_H

#include <stdint.h>

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t battery;

    uint32_t steps;

    uint8_t heart_rate;

} WatchData_t;

/* 初始化手表数据服务 */
void WatchData_Init(void);

/* 获取当前数据快照 */
void WatchData_Get(WatchData_t *data);

void WatchData_SetSteps(uint32_t steps);

void WatchData_SetTime(
    uint8_t hour,
    uint8_t minute,
    uint8_t second
);

#endif
