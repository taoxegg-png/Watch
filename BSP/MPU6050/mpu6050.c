#include "mpu6050.h"
#include <math.h>
#include "i2c_bus.h"

#define MPU6050_I2C_ADDR             0x68U

#define MPU6050_REG_PWR_MGMT_1       0x6BU
#define MPU6050_REG_WHO_AM_I         0x75U
#define MPU6050_REG_ACCEL_XOUT_H     0x3BU
#define MPU6050_REG_ACCEL_CONFIG     0x1CU
#define MPU6050_ACCEL_SENS_2G        16384L

static HAL_StatusTypeDef MPU6050_ReadReg(
    uint8_t reg,
    uint8_t *data)
{
    if (data == NULL)
    {
        return HAL_ERROR;
    }

    return I2C1_Bus_MemRead(
        MPU6050_I2C_ADDR << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        1,
        100
    );
}

static HAL_StatusTypeDef MPU6050_WriteReg(
    uint8_t reg,
    uint8_t data)
{
    return I2C1_Bus_MemWrite(
        MPU6050_I2C_ADDR << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        &data,
        1,
        100
    );
}

HAL_StatusTypeDef MPU6050_ReadWhoAmI(uint8_t *who_am_i)
{
    return MPU6050_ReadReg(
        MPU6050_REG_WHO_AM_I,
        who_am_i
    );
}

HAL_StatusTypeDef MPU6050_ReadPowerManagement(uint8_t *value)
{
    return MPU6050_ReadReg(
        MPU6050_REG_PWR_MGMT_1,
        value
    );
}

HAL_StatusTypeDef MPU6050_WakeUp(void)
{
    uint8_t value;

    HAL_StatusTypeDef status;

    /* 1. 先读取原来的 PWR_MGMT_1 */
    status = MPU6050_ReadReg(
        MPU6050_REG_PWR_MGMT_1,
        &value
    );

    if (status != HAL_OK)
    {
        return status;
    }

    /* 2. 清除 SLEEP 位 Bit6 */
    value &= ~(1U << 6);

    /* 3. 写回寄存器 */
    status = MPU6050_WriteReg(
        MPU6050_REG_PWR_MGMT_1,
        value
    );

    return status;
}

static HAL_StatusTypeDef MPU6050_ReadRegs(
    uint8_t start_reg,
    uint8_t *data,
    uint16_t len)
{
    if ((data == NULL) || (len == 0U))
    {
        return HAL_ERROR;
    }

    return I2C1_Bus_MemRead(
        MPU6050_I2C_ADDR << 1,
        start_reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        len,
        100
    );
}

HAL_StatusTypeDef MPU6050_ReadAccelRaw(
    MPU6050_AccelRaw_t *accel)
{
    uint8_t buf[6];
    HAL_StatusTypeDef status;

    if (accel == NULL)
    {
        return HAL_ERROR;
    }

    status = MPU6050_ReadRegs(
        MPU6050_REG_ACCEL_XOUT_H,
        buf,
        6
    );

    if (status != HAL_OK)
    {
        return status;
    }

    accel->x = (int16_t)(
        ((uint16_t)buf[0] << 8) |
        buf[1]
    );

    accel->y = (int16_t)(
        ((uint16_t)buf[2] << 8) |
        buf[3]
    );

    accel->z = (int16_t)(
        ((uint16_t)buf[4] << 8) |
        buf[5]
    );

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadAccelConfig(uint8_t *value)
{
    return MPU6050_ReadReg(
        MPU6050_REG_ACCEL_CONFIG,
        value
    );
}

HAL_StatusTypeDef MPU6050_ReadAccelMg(
    MPU6050_AccelMg_t *accel)
{
    MPU6050_AccelRaw_t raw;
    HAL_StatusTypeDef status;

    if (accel == NULL)
    {
        return HAL_ERROR;
    }

    status = MPU6050_ReadAccelRaw(&raw);

    if (status != HAL_OK)
    {
        return status;
    }

    accel->x =
        ((int32_t)raw.x * 1000L) /
        MPU6050_ACCEL_SENS_2G;

    accel->y =
        ((int32_t)raw.y * 1000L) /
        MPU6050_ACCEL_SENS_2G;

    accel->z =
        ((int32_t)raw.z * 1000L) /
        MPU6050_ACCEL_SENS_2G;

    return HAL_OK;
}

int32_t MPU6050_CalcMagnitudeMg(
    const MPU6050_AccelMg_t *accel)
{
    float x;
    float y;
    float z;
    float magnitude;

    if (accel == NULL)
    {
        return 0;
    }

    x = (float)accel->x;
    y = (float)accel->y;
    z = (float)accel->z;

    magnitude = sqrtf(
        x * x +
        y * y +
        z * z
    );

    return (int32_t)magnitude;
}

HAL_StatusTypeDef MPU6050_Init(void)
{
    uint8_t id;
    HAL_StatusTypeDef status;

    /* 1. 确认设备身份 */
    status = MPU6050_ReadWhoAmI(&id);

    if (status != HAL_OK)
    {
        return status;
    }

    if (id != 0x68U)
    {
        return HAL_ERROR;
    }

    /* 2. 退出 Sleep */
    status = MPU6050_WakeUp();

    if (status != HAL_OK)
    {
        return status;
    }

    HAL_Delay(10);

    return HAL_OK;
}
