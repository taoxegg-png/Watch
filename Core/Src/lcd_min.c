/* lcd_min.c */

#include "lcd_min.h"

/*
 * SPI1 句柄由 CubeMX 生成。
 * 如果 CubeMX 将 SPI 代码放在 main.c 中，
 * 这里使用 extern 引用同一个 hspi1。
 */
extern SPI_HandleTypeDef hspi1;

/* 在 Keil Watch 中可查看 SPI 发送失败次数 */
volatile uint32_t lcd_spi_error_count = 0;

/* ---------- LCD 控制引脚 ---------- */

static void LCD_CS_Low(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

static void LCD_CS_High(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}

static void LCD_DC_Command(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
}

static void LCD_DC_Data(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
}

static void LCD_RST_Low(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

static void LCD_RST_High(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

/* ---------- SPI 发送 ---------- */

static void LCD_SPI_Send(const uint8_t *buf, uint16_t len)
{
    if (HAL_SPI_Transmit(
            &hspi1,
            (uint8_t *)buf,
            len,
            1000) != HAL_OK)
    {
        lcd_spi_error_count++;
    }
}

/*
 * 发命令时 DC = 0
 * 发数据时 DC = 1
 *
 * CS 在一次完整的 LCD 操作期间保持低电平。
 */

static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_DC_Command();
    LCD_SPI_Send(&cmd, 1);
    LCD_DC_Data();
}

static void LCD_WriteData(const uint8_t *data, uint16_t len)
{
    LCD_DC_Data();
    LCD_SPI_Send(data, len);
}

static void LCD_WriteCommandData(
    uint8_t cmd,
    const uint8_t *data,
    uint16_t len)
{
    LCD_WriteCommand(cmd);

    if ((data != NULL) && (len > 0))
    {
        LCD_WriteData(data, len);
    }
}

/* ---------- 设置 LCD 显示区域 ---------- */

static void LCD_SetAddress(
    uint16_t x1,
    uint16_t y1,
    uint16_t x2,
    uint16_t y2)
{
    uint8_t addr[4];

    /*
     * 参考开源驱动：
     * USE_HORIZONTAL = 1 时，Y 方向偏移 20。
     */
    y1 += 20U;
    y2 += 20U;

    /* 0x2A：设置列地址 */
    addr[0] = (uint8_t)(x1 >> 8);
    addr[1] = (uint8_t)x1;
    addr[2] = (uint8_t)(x2 >> 8);
    addr[3] = (uint8_t)x2;

    LCD_WriteCommandData(0x2A, addr, 4);

    /* 0x2B：设置行地址 */
    addr[0] = (uint8_t)(y1 >> 8);
    addr[1] = (uint8_t)y1;
    addr[2] = (uint8_t)(y2 >> 8);
    addr[3] = (uint8_t)y2;

    LCD_WriteCommandData(0x2B, addr, 4);

    /* 0x2C：开始写入显存 */
    LCD_WriteCommand(0x2C);
}

/* ---------- ST7789 初始化 ---------- */

void LCD_MIN_Init(void)
{
    static const uint8_t porch[] = {
        0x0C, 0x0C, 0x00, 0x33, 0x33
    };

    static const uint8_t power[] = {
        0xA4, 0xA1
    };

    static const uint8_t gamma_pos[] = {
        0xD0, 0x04, 0x0D, 0x11,
        0x13, 0x2B, 0x3F, 0x54,
        0x4C, 0x18, 0x0D, 0x0B,
        0x1F, 0x23
    };

    static const uint8_t gamma_neg[] = {
        0xD0, 0x04, 0x0C, 0x11,
        0x13, 0x2C, 0x3F, 0x44,
        0x51, 0x2F, 0x1F, 0x1F,
        0x20, 0x23
    };

    uint8_t value;

    /* 初始化时先关闭背光 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

    LCD_CS_High();
    LCD_DC_Data();

    /* 硬件复位 */
    LCD_RST_Low();
    HAL_Delay(100);

    LCD_RST_High();
    HAL_Delay(100);

    /* 选中 LCD */
    LCD_CS_Low();

    /* 退出休眠 */
    LCD_WriteCommand(0x11);
    HAL_Delay(120);

    /* 显示方向：对应原驱动 USE_HORIZONTAL = 1 */
    value = 0xC0;
    LCD_WriteCommandData(0x36, &value, 1);

    /* RGB565：16 位像素 */
    value = 0x05;
    LCD_WriteCommandData(0x3A, &value, 1);

    LCD_WriteCommandData(0xB2, porch, sizeof(porch));

    value = 0x35;
    LCD_WriteCommandData(0xB7, &value, 1);

    value = 0x19;
    LCD_WriteCommandData(0xBB, &value, 1);

    value = 0x2C;
    LCD_WriteCommandData(0xC0, &value, 1);

    value = 0x01;
    LCD_WriteCommandData(0xC2, &value, 1);

    value = 0x12;
    LCD_WriteCommandData(0xC3, &value, 1);

    value = 0x20;
    LCD_WriteCommandData(0xC4, &value, 1);

    value = 0x0F;
    LCD_WriteCommandData(0xC6, &value, 1);

    LCD_WriteCommandData(0xD0, power, sizeof(power));

    LCD_WriteCommandData(
        0xE0,
        gamma_pos,
        sizeof(gamma_pos));

    LCD_WriteCommandData(
        0xE1,
        gamma_neg,
        sizeof(gamma_neg));

    /* 显示反转：与原项目初始化一致 */
    LCD_WriteCommand(0x21);

    /* 开启显示 */
    LCD_WriteCommand(0x29);

    HAL_Delay(20);

    LCD_CS_High();
}

/* ---------- 整屏填充 RGB565 颜色 ---------- */

void LCD_MIN_Fill(uint16_t color)
{
    /*
     * 一行 240 个像素，每个像素 2 字节。
     *
     * 不申请整屏显存，只准备一行的数据。
     */
    static uint8_t line[LCD_MIN_WIDTH * 2U];

    uint16_t x;
    uint16_t y;

    uint8_t high = (uint8_t)(color >> 8);
    uint8_t low  = (uint8_t)(color & 0xFFU);

    /* 生成一整行的颜色数据 */
    for (x = 0; x < LCD_MIN_WIDTH; x++)
    {
        line[x * 2U]      = high;
        line[x * 2U + 1U] = low;
    }

    LCD_CS_Low();

    /* 设置整个可见区域：x 0~239，y 0~279 */
    LCD_SetAddress(
        0,
        0,
        LCD_MIN_WIDTH - 1U,
        LCD_MIN_HEIGHT - 1U);

    /* 连续写入 280 行 */
    for (y = 0; y < LCD_MIN_HEIGHT; y++)
    {
        LCD_WriteData(line, sizeof(line));
    }

    LCD_CS_High();
}

void LCD_MIN_FillRect(
    uint16_t x1,
    uint16_t y1,
    uint16_t x2,
    uint16_t y2,
    uint16_t color)
{
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;

    uint8_t high;
    uint8_t low;

    static uint8_t line[LCD_MIN_WIDTH * 2U];

    /* 防止坐标错误 */
    if ((x1 > x2) || (y1 > y2))
    {
        return;
    }

    if ((x2 >= LCD_MIN_WIDTH) ||
        (y2 >= LCD_MIN_HEIGHT))
    {
        return;
    }

    width  = x2 - x1 + 1U;
    height = y2 - y1 + 1U;

    high = (uint8_t)(color >> 8);
    low  = (uint8_t)(color & 0xFFU);

    /*
     * 先生成“一行”的像素数据
     */
    for (x = 0; x < width; x++)
    {
        line[x * 2U]      = high;
        line[x * 2U + 1U] = low;
    }

    LCD_CS_Low();

    /*
     * 告诉 ST7789：
     * 接下来只操作这个矩形区域
     */
    LCD_SetAddress(x1, y1, x2, y2);

    /*
     * 一行一行写进去
     */
    for (y = 0; y < height; y++)
    {
        LCD_WriteData(
            line,
            width * 2U
        );
    }

    LCD_CS_High();
}

void LCD_MIN_DrawArea(
    uint16_t x1,
    uint16_t y1,
    uint16_t x2,
    uint16_t y2,
    const uint16_t *color_p)
{
    uint32_t pixel_count;
    uint32_t i;

    uint8_t pixel[2];

    if (color_p == NULL)
    {
        return;
    }

    if ((x1 > x2) || (y1 > y2))
    {
        return;
    }

    if ((x2 >= LCD_MIN_WIDTH) ||
        (y2 >= LCD_MIN_HEIGHT))
    {
        return;
    }

    pixel_count =
        (uint32_t)(x2 - x1 + 1U) *
        (uint32_t)(y2 - y1 + 1U);

    LCD_CS_Low();

    LCD_SetAddress(
        x1,
        y1,
        x2,
        y2
    );

    for (i = 0; i < pixel_count; i++)
    {
        uint16_t color = color_p[i];

        pixel[0] = (uint8_t)(color >> 8);
        pixel[1] = (uint8_t)(color & 0xFF);

        LCD_WriteData(pixel, 2);
    }

    LCD_CS_High();
}
