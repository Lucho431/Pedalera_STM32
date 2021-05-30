/*
 * osc_dac_lfs.h
 *
 *  Created on: Oct 31, 2020
 *      Author: Luciano Salvatore
 */

#ifndef INC_OSC_DAC_LFS_H_
#define INC_OSC_DAC_LFS_H_


/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "stm32f4xx_hal.h"
//#include "tablas_osc_lfs.h"
#define NOTA_C1 24 //nota más baja de la tabla

void osc_dac_init(DAC_HandleTypeDef*, uint32_t);
void osc_dac_update(void);
int8_t osc_setNote(uint8_t);
int8_t osc_setRatios(uint8_t, uint8_t, uint8_t);



/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif /* INC_OSC_DAC_LFS_H_ */
