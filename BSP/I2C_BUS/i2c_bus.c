#include "i2c_bus.h"

#include "FreeRTOS.h"
#include "semphr.h"

extern I2C_HandleTypeDef hi2c1;

static SemaphoreHandle_t s_i2c1_mutex = NULL;

HAL_StatusTypeDef I2C1_Bus_Init(void)
{
    s_i2c1_mutex = xSemaphoreCreateMutex();

    if (s_i2c1_mutex == NULL)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef I2C1_Bus_MemRead(
    uint16_t dev_addr,
    uint16_t reg_addr,
    uint16_t reg_addr_size,
    uint8_t *data,
    uint16_t len,
    uint32_t timeout)
{
    HAL_StatusTypeDef status;

    if ((s_i2c1_mutex == NULL) ||
        (data == NULL) ||
        (len == 0U))
    {
        return HAL_ERROR;
    }

    /*
     * 等待获得 I2C1 使用权
     */
    if (xSemaphoreTake(
            s_i2c1_mutex,
            portMAX_DELAY) != pdTRUE)
    {
        return HAL_ERROR;
    }

    /*
     * 现在只有当前 Task 可以操作 I2C1
     */
    status = HAL_I2C_Mem_Read(
        &hi2c1,
        dev_addr,
        reg_addr,
        reg_addr_size,
        data,
        len,
        timeout
    );

    /*
     * 无论 HAL_OK 还是 HAL_ERROR，
     * 都必须释放 Mutex。
     */
    xSemaphoreGive(s_i2c1_mutex);

    return status;
}

HAL_StatusTypeDef I2C1_Bus_MemWrite(
    uint16_t dev_addr,
    uint16_t reg_addr,
    uint16_t reg_addr_size,
    uint8_t *data,
    uint16_t len,
    uint32_t timeout)
{
    HAL_StatusTypeDef status;

    if ((s_i2c1_mutex == NULL) ||
        (data == NULL) ||
        (len == 0U))
    {
        return HAL_ERROR;
    }

    if (xSemaphoreTake(
            s_i2c1_mutex,
            portMAX_DELAY) != pdTRUE)
    {
        return HAL_ERROR;
    }

    status = HAL_I2C_Mem_Write(
        &hi2c1,
        dev_addr,
        reg_addr,
        reg_addr_size,
        data,
        len,
        timeout
    );

    xSemaphoreGive(s_i2c1_mutex);

    return status;
}
