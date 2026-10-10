#include "watch_rtc.h"

extern RTC_HandleTypeDef hrtc;


HAL_StatusTypeDef WatchRTC_GetTime(
    WatchRTC_Time_t *time)
{
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    HAL_StatusTypeDef status;

    if (time == NULL)
    {
        return HAL_ERROR;
    }

    /*
     * 读取时间
     */
    status = HAL_RTC_GetTime(
        &hrtc,
        &rtc_time,
        RTC_FORMAT_BIN
    );

    if (status != HAL_OK)
    {
        return status;
    }

    /*
     * STM32 RTC：
     * GetTime() 后必须继续 GetDate()
     * 以正确同步 shadow registers。
     */
    status = HAL_RTC_GetDate(
        &hrtc,
        &rtc_date,
        RTC_FORMAT_BIN
    );

    if (status != HAL_OK)
    {
        return status;
    }

    time->hour   = rtc_time.Hours;
    time->minute = rtc_time.Minutes;
    time->second = rtc_time.Seconds;

    return HAL_OK;
}


HAL_StatusTypeDef WatchRTC_SetTime(
    uint8_t hour,
    uint8_t minute,
    uint8_t second)
{
    RTC_TimeTypeDef rtc_time = {0};

    /*
     * 最基本的参数检查
     */
    if ((hour > 23U) ||
        (minute > 59U) ||
        (second > 59U))
    {
        return HAL_ERROR;
    }

    rtc_time.Hours   = hour;
    rtc_time.Minutes = minute;
    rtc_time.Seconds = second;

    rtc_time.DayLightSaving =
        RTC_DAYLIGHTSAVING_NONE;

    rtc_time.StoreOperation =
        RTC_STOREOPERATION_RESET;

    return HAL_RTC_SetTime(
        &hrtc,
        &rtc_time,
        RTC_FORMAT_BIN
    );
}
