/*
 * SSpedalBPMFX.h
 *
 *  Created on: 28 jun. 2019
 *      Author: Luciano Salvatore
 */

#ifndef SSPEDALBPMFX_H_
#define SSPEDALBPMFX_H_


/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "stm32f4xx_hal.h"
#include "lcd_i2c_IT_lfs.h"

#define CHORD_MAX 7			//max index possition of "str_chord []"
#define CHORDINV_MAX  2		//max index possition of "str_chordinv []"


//Public functions:

//unsigned long makeChord(unsigned long fnote, unsigned long chord, unsigned long chordInv);
unsigned long makeChord(unsigned long fnote, uint8_t fchord);
void plesewait_display(uint8_t lcd_dir);
void screenSelect (uint8_t screenNum);
void setingUp_BassPedal (void);
void setChord(uint8_t fnote);

//Private functions:
void turnOnScreen(void);
void mainScreen(void);
void chordScreen(void);
void tunneScreen(void);
void plesewaitScreen(void);
void on_off_octave(void);
void recuest_footkey (void);
void pop_up_savedChord(uint8_t);
void pop_up_sustain(void);
void pop_up_cancel(void);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* SSPEDALBPMFX_H_ */
