/*
 * SSpedalBPMFX.c
 *
 *  Created on: 28 jun. 2019
 *      Author: Luciano Salvatore
 *       Brief: This library is a port for the original to Arduino Due board.
 *
 */

#include "SSpedalBPMFX.h"

extern unsigned char bmp_logo_128_64 [];

unsigned int   chordSelect_cursor;  //posicion del cursor en el menu de acordes.

uint8_t firstDoTunning;

char* str_tunne [12]=	{"C",
							"C#",
							"D",
							"D#",
							"E",
							"F",
							"F#",
							"G",
							"G#",
							"A",
							"A#",
							"B"};

int chord;			//is the str_chord[] index (from 0 to 7).
							//values ​​from 0 to 8 indicate that the chord corresponding
							//to the string of the position "str_chord [chord]" was selected.

char* str_chord[CHORD_MAX + 1]= {"1-3-5",
										"1-3m-5",
										"1-3-6",
										"1-3m-6",
										"1-3-b7",
										"1-3m-b7",
										"1-3-7",
										"1-3m-7"};


int chordInv;     //is the str_chordInv[] index (from 0 to 2).


char* str_chordInv[CHORDINV_MAX + 1]={	"Fundamental",
												"1 Inversion",
												"2 Inversion"};

int octava = 0;

char* str_octava[3]= {"Grave",
							"Media",
							"Aguda"};

uint8_t sust_id;

unsigned long chordSet[13] =  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

const unsigned long hex_chord[CHORD_MAX + 1]= {	0x070400,
												0x070300,
												0x090400,
												0x090300,
												0x0a0400,
												0x0a0300,
												0x0b0400,
												0x0b0300 };


const unsigned long hex_chordInv[CHORDINV_MAX + 1]={	0x0,
														0x0c0c00,
														0x0c0000 };


/*	Function:					makeChord (overload)

	Description:			generates the indicated chord, based on the fundamental note received

	Parameter: bnote		Is a single-byte number that contains the fundamental note in HEX.

	Parameter: fchord		Number of the chord sent. It was made by setChord() and store in "chordSet[]" vector.

	Returns:	unsigned long	(0xXXXXXX) A 3-byte number that contains the fundamental note in the least significant byte, followed by the 2nd and 3rd notes of the chord.
*/
unsigned long makeChord(unsigned long bnote, uint8_t fchord){

	unsigned long notes;

	notes = bnote;
	notes += (bnote << 8);
	notes += (bnote << 16);


	notes += chordSet[fchord];


	return notes;
}

/*	Function:					setChord

	Description:			links the selected chord w/ the pressed note.

	Parameter: fnote		id of the pressed note.

	Returns:	void
*/
void setChord(uint8_t fnote){

chordSet[fnote] = hex_chord[chord] - hex_chordInv[chordInv];

}

/*	Function:					screenSelect

	Description:			choose what screen must print in the LCD.

	Parameter: screenNum	Is the screen id

	Returns:	void
*/
/*
void screenSelect(uint8_t screenNum){
	switch (screenNum){
		case 1:
			__turnOnScreen ();
		break;

		case 2:
			__plesewaitScreen (); // hay que cambiar la funcion
		break;

		case 3:
			__mainScreen ();
		break;

		case 4:
			__chordScreen ();
		break;

		case 5:
			__on_off_octave (); //hay que definir
		break;

		case 6:
			__recuest_footkey ();
		break;

		case 7:
			__pop_up_screen ();
		break;
	}
}
*/

/*	Function:					__turnOnScreen

	Description:			prints the first screen while turning on.

	Parameters: void

	Returns:	void
*/

void __turnOnScreen(void){

	/////////////CARACTERES ESPECIALES

	SSD1306_DrawPicture_custom(0, 0, 128, 64, bmp_logo_128_64);
	SSD1306_UpdateScreen();
	HAL_Delay(3000);
}

/*	Function:					__chordScreen

	Description:			prints the chord menu screen.

	Parameters: void

	Returns:	void
*/

