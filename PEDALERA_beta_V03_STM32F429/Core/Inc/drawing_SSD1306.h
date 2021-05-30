#ifndef DRAWING_SSD1306_H
#define DRAWING_SSD1306_H

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "stm32f4xx_hal.h"
#include "ssd1306.h"


void SSD1306_DrawPicture_custom(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t picture[]);
void SSD1306_DrawPicture_16x16(uint8_t x0, uint8_t y0, uint8_t picture[]);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
