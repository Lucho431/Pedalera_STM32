/*
 * 74HC165_SPI_lfs.h
 *
 *  Created on: 24 jun. 2021
 *      Author: Usuario
 */

#ifndef INC_74HC165_SPI_LFS_H_
#define INC_74HC165_SPI_LFS_H_

//#include "stm32f1xx_hal.h"
#include "main.h"

void spi_74HC165_init (SPI_HandleTypeDef*, GPIO_TypeDef*, uint16_t, GPIO_TypeDef*, uint16_t);
void spi_74HC165_receive (uint8_t*, uint16_t);

#endif /* INC_74HC165_SPI_LFS_H_ */
