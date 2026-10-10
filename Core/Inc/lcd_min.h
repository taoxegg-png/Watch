/* lcd_min.h */
#ifndef __LCD_MIN_H
#define __LCD_MIN_H

#include "main.h"

#define LCD_MIN_WIDTH   240U
#define LCD_MIN_HEIGHT  280U

#define LCD_MIN_RED     0xF800U
#define LCD_MIN_GREEN   0x07E0U
#define LCD_MIN_BLUE    0x001FU
#define LCD_MIN_BLACK   0x0000U
#define LCD_MIN_WHITE   0xFFFFU

extern volatile uint32_t lcd_spi_error_count;

void LCD_MIN_FillRect(
    uint16_t x1,
    uint16_t y1,
    uint16_t x2,
    uint16_t y2,
    uint16_t color
);

void LCD_MIN_DrawArea(
    uint16_t x1,
    uint16_t y1,
    uint16_t x2,
    uint16_t y2,
    const uint16_t *color_p
);

void LCD_MIN_Init(void);
void LCD_MIN_Fill(uint16_t color);

#endif
