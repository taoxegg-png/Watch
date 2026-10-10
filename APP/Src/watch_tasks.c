#include "watch_tasks.h"

#include "main.h"
#include "cmsis_os.h"

#include "watch_data.h"
#include "lvgl.h"

#include <stdio.h>

#include "watch_ui.h"
#include "touch.h"
#include "mpu6050.h"
#include "pedometer.h"
#include "watch_rtc.h"
#include "watch_data.h"
/*
 * huart1 当前仍然由 CubeMX main.c 创建。
 *
 * watch_tasks.c 中的 PrintTask 需要使用它，
 * 所以暂时使用 extern。
 *
 * 后续我们还会继续整理硬件层。
 */
extern UART_HandleTypeDef huart1;

/* =========================================================
 * Task Handles
 * =========================================================
 */

static osThreadId_t sensorTaskHandle;
static osThreadId_t printTaskHandle;
static osThreadId_t uiTaskHandle;
static osThreadId_t systemTaskHandle;
/* =========================================================
 * Task Attributes
 * =========================================================
 */

static const osThreadAttr_t sensorTask_attributes =
    {
        .name = "SensorTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityNormal};

static const osThreadAttr_t printTask_attributes =
    {
        .name = "PrintTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t)osPriorityNormal};

static const osThreadAttr_t uiTask_attributes =
    {
        .name = "UITask",
        .stack_size = 4096,
        .priority = (osPriority_t)osPriorityNormal};
	
static const osThreadAttr_t systemTask_attributes =
{
    .name = "SystemTask",
    .stack_size = 512,
    .priority = (osPriority_t)osPriorityBelowNormal
};
/* =========================================================
 * Private Function Prototypes
 * =========================================================
 */

static void StartSensorTask(void *argument);
static void StartPrintTask(void *argument);
static void StartUITask(void *argument);
static void StartSystemTask(void *argument);

/* =========================================================
 * Public Interface
 * =========================================================
 */

void WatchTasks_Init(void)
{
    sensorTaskHandle = osThreadNew(
        StartSensorTask,
        NULL,
        &sensorTask_attributes);

    if (sensorTaskHandle == NULL)
    {
        Error_Handler();
    }

    printTaskHandle = osThreadNew(
        StartPrintTask,
        NULL,
        &printTask_attributes);

    if (printTaskHandle == NULL)
    {
        Error_Handler();
    }

    uiTaskHandle = osThreadNew(
        StartUITask,
        NULL,
        &uiTask_attributes);

    if (uiTaskHandle == NULL)
    {
        Error_Handler();
    }
	
	systemTaskHandle = osThreadNew(
    StartSystemTask,
    NULL,
    &systemTask_attributes
);

if (systemTaskHandle == NULL)
{
    Error_Handler();
}
}

/* =========================================================
 * Sensor Task
 * =========================================================
 */

static void StartSensorTask(void *argument)
{
    MPU6050_AccelMg_t accel;
    int32_t magnitude;

    (void)argument;

    Pedometer_Init();

    if (MPU6050_Init() != HAL_OK)
    {
        printf("MPU6050 Init Failed\r\n");

        for (;;)
        {
            osDelay(1000);
        }
    }

    printf("MPU6050 Init OK\r\n");

    for (;;)
    {
        if (MPU6050_ReadAccelMg(&accel) == HAL_OK)
        {
            magnitude =
                MPU6050_CalcMagnitudeMg(&accel);

            if (Pedometer_Update(
                    magnitude,
                    HAL_GetTick()) != 0U)
            {
                WatchData_SetSteps(
                    Pedometer_GetSteps()
                );
            }
        }

        osDelay(20);
    }
}

/* =========================================================
 * Print Task
 * =========================================================
 */

static void StartPrintTask(void *argument)
{
    (void)argument;

    WatchData_t data;

    char buf[128];

    for (;;)
    {
        /*
         * 获取当前状态快照
         */
        WatchData_Get(&data);

        int len = snprintf(
            buf,
            sizeof(buf),
            "%02d:%02d:%02d  Battery:%d%%  Steps:%lu  HR:%d\r\n",
            data.hour,
            data.minute,
            data.second,
            data.battery,
            (unsigned long)data.steps,
            data.heart_rate);

        HAL_UART_Transmit(
            &huart1,
            (uint8_t *)buf,
            len,
            HAL_MAX_DELAY);

        osDelay(1000);
    }
}

/* =========================================================
 * UI Task
 * =========================================================
 */

static void StartUITask(void *argument)
{
    (void)argument;

    WatchData_t data;

    uint32_t last_tick = HAL_GetTick();
    uint32_t last_ui_update = HAL_GetTick();

    for (;;)
    {
        uint32_t now = HAL_GetTick();

        /*
         * LVGL 时间基准
         */
        uint32_t elapsed = now - last_tick;

        last_tick = now;

        if (elapsed > 0U)
        {
            lv_tick_inc(elapsed);
        }

        /*
         * 应用数据不需要每次 LVGL Handler
         * 都重新读取。
         *
         * 当前先设成 100ms。
         */
        if ((now - last_ui_update) >= 100U)
        {
            last_ui_update = now;

            WatchData_Get(&data);

            WatchUI_Update(&data);
        }

        /*
         * LVGL 主处理函数
         */
        lv_timer_handler();

        osDelay(5);
    }
}

static void StartSystemTask(void *argument)
{
    WatchRTC_Time_t rtc_time;

    (void)argument;

    for (;;)
    {
        if (WatchRTC_GetTime(&rtc_time) == HAL_OK)
        {
            WatchData_SetTime(
                rtc_time.hour,
                rtc_time.minute,
                rtc_time.second
            );
			printf(
    "RTC raw: H=%u M=%u S=%u\r\n",
    rtc_time.hour,
    rtc_time.minute,
    rtc_time.second
);
        }

        /*
         * RTC只需要大约每秒读取一次。
         */
        osDelay(1000);
    }
}
