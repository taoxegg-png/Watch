#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <stdint.h>

void Pedometer_Init(void);

/*
 * 每得到一个新的 Magnitude 就调用一次。
 *
 * magnitude_mg：三轴加速度模长，单位 mg
 * now_ms：当前时间，单位 ms
 *
 * 返回值：
 * 1 = 本次检测到新的一步
 * 0 = 没检测到
 */
uint8_t Pedometer_Update(
    int32_t magnitude_mg,
    uint32_t now_ms
);

uint32_t Pedometer_GetSteps(void);

#endif