void __chordScreen(void){
	SSD1306_Fill(0);
	SSD1306_GotoXY(10,0);
	SSD1306_Puts("Acorde: ", &Font_7x10, 1);

	SSD1306_Puts( str_chord[chord], &Font_7x10, 1);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("Pos:", &Font_7x10, 1);
	SSD1306_Puts( str_chordInv[chordInv], &Font_7x10, 1);
	SSD1306_GotoXY(10,40);
	SSD1306_Puts("Grabar acorde", &Font_7x10, 1);

	SSD1306_GotoXY(0, chordSelect_cursor*20);
	SSD1306_Puts(">", &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*	Method:					__mainScreen

	Description:			prints the main menu screen.

	Parameters: void

	Returns:	void
*/

void __mainScreen(void){

	extern uint8_t sustainMode;
	extern uint8_t acorde;

  	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Afinacion: ", &Font_7x10, 1);
	SSD1306_Puts(str_tunne[firstDoTunning - 48], &Font_7x10, 1);
	SSD1306_GotoXY(0,16);
	SSD1306_Puts("Octava: ", &Font_7x10, 1);
	SSD1306_Puts(str_octava[octava + 1], &Font_7x10, 1);
	SSD1306_GotoXY(0,31);
	SSD1306_Puts("Sustain: ", &Font_7x10, 1);
	switch (sustainMode){
		case 0:
			SSD1306_Puts("OFF", &Font_7x10, 1);
		break;
		case 1:
			SSD1306_Puts("smart", &Font_7x10, 1);
		break;
		case 2:
			SSD1306_Puts("clasico", &Font_7x10, 1);
		break;
	}//end switch.
	SSD1306_GotoXY(0,46);
	SSD1306_Puts("Acordes: ", &Font_7x10, 1);
	switch (acorde){
		case 0:
			SSD1306_Puts("OFF", &Font_7x10, 1);
		break;
		case 1:
			SSD1306_Puts("ON", &Font_7x10, 1);
		break;
	}//end switch.
	SSD1306_UpdateScreen();

}
/*	Method:					tunneScreen

	Description:			print the tunne selection screen.

	Parameter: lcd_dir		Is the I2C direcction of LCD display.

	Returns:	void
*/

void __tunneScreen(void){

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,0);
	SSD1306_Puts("AFINACION:", &Font_7x10, 1);
	SSD1306_GotoXY(20,20);
	SSD1306_Puts(str_tunne[firstDoTunning - 48], &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*	Method:					plesewait_display

	Description:			print the "please wait" screen.

	Parameter: lcd_dir		Is the I2C direcction of LCD display.

	Returns:	void
*/

void __plesewaitScreen (void){

	SSD1306_Fill(0);
	SSD1306_GotoXY(30, 20);
	SSD1306_Puts("Por favor", &Font_7x10, 1);
	SSD1306_GotoXY(40, 30);
	SSD1306_Puts("Espere...", &Font_7x10, 1);
	SSD1306_UpdateScreen();
	for (int i=0; i<6; i++){

		for (int j =0; j<4; j++){
		/*
			lcd.createChar(1, pedalera.relojA1[j]);
			lcd.createChar(2, pedalera.relojA2[j]);
			lcd.createChar(3, pedalera.relojA3[j]);
			lcd.createChar(4, pedalera.relojA4[j]);
			lcd.createChar(5, pedalera.relojA5[j]);
			lcd.createChar(6, pedalera.relojA6[j]);

			oled.setTextXY(0,0);
			oled.putStringByte(1);
			oled.setTextXY(1, 0);
			oled.putStringByte(2);
			oled.setTextXY(2, 0);
			oled.putStringByte(3);
			oled.setTextXY(0, 1);
			oled.putStringByte(4);
			oled.setTextXY(1, 1);
			oled.putStringByte(5);
			oled.setTextXY(2, 1);
			oled.putStringByte(6);
		*/
			HAL_Delay(400);
		}

	}
}


/*	Method:					__on_off_octave

	Description:			print the "please wait" screen.

	Parameter: lcd_dir		Is the I2C direcction of LCD display.

	Returns:	void
*/

void __on_off_octave (void){

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("Octava:  ", &Font_7x10, 1);
	SSD1306_Puts(str_octava[octava + 1], &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*	Method:					__recuest_footkey

	Description:			print the recuest footkey screen.

	Parameter: void

	Returns:	void
*/

void __recuest_footkey (void){

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,0);
	SSD1306_Puts("Pulse una tecla para", &Font_7x10, 1);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("grabar el acorde...", &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

/*	Method:					__pop_up_savedChord

	Description:			print a pop-up screen.

	Parameter: void

	Returns:	void
*/

void __pop_up_savedChord (uint8_t fk){

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,0);
	SSD1306_Puts("ACORDE GRABADO EN LA", &Font_7x10, 1);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("TECLA ", &Font_7x10, 1);
	SSD1306_Puts(str_tunne[fk], &Font_7x10, 1);
	SSD1306_UpdateScreen();
}



/*	Method:					pop_up_sustain

	Description:			print the "please wait" screen.

	Parameter: lcd_dir		Is the I2C direcction of LCD display.

	Returns:	void
*/

void __pop_up_sustain (void){

	extern uint8_t sustainMode;

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("Sustain: ", &Font_7x10, 1);
	switch (sustainMode){
		case 0:
			SSD1306_Puts("OFF", &Font_7x10, 1);
		break;
		case 1:
			SSD1306_Puts("smart", &Font_7x10, 1);
		break;
		case 2:
			SSD1306_Puts("clasico", &Font_7x10, 1);
		break;
	}//end switch.
	SSD1306_UpdateScreen();
}



/*	Method:					pop_up_cancel

	Description:			print the cancel screen.

	Parameter: lcd_dir		Is the I2C direcction of LCD display.

	Returns:	void
*/

void __pop_up_cancel (void){

	SSD1306_Fill(0);
	SSD1306_GotoXY(10,20);
	SSD1306_Puts("ACCION CANCELADA", &Font_7x10, 1);
	SSD1306_UpdateScreen();
}
