#include "touch.h"
#include "i2c_bus.h"

/* CST816T 的 7-bit I2C 地址 */
#define TOUCH_I2C_ADDR          0x15U

/* CST816T 寄存器 */
#define TOUCH_REG_FINGER        0x02U
#define TOUCH_REG_CHIP_ID       0xA7U


HAL_StatusTypeDef Touch_ReadChipID(uint8_t *chip_id)
{
    if (chip_id == NULL)
    {
        return HAL_ERROR;
    }

    return I2C1_Bus_MemRead(
        TOUCH_I2C_ADDR << 1,
        TOUCH_REG_CHIP_ID,
        I2C_MEMADD_SIZE_8BIT,
        chip_id,
        1,
        100
    );
}


HAL_StatusTypeDef Touch_ReadPoint(TouchPoint_t *point)
{
    uint8_t buf[5];
    HAL_StatusTypeDef status;

    if (point == NULL)
    {
        return HAL_ERROR;
    }

    /*
     * 从 0x02 开始连续读取 5 个字节：
     *
     * 0x02 FingerNum
     * 0x03 XH
     * 0x04 XL
     * 0x05 YH
     * 0x06 YL
     */
    status = I2C1_Bus_MemRead(
        TOUCH_I2C_ADDR << 1,
        TOUCH_REG_FINGER,
        I2C_MEMADD_SIZE_8BIT,
        buf,
        sizeof(buf),
        100
    );

    if (status != HAL_OK)
    {
        return status;
    }

    /*
     * 没有触摸点
     */
    if ((buf[0] & 0x0FU) == 0U)
    {
        point->pressed = 0U;

        return HAL_OK;
    }

    /*
     * 检测到触摸
     */
    point->pressed = 1U;

    /*
     * buf[1] = 0x03
     * Bit7:6 保存 Event
     */
    point->event =
        (buf[1] >> 6) & 0x03U;

    /*
     * X 坐标：
     * 0x03低4位 = X[11:8]
     * 0x04       = X[7:0]
     */
    point->x =
        ((uint16_t)(buf[1] & 0x0FU) << 8)
        |
        buf[2];

    /*
     * Y 坐标：
     * 0x05低4位 = Y[11:8]
     * 0x06       = Y[7:0]
     */
    point->y =
        ((uint16_t)(buf[3] & 0x0FU) << 8)
        |
        buf[4];

    return HAL_OK;
}
