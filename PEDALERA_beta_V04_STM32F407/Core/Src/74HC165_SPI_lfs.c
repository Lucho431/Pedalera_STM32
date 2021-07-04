/*
 * 74HC165_SPI_lfs.c
 *
 *  Created on: 24 jun. 2021
 *      Author: Luciano Salvatore
 */

#include "74HC165_SPI_lfs.h"

SPI_HandleTypeDef* spi_handler;
GPIO_TypeDef* portPL;
GPIO_TypeDef* portCE;
uint16_t pinPL, pinCE;


void spi_74HC165_init (SPI_HandleTypeDef* hspi, GPIO_TypeDef* PLport, uint16_t PLpin, GPIO_TypeDef* CEport, uint16_t CEpin){

	spi_handler=hspi;

	portPL = PLport;
	pinPL = PLpin;

	portCE = CEport;
	pinCE = CEpin;

}


void spi_74HC165_receive (uint8_t* pdata, uint16_t sizeData){

	//carga paralela
	HAL_GPIO_WritePin(portPL, pinPL, 0);
	HAL_GPIO_WritePin(portPL, pinPL, 1);

	//clock enable
	HAL_GPIO_WritePin(portCE, pinCE, 0);

	//SPI
	HAL_SPI_Receive(spi_handler, pdata, sizeData, 100);

	//clock disable
	HAL_GPIO_WritePin(portCE, pinCE, 1);
}
