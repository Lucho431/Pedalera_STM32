/*
 * 74_HC595_SPI_lfs.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Luciano Salvatore
 */

#ifndef INC_74_HC595_SPI_LFS_H_
#define INC_74_HC595_SPI_LFS_H_

//#include "stm32f1xx_hal.h"
#include "main.h"

void spi_74HC595_init (SPI_HandleTypeDef*, GPIO_TypeDef*, uint16_t);
void spi_74HC595_Transmit (uint8_t*, uint16_t);


#endif /* INC_74_HC595_SPI_LFS_H_ */
