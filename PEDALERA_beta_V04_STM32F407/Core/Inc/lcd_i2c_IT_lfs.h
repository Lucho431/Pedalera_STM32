/*
 * lcd_i2c_lfs_IT.h
 *
 *  Created on: Sep 2, 2020
 *      Author: Luciano Salvatore
 */

#ifndef LCD_I2C_LFS_H_
#define LCD_I2C_LFS_H_


#include "stm32f4xx_hal.h"

#define LCD_DDRAM_ADDRESS	  0x80 // 0b10000000
#define LCD_START_LINE1 	  0x00
#define LCD_START_LINE2 	  0x40
#define LCD_START_LINE3 	  0x14
#define LCD_START_LINE4 	  0x54


//Public functions:

void lcd_init (I2C_HandleTypeDef*,uint8_t);   // initialize lcd
void lcd_cursor_buff(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
void lcd_string_buff (char *str);  // send string to the lcd
void lcd_clearScreen_buff (void);
void lcd_upDate_IT (void);

/* Insert the next function in your ISR*/
void _lcd_ISR (void);


//Private functions:

void __lcd_send_cmd (char cmd);  // send command to the lcd
void __lcd_send_cmd_buff (char cmd);  // send command to the lcd
void __lcd_send_data_buff (char data);  // send data to the lcd
void __add_to_lcdBuff (uint8_t *, uint8_t);
void __lcd_sendBuff_IT (void);


#endif /* LCD_I2C_LFS_H_ */
