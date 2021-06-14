/*
 * buttonsBPMFX.h
 *
 *  Created on: 13 jun. 2021
 *      Author: Luciano Salvatore
 */

#ifndef INC_BUTTONSBPMFX_H_
#define INC_BUTTONSBPMFX_H_

#include "main.h"

typedef enum{
	IN_DO,
	IN_REb,
	IN_RE,
	IN_MIb,
	IN_MI,
	IN_FA,
	IN_SOLb,
	IN_SOL,
	IN_LAb,
	IN_LA,
	IN_SIb,
	IN_SI,
	IN_DO8,
	IN_OCT_G,
	IN_OCT_M,
	IN_OCT_A,
	IN_SUST_PROP,
	IN_SUST_MIDI,
	IN_CHORD,
	IN_UP,
	IN_DOWN,
	IN_LEFT,
	IN_RIGHT,
	IN_ENTER,
	IN_PRESET0,
	IN_PRESET1,
	IN_PRESET2,
	IN_PRESET3,
	IN_PRESET4,
	IN_PRESET5,
	IN_TUNNE,
	STRUCT_LENGTH,
}BUTTON_T;

typedef enum{
    LOW_L,
    HIGH_L,
    FALL,
    RISE,
}T_INPUT;

void updateInputs(void);
void readButtons(void);
T_INPUT getStatButton(BUTTON_T);

#endif /* INC_BUTTONSBPMFX_H_ */
