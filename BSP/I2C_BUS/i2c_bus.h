#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

/* 创建 I2C1 总线互斥锁 */
HAL_StatusTypeDef I2C1_Bus_Init(void);

/* 读取设备寄存器 */
HAL_StatusTypeDef I2C1_Bus_MemRead(
    uint16_t dev_addr,
    uint16_t reg_addr,
    uint16_t reg_addr_size,
    uint8_t *data,
    uint16_t len,
    uint32_t timeout
);

/* 写设备寄存器 */
HAL_StatusTypeDef I2C1_Bus_MemWrite(
    uint16_t dev_addr,
    uint16_t reg_addr,
    uint16_t reg_addr_size,
    uint8_t *data,
    uint16_t len,
    uint32_t timeout
);

#endif
