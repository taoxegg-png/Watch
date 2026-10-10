#include "pedometer.h"

#define PEDOMETER_HIGH_THRESHOLD_MG    120
#define PEDOMETER_LOW_THRESHOLD_MG      60

#define PEDOMETER_MIN_STEP_TIME_MS      250U

typedef enum
{
    PEDOMETER_READY = 0,
    PEDOMETER_WAIT_RELEASE

} PedometerState_t;


static PedometerState_t s_state;

static int32_t s_baseline;

static uint32_t s_steps;

static uint32_t s_last_step_time;

static uint8_t s_initialized;


void Pedometer_Init(void)
{
    s_state = PEDOMETER_READY;

    s_baseline = 0;

    s_steps = 0;

    s_last_step_time = 0;

    s_initialized = 0;
}


uint8_t Pedometer_Update(
    int32_t magnitude_mg,
    uint32_t now_ms)
{
    int32_t dynamic;
    int32_t abs_dynamic;

    /*
     * 第一个采样值直接作为初始基线。
     */
    if (s_initialized == 0U)
    {
        s_baseline = magnitude_mg;

        s_initialized = 1U;

        return 0U;
    }

    /*
     * 慢速跟踪重力/姿态基线。
     *
     * 相当于简单的一阶低通。
     */
    s_baseline +=
        (magnitude_mg - s_baseline) / 32;

    /*
     * 去掉慢变化部分。
     */
    dynamic =
        magnitude_mg - s_baseline;

    /*
     * 求绝对值。
     */
    if (dynamic < 0)
    {
        abs_dynamic = -dynamic;
    }
    else
    {
        abs_dynamic = dynamic;
    }


    switch (s_state)
    {
        case PEDOMETER_READY:

            if (abs_dynamic >=
                PEDOMETER_HIGH_THRESHOLD_MG)
            {
                /*
                 * 防止短时间连续重复计数。
                 */
                if ((now_ms - s_last_step_time) >=
                    PEDOMETER_MIN_STEP_TIME_MS)
                {
                    s_steps++;

                    s_last_step_time = now_ms;

                    s_state =
                        PEDOMETER_WAIT_RELEASE;

                    return 1U;
                }
            }

            break;


        case PEDOMETER_WAIT_RELEASE:

            /*
             * 必须先回到较低区域，
             * 才允许检测下一步。
             */
            if (abs_dynamic <=
                PEDOMETER_LOW_THRESHOLD_MG)
            {
                s_state = PEDOMETER_READY;
            }

            break;


        default:

            s_state = PEDOMETER_READY;

            break;
    }

    return 0U;
}


uint32_t Pedometer_GetSteps(void)
{
    return s_steps;
}
