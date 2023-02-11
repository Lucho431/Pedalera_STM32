/*
 * buttonsBPMFX.h
 *
 *  Created on: 13 jun. 2021
 *      Author: Luciano Salvatore
 */

#ifndef INC_BUTTONSBPMFX_H_
#define INC_BUTTONSBPMFX_H_

#include "main.h"

//typedef enum{
//	IN_DO,
//	IN_REb,
//	IN_RE,
//	IN_MIb,
//	IN_MI,
//	IN_FA,
//	IN_SOLb,
//	IN_SOL,
//	IN_LAb,
//	IN_LA,
//	IN_SIb,
//	IN_SI,
//	IN_DO8,
//	IN_OCT_G,
//	IN_OCT_M,
//	IN_OCT_A,
//	IN_PRESET0,
//	IN_PRESET1,
//	IN_PRESET2,
//	IN_PRESET3,
//	IN_PRESET4,
//	IN_PRESET5,
//	IN_SUST_MIDI,
//	IN_SUST_PROP,
//	IN_CHORD,
//	IN_TUNNE,
//	IN_UP,
//	IN_DOWN,
//	IN_LEFT,
//	IN_RIGHT,
//	IN_OK,
//	IN_BACK,
//	STRUCT_LENGTH,
//}BUTTON_T;

typedef enum{
	IN_CHORD,
	IN_OK, //sustituido. Original: IN_TUNNE
	IN_UP,
	IN_DOWN,
	IN_LEFT,
	IN_RIGHT,
	IN_TUNNE, //sustituido. Original: IN_OK
	IN_BACK,
	IN_PRESET0,
	IN_PRESET1,
	IN_PRESET2,
	IN_PRESET3,
	IN_PRESET4,
	IN_PRESET5,
	IN_SUST_MIDI,
	IN_SUST_PROP,
	IN_LAb,
	IN_LA,
	IN_SIb,
	IN_SI,
	IN_DO8,
	IN_OCT_G,
	IN_OCT_M,
	IN_OCT_A,
	IN_DO,
	IN_REb,
	IN_RE,
	IN_MIb,
	IN_MI,
	IN_FA,
	IN_SOLb,
	IN_SOL,
	STRUCT_LENGTH,
}BUTTON_T;

typedef enum{
	LED_PS5,
	LED_SUST_M,
	LED_SUST_P,
	LED_CHORD,
	LED_OCT_G = 8,
	LED_OCT_M,
	LED_OCT_A,
	LED_PS0,
	LED_PS1,
	LED_PS2,
	LED_PS3,
	LED_PS4,
	STRUCT_LEDS_LENGTH,
}T_LEDS;

//typedef enum{
//    LOW_L,
//    HIGH_L,
//    FALL,
//    RISE,
//}T_INPUT;

#endif /* INC_BUTTONSBPMFX_H_ */
