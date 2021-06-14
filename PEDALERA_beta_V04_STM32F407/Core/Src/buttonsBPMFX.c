/*
 * buttonsBPMFX.c
 *
 *  Created on: 13 jun. 2021
 *      Author: Luciano Salvatore
 */

#include "buttonsBPMFX.h"

uint8_t __read_button[STRUCT_LENGTH];
uint8_t __last_button[STRUCT_LENGTH] = {0};
T_INPUT __status_button[STRUCT_LENGTH];



//run the next function every 1ms for a 20ms debounce time.
void readButtons(void){

	static uint8_t readStatus = 0;

	switch (readStatus){
		case 0:
			__read_button[IN_DO] = HAL_GPIO_ReadPin(DO_GPIO_Port, DO_Pin);

			__read_button[IN_REb] = HAL_GPIO_ReadPin(REb_GPIO_Port, REb_Pin);

			__read_button[IN_RE] = HAL_GPIO_ReadPin(RE_GPIO_Port, RE_Pin);

			__read_button[IN_MIb] = HAL_GPIO_ReadPin(MIb_GPIO_Port, MIb_Pin);

			__read_button[IN_MI] = HAL_GPIO_ReadPin(MI_GPIO_Port, MI_Pin);

			__read_button[IN_FA] = HAL_GPIO_ReadPin(FA_GPIO_Port, FA_Pin);

			__read_button[IN_SOLb] = HAL_GPIO_ReadPin(SOLb_GPIO_Port, SOLb_Pin);

			__read_button[IN_SOL] = HAL_GPIO_ReadPin(SOL_GPIO_Port, SOL_Pin);

			__read_button[IN_LAb] = HAL_GPIO_ReadPin(LAb_GPIO_Port, LAb_Pin);

			__read_button[IN_LA] = HAL_GPIO_ReadPin(LA_GPIO_Port, LA_Pin);

			__read_button[IN_SIb] = HAL_GPIO_ReadPin(SIb_GPIO_Port, SIb_Pin);

			__read_button[IN_SI] = HAL_GPIO_ReadPin(SI_GPIO_Port, SI_Pin);

			__read_button[IN_DO8] = HAL_GPIO_ReadPin(DO8_GPIO_Port, DO8_Pin);

			readStatus++;
		break;
		case 1:
			__read_button[IN_OCT_G] = HAL_GPIO_ReadPin(OCT_G_GPIO_Port, OCT_G_Pin);

			readStatus++;
		break;
		case 2:
			__read_button[IN_OCT_M] = HAL_GPIO_ReadPin(OCT_M_GPIO_Port, OCT_M_Pin);

			readStatus++;
		break;
		case 3:
			__read_button[IN_OCT_A] = HAL_GPIO_ReadPin(OCT_A_GPIO_Port, OCT_A_Pin);

			readStatus++;
		break;
		case 4:
			__read_button[IN_SUST_PROP] = HAL_GPIO_ReadPin(SUST_PROP_GPIO_Port, SUST_PROP_Pin);

			readStatus++;
		break;
		case 5:
			__read_button[IN_SUST_MIDI] = HAL_GPIO_ReadPin(SUST_MIDI_GPIO_Port, SUST_MIDI_Pin);

			readStatus++;
		break;
		case 6:
			__read_button[IN_CHORD] = HAL_GPIO_ReadPin(CHORD_GPIO_Port, CHORD_Pin);

			readStatus++;
		break;
		case 7:
			__read_button[IN_TUNNE] = HAL_GPIO_ReadPin(TUNNE_GPIO_Port, TUNNE_Pin);

			readStatus++;
		break;
		case 8:
			__read_button[IN_UP] = HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin);

			readStatus++;
		break;
		case 9:
			__read_button[IN_DOWN] = HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin);

			readStatus++;
		break;
		case 10:
			__read_button[IN_LEFT] = HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin);

			readStatus++;
		break;
		case 11:
			__read_button[IN_RIGHT] = HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin);

			readStatus++;
		break;
		case 12:
			__read_button[IN_PRESET0] = HAL_GPIO_ReadPin(PRESET0_GPIO_Port, PRESET0_Pin);

			readStatus++;
		break;
		case 13:
			__read_button[IN_PRESET1] = HAL_GPIO_ReadPin(PRESET1_GPIO_Port, PRESET1_Pin);

			readStatus++;
		break;
		case 14:
			__read_button[IN_PRESET2] = HAL_GPIO_ReadPin(PRESET2_GPIO_Port, PRESET2_Pin);

			readStatus++;
		break;
		case 15:
			__read_button[IN_PRESET3] = HAL_GPIO_ReadPin(PRESET3_GPIO_Port, PRESET3_Pin);

			readStatus++;
		break;
		case 16:
			__read_button[IN_PRESET4] = HAL_GPIO_ReadPin(PRESET4_GPIO_Port, PRESET4_Pin);

			readStatus++;
		break;
		case 17:
			__read_button[IN_PRESET5] = HAL_GPIO_ReadPin(PRESET5_GPIO_Port, PRESET5_Pin);

			readStatus++;
		break;
		case 18:
			readStatus++;
		break;
		case 19:
			readStatus=0;
		default:
		break;
	} //end switch readStatus
} //end readButtons()



//run the next function under main loop.
void updateInputs(void){

	for (uint8_t i = IN_DO; i < STRUCT_LENGTH; i++){
		if(!__read_button[i]){
			if(__last_button[i] != 0){
				__status_button[i] = FALL;
			}else{
				__status_button[i] = LOW_L;
			}
		}else{
			if(__last_button[i] != 0){
				__status_button[i] = HIGH_L;
			}else{
				__status_button[i] = RISE;
			}
		} //end if !__read_button[]
		__last_button[i] = __read_button[i];
	} //end for i

} // end updateInputs()


T_INPUT getStatButton(BUTTON_T b){

	return __status_button[b];
} // end getStatButton()
