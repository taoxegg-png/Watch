#include "watch_data.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <string.h>

static WatchData_t g_watch_data;

static SemaphoreHandle_t watch_data_mutex = NULL;

void WatchData_Init(void)
{
    memset(&g_watch_data, 0, sizeof(g_watch_data));

    watch_data_mutex = xSemaphoreCreateMutex();
}

void WatchData_Get(WatchData_t *data)
{
    if ((data == NULL) || (watch_data_mutex == NULL))
    {
        return;
    }

    if (xSemaphoreTake(watch_data_mutex, portMAX_DELAY) == pdTRUE)
    {
        *data = g_watch_data;

        xSemaphoreGive(watch_data_mutex);
    }
}

void WatchData_SetSteps(uint32_t steps)
{
    if (watch_data_mutex == NULL)
    {
        return;
    }

    if (xSemaphoreTake(
            watch_data_mutex,
            portMAX_DELAY) == pdTRUE)
    {
        g_watch_data.steps = steps;

        xSemaphoreGive(watch_data_mutex);
    }
}

void WatchData_SetTime(
    uint8_t hour,
    uint8_t minute,
    uint8_t second)
{
    if (watch_data_mutex == NULL)
    {
        return;
    }

    if (xSemaphoreTake(
            watch_data_mutex,
            portMAX_DELAY) == pdTRUE)
    {
        g_watch_data.hour   = hour;
        g_watch_data.minute = minute;
        g_watch_data.second = second;

        xSemaphoreGive(watch_data_mutex);
    }
}
