/*
 * lcd_i2c_IT_lfs.c
 *
 *  Created on: Sep 2, 2020
 *      Author: Luciano Salvatore
 */

/** Put this in the src folder **/

#include <lcd_i2c_IT_lfs.h>

#ifndef LCD_CIRCBUFF_MAX
#define LCD_CIRCBUFF_MAX 680
#endif

I2C_HandleTypeDef* i2c_handler;  // change your handler here accordingly
uint8_t SLAVE_ADDRESS_LCD; //(0x3F)<<1 // change this according to ur setup

uint8_t lcd_circ_buffer[LCD_CIRCBUFF_MAX];
uint16_t lcd_circ_buff_in = 0;
uint16_t lcd_circ_buff_out = 0;
uint16_t lcd_tx_length = 0;


uint8_t flag_sendLcd = 1;
uint8_t I2CtxComplete = 1;

/*Used by lcd_init() only*/
void __lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (i2c_handler, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void __lcd_send_cmd_buff (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
//	HAL_I2C_Master_Transmit (i2c_handler, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
	__add_to_lcdBuff((uint8_t*)data_t,4);
}


void __lcd_send_data_buff (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
//	HAL_I2C_Master_Transmit (i2c_handler, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
	__add_to_lcdBuff((uint8_t*)data_t,4);
}


void lcd_clearScreen_buff (void)
{
	__lcd_send_cmd_buff (0x80);
	for (int i=0; i<80; i++)
	{
		__lcd_send_data_buff (' ');
	}
}


void lcd_cursor_buff(int x, int y)
{
    switch (y){
    	case 0:
    		__lcd_send_cmd_buff(LCD_DDRAM_ADDRESS + LCD_START_LINE1 + x);
    	break;
    	case 1:
    		__lcd_send_cmd_buff(LCD_DDRAM_ADDRESS + LCD_START_LINE2 + x);
    	break;
    	case 2:
    		__lcd_send_cmd_buff(LCD_DDRAM_ADDRESS + LCD_START_LINE3 + x);
    	break;
    	case 3:
    		__lcd_send_cmd_buff(LCD_DDRAM_ADDRESS + LCD_START_LINE4 + x);
    	break;
    }
}


void lcd_init (I2C_HandleTypeDef* hi2c, uint8_t i2c_address)
{
	i2c_handler = hi2c; //set the i2c handler struct.
	SLAVE_ADDRESS_LCD = (i2c_address << 1); //set the lcd i2c address (left alignment).

	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	__lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	__lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	__lcd_send_cmd (0x30);
	HAL_Delay(10);
	__lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	__lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	__lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	__lcd_send_cmd (0x01);  // clear display
	HAL_Delay(2);
	__lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	__lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}


void lcd_string_buff (char *str)
{
	while (*str) __lcd_send_data_buff(*str++);
}


void __add_to_lcdBuff(uint8_t* data, uint8_t sizeData){
	uint8_t i = 0;
	while (i < sizeData){
		lcd_circ_buffer[lcd_circ_buff_in] = *(data + i);
		lcd_circ_buff_in++;
		i++;
		if (lcd_circ_buff_in == LCD_CIRCBUFF_MAX){
			lcd_circ_buff_in = 0;
		}
	}
	flag_sendLcd = 1;
}


void __lcd_sendBuff_IT (void){

	extern uint8_t I2CtxComplete;

	uint16_t I2C_Tx_ptr;
	uint16_t I2C_Tx_length;

	//if (I2CtxComplete != 0){ //si no termino la transmision anterior...

		if (lcd_circ_buff_out == LCD_CIRCBUFF_MAX){
			lcd_circ_buff_out = 0;
		}

		if (lcd_circ_buff_out == lcd_circ_buff_in){
			//no hay transmisión acá
			flag_sendLcd = 0;
			return;
		}

		if (lcd_circ_buff_out > lcd_circ_buff_in){
			lcd_tx_length = LCD_CIRCBUFF_MAX - lcd_circ_buff_out;//transmite solo hasta el final del buffer, para luego reacomodar los punteros.
		}else{
			lcd_tx_length = lcd_circ_buff_in - lcd_circ_buff_out;
		}

//		I2C_Tx_ptr = lcd_circ_buff_out;
//		I2C_Tx_length = lcd_tx_length;
//		lcd_circ_buff_out += lcd_tx_length;
//		lcd_tx_length = 0;

		if (I2CtxComplete != 0){
			I2CtxComplete = 0;

			I2C_Tx_ptr = lcd_circ_buff_out;
			I2C_Tx_length = lcd_tx_length;
			lcd_circ_buff_out += lcd_tx_length;
			lcd_tx_length = 0;
			HAL_I2C_Master_Transmit_IT(i2c_handler, SLAVE_ADDRESS_LCD,(uint8_t *) &lcd_circ_buffer[I2C_Tx_ptr],I2C_Tx_length);
		}
	//}
}


void lcd_upDate_IT(void){
	if (flag_sendLcd) __lcd_sendBuff_IT();
}


/* Insert the next function in your ISR*/
void _lcd_ISR (void){
	I2CtxComplete=1;
}
