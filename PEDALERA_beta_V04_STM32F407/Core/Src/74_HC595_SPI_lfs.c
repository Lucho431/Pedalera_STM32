/*
 * 74_HC595_SPI_lfs.c
 *
 *  Created on: Dec 12, 2021
 *      Author: Luciano Salvatore
 */

#include "74_HC595_SPI_lfs.h"

SPI_HandleTypeDef* spi_handler;
GPIO_TypeDef* portST;
uint16_t pinST;


void spi_74HC595_init (SPI_HandleTypeDef* hspi, GPIO_TypeDef* STport, uint16_t STpin){

	spi_handler =hspi;

	portST = STport;
	pinST = STpin;

	HAL_GPIO_WritePin(portST, pinST, 0);

} //end spi_74HC595_init()


void spi_74HC595_Transmit (uint8_t* pdata, uint16_t sizeData){

	HAL_SPI_Transmit(spi_handler, pdata, sizeData, 100);
	HAL_GPIO_WritePin(portST, pinST, 1);
//	HAL_Delay(1);
	HAL_GPIO_WritePin(portST, pinST, 0);

} //end spi_74HC595_transmit()
