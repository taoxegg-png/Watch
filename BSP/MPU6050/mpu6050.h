#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;

} MPU6050_AccelRaw_t;

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;

} MPU6050_AccelMg_t;

/* 读取 MPU6050 的 WHO_AM_I 寄存器 */
HAL_StatusTypeDef MPU6050_ReadWhoAmI(uint8_t *who_am_i);

HAL_StatusTypeDef MPU6050_ReadPowerManagement(uint8_t *value);

HAL_StatusTypeDef MPU6050_WakeUp(void);

HAL_StatusTypeDef MPU6050_ReadAccelRaw(MPU6050_AccelRaw_t *accel);

HAL_StatusTypeDef MPU6050_ReadAccelConfig(uint8_t *value);

HAL_StatusTypeDef MPU6050_ReadAccelMg(MPU6050_AccelMg_t *accel);

int32_t MPU6050_CalcMagnitudeMg(const MPU6050_AccelMg_t *accel);

HAL_StatusTypeDef MPU6050_Init(void);

#endif
