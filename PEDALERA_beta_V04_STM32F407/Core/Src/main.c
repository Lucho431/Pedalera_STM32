/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buttonsBPMFX.h"
#include "74HC165_SPI_lfs.h"
#include "74_HC595_SPI_lfs.h"
#include "usbd_midi_if.h"
#include "SSpedalBPMFX.h"
#include "osc_dac_lfs.h"

//#include "usbd_core.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum{
	NO_SCREEN,
	TURN_ON_SCREEN,
	PLEASE_WAIT_SCREEN,
	MAIN_SCREEN,
	CHORD_SCREEN,
	TUNNE_SCREEN,
	PRESET_SCREEN,
	RECUEST_FOOTKEY_SCREEN,
	POPUP_SAVED_CHORD,
	POPUP_OCTAVE,
	POPUP_SUSTAIN,
	POPUP_CANCEL,
}SCREEN_NUM;

typedef enum{
	NORMAL,
	SAVE_CHORD,
	SAVE_TUNNE,
}FOOTKEY_FUNC;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MIDI_C2 36
#define MIDI_B2 47
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define GETBUTTONSTATUS(BUTTON,EDGE) (EDGE & (0B1 << (BUTTON)))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned long lastSendNote[] =  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};   // vector para apagar la ultima nota mandada por el pedal (salva el error del octavador)
unsigned long lastSustainNote = 0;		//

uint8_t sustainFlag = 1;   /// flag to switch on/off a sustained note.

uint8_t sustainMode = 0; // 0: none, 1: prop, 2: midi.

//variables del adc
uint32_t bufferADC [3] = {0, 0, 0};
uint8_t flag_adc = 0;
uint16_t refresh_adc = 250;

//variables de escritura de leds
uint16_t leds_octava;
uint16_t leds_presets;
uint16_t leds_resto;
uint16_t leds_buffer;

//variables de lectura de botones:
uint32_t read_button = 0xFFFFFFFF;
uint32_t last_button = 0xFFFFFFFF;
uint32_t buttonFall = 0;
uint32_t buttonRise = 0;
uint8_t refresh_buttons = 20;
BUTTON_T teclas[] = {
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
};



//uint8_t buttonState [STRUCT_LENGTH];
//uint8_t lastState [STRUCT_LENGTH];

uint32_t last_GPIOB_IDR = 0;


SCREEN_NUM menu = MAIN_SCREEN;

uint8_t presetLed = 0; // flag to manage the presets leds. 0 value is the first.

uint8_t acorde = 0;	// flag to switch on/off the chord mode

uint8_t tunne = 0;		// flag to switch on/off the tunne mode

//unsigned int d_pad;  //las flechitas pulsadas: 0-ninguna, 1-arriba, 2-abajo, 3-izquierda, 4-derecha.

SCREEN_NUM screenNum = NO_SCREEN;	//flag to select the screen to display.
SCREEN_NUM last_screenNum = MAIN_SCREEN;	//flag to select the screen to display.


volatile int32_t popup_time = 0;
volatile uint8_t popup_timeUp = 0;


uint8_t lastKey_pos; // used to show the footkey possition in the "pop_up_savedChord()" function.

uint8_t generalState = 0; // global state machine variable.

FOOTKEY_FUNC footKey = NORMAL;

extern unsigned int   chordSelect_cursor;  //posicion del cursor en el menu de acordes.

extern int chord;

extern uint8_t firstDoTunning;

extern int chordInv;

extern int octava;

uint8_t midi_msg[4]; //midi frame to send over USB.

uint8_t flag_tick= 0; // HAL_TIM_PeriodElapsedCallback flag for tick.
uint8_t flag_dac = 0; // HAL_TIM_PeriodElapsedCallback flag for dac.
uint8_t lcd_refresh_time = 25;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void sustainProp(void);
void sustainMIDI(void);
void inputChord(void);
void inputOctave(void);
void inputTunne(void);
void inputPresets(void);
void inputNotes(void);
void inputSaveChord(void);
void chordSelect(void);
void tunneSelect(void);
void sendChord(unsigned long, int, int);
void showLeds(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  firstDoTunning = 36;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /*
   * TIM2 is an 1 ms tick timer.
   * TIM3 is a 44100Hz timer.
   */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_I2C1_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  spi_74HC165_init(&hspi1, PL_bot_GPIO_Port, PL_bot_Pin, CE_bot_GPIO_Port, CE_bot_Pin);
  spi_74HC595_init(&hspi1, ST_leds_GPIO_Port, ST_leds_Pin);

  adc_potes_init();

  HAL_TIM_Base_Start_IT(&htim2); // 1 ms tick timer.
  HAL_TIM_Base_Start_IT(&htim3); // 44100Hz timer.

  osc_dac_init(&hdac, DAC_CHANNEL_1);
  osc_setRatios(100, 100, 100);

  leds_buffer = 0xFFFF;
  spi_74HC595_Transmit((uint8_t*) &leds_buffer, 2 /*bytes*/);

  lcd_init(&hi2c1, 0x3f);

  //rutina de encendido//

  turnOnScreen();
  HAL_Delay(500);
  showLeds();
  HAL_Delay(500);
  mainScreen();
  leds_octava = 0b1 << LED_OCT_M;
  leds_buffer = leds_octava;
  leds_buffer = ~leds_buffer;
  spi_74HC595_Transmit((uint8_t*) &leds_buffer, 2 /*bytes*/);

  HAL_ADC_Start_IT(&hadc1);
  //fin rutina de encendido//

  //mensaje midi de prueba//
//  midi_msg[0] = 0x0C;	//0x0C == type event (program change)
//  midi_msg[1] = 0xC0 | 1;
//  midi_msg[2] = 60;
//  midi_msg[3] = 0;	//last param == 0 (unused in program change).
//  MIDI_SendBuffer(midi_msg, 4);
  //fin mensaje midi de prueba//

  osc_setNote(24);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch (screenNum){
	  		case MAIN_SCREEN:
	  			mainScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case CHORD_SCREEN:
	  			chordScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case TUNNE_SCREEN:
	  			tunneScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case RECUEST_FOOTKEY_SCREEN:
	  			recuest_footkey();
	  			last_screenNum = screenNum; //this variable must store the CHORD_SCREEN value, to use it as return point.
	  		break;
	  		case POPUP_OCTAVE:
	  			on_off_octave();
	  			popup_time = 2000;
	  		break;
	  		case POPUP_SAVED_CHORD:
	  			pop_up_savedChord(lastKey_pos);
	  			last_screenNum = CHORD_SCREEN;
	  			popup_time = 2000;
	  		break;
	  		case POPUP_SUSTAIN:
	  			pop_up_sustain();
	  			popup_time = 2000;
	  		break;
	  		case POPUP_CANCEL:
	  			pop_up_cancel();
	  			popup_time = 2000;
	  			last_screenNum = CHORD_SCREEN;
	  		break;
	  		default:
	  		break;
	  	}
	  	screenNum = NO_SCREEN;

	  	if (popup_timeUp){
	  		if (screenNum == NO_SCREEN) screenNum = last_screenNum;
	  		popup_timeUp = 0;
	  	}

	  	//botones y teclas
	  	buttonFall = last_button & ~read_button;
	  	buttonRise = ~last_button & read_button;
	  	if (GETBUTTONSTATUS(IN_BACK, buttonFall)){
	  		screenNum = MAIN_SCREEN;
	  		menu = MAIN_SCREEN;
	  	}
	  	if (GETBUTTONSTATUS(IN_SUST_PROP, buttonFall)){
	  		sustainProp();
	  	}
	  	if (GETBUTTONSTATUS(IN_SUST_MIDI, buttonFall)){
	  		sustainMIDI();
	  	}
	  	if (GETBUTTONSTATUS(IN_CHORD, buttonFall)){
	  		inputChord();
	  	}
	  	inputOctave();
	  	if (GETBUTTONSTATUS(IN_TUNNE, buttonFall)){
	  		inputTunne();
	  	}
	  	inputPresets();

	  	last_button = read_button;
	  	//fin botones y teclas

	  	/*
	  	switch (footKey){
	  		case NORMAL:

	  			inputNotes();

	  		break;
	  		case SAVE_CHORD:

	  			inputSaveChord();
	  		default:
	  		break;
	  	}
	  	*/

	    switch (menu){ //manage the recuested menu function.
	    	case MAIN_SCREEN:
	    		inputNotes();
	    	break;
	  		case CHORD_SCREEN:
	  			chordSelect();
	  			inputNotes();
	  		break;
	  		case TUNNE_SCREEN:
	  			tunneSelect();
	  			inputNotes();
	  		break;
	  		case RECUEST_FOOTKEY_SCREEN:
	  			inputSaveChord(); // @suppress("No break at end of case")
	  		default:
	  		break;

	    }

	    MIDI_SendCheck();

		if (flag_dac != 0){
		  osc_dac_update();
		  flag_dac = 0;
		}


	    if (flag_tick != 0){

	    	if (lcd_refresh_time != 0){
	    		lcd_refresh_time--;
	    	}else{
	    		lcd_upDate_IT();
	    		lcd_refresh_time = 25;
	    	}//end if lcd_refresh_time

	    	if (popup_time != 0){
				popup_time--;
				if (popup_time == 1) popup_timeUp = 1;
			}//end if popup_time

	    	if (refresh_buttons != 0){
	    		refresh_buttons--;
	    	}else{
//	    		last_button = read_button;
	    		spi_74HC165_receive((uint8_t *)&read_button, sizeof(read_button));

//	    		buttonFall = last_button & ~read_button;
//	    		buttonRise = ~last_button & read_button;

	    		leds_buffer = leds_octava | leds_presets | leds_resto;
	    		leds_buffer = ~leds_buffer; //pasa a logica negativa
	    		spi_74HC595_Transmit((uint8_t*) &leds_buffer, 2 /*bytes*/);

//	    		for (uint8_t i = 0; i < 16; i++){
//	    			buff_leds = 0x1 << i;
//	    			spi_74HC595_Transmit((uint8_t*) &buff_leds, 2);
//	    			__NOP();
//	    		}


	    		refresh_buttons = 20;
	    	}

	    	if (refresh_adc != 0){
	    		refresh_adc--;
	    	}else{
	    		if (flag_adc != 0){

	    			set_nextPote();

	    			HAL_ADC_Start_IT(&hadc1);

	    			flag_adc = 0;

	    			refresh_adc = 250;
	    		}
	    	}

	    	flag_tick = 0;
	    }// end flag_tick

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void sustainProp (void){

		for (int j = 0; j <= 12; j++){
			sendChord(lastSendNote[j], 0, 1); // silents all notes
		}

		switch (sustainMode){
			case 2:
				//event = {0x0B, 0xB0 | MIDI_CHANNEL, 64, 0};
				//MidiUSB.sendMIDI(event);
				midi_msg[0] = 0x0B; //0x0B == type event (control change)
				midi_msg[1] = 0xB0 | 1;
				midi_msg[2] = 64;	// 64 == sustain.
				midi_msg[3] = 0;
				MIDI_SendBuffer(midi_msg, 4); // @suppress("No break at end of case")
			case 0:
				lastSustainNote = 0;
				sustainMode = 1; //prop
				leds_resto |= 0x1 << LED_SUST_P;
				leds_resto &= ~(0x1 << LED_SUST_M);
			break;

			case 1:
				sustainFlag = 1;
				sustainMode = 0;
				leds_resto &= ~(0x1 << LED_SUST_M);
				leds_resto &= ~(0x1 << LED_SUST_P);
			break;
		} //end switch

		screenNum = POPUP_SUSTAIN;

} //end sustainProp()

void sustainMIDI (void){

		switch (sustainMode){
			case 0:
			case 1:
				sustainFlag = 1;	// needed to normal notes functioning.

				midi_msg[0] = 0x0B;		//0x0B == type event (control change),
				midi_msg[1] = 0xB0 | 1;
				midi_msg[2] = 64;	//64 == sustain.
				midi_msg[3] = 127;
				MIDI_SendBuffer(midi_msg, 4);

				sustainMode = 2;
				leds_resto |= 0x1 << LED_SUST_M;
				leds_resto &= ~(0x1 << LED_SUST_P);
			break;
			case 2:
				midi_msg[0] = 0x0B;	//0x0B == type event (control change)
				midi_msg[1] = 0xB0 | 1;
				midi_msg[2] = 64;	//64 == sustain.
				midi_msg[3] = 0;
				MIDI_SendBuffer(midi_msg, 4);

				sustainMode = 0;
				leds_resto &= ~(0x1 << LED_SUST_M);
				leds_resto &= ~(0x1 << LED_SUST_P);
			break;
		} //end switch

		screenNum = POPUP_SUSTAIN;
} //end sustainMIDI()

void inputChord (void){

	switch (menu){
		case MAIN_SCREEN:
			if (acorde != 0){
				acorde = 0;
				leds_resto &= ~(0x1 << LED_CHORD);
				screenNum = MAIN_SCREEN;
			}else{
				screenNum = CHORD_SCREEN;
				menu = CHORD_SCREEN;
				acorde = 1;
				leds_resto |= 0x1 << LED_CHORD;
			} //fin if acorde
		break;
		case CHORD_SCREEN:
			screenNum = MAIN_SCREEN;
			menu = MAIN_SCREEN;
			acorde = 0;
			leds_resto &= ~(0x1 << LED_CHORD);
		break;
		default:
			screenNum = CHORD_SCREEN;
			menu = CHORD_SCREEN;
			acorde = 1;
			leds_resto |= 0x1 << LED_CHORD;
		break;
	} //fin switch menu

} //end inputChord()


void inputOctave (void){

	if(GETBUTTONSTATUS(IN_OCT_G, buttonFall)){
		octava = -1;
		screenNum=POPUP_OCTAVE;
		leds_octava = 0x1 << LED_OCT_G;
		return;
	}

	if (GETBUTTONSTATUS(IN_OCT_M, buttonFall)){
		octava = 0;
		screenNum=POPUP_OCTAVE;
		leds_octava = 0x1 << LED_OCT_M;
		return;
	}

	if (GETBUTTONSTATUS(IN_OCT_A, buttonFall)){
		octava = 1;
		screenNum=POPUP_OCTAVE;
		leds_octava = 0x1 << LED_OCT_A;
		return;
	}
} //end inputOctave()


void inputTunne(void){

      if (menu != TUNNE_SCREEN){
		screenNum = TUNNE_SCREEN;
		menu = TUNNE_SCREEN;
	  }else{
		screenNum = MAIN_SCREEN;
		menu = MAIN_SCREEN;
	  } //end  if menu

} //end inputTunne()


void inputPresets (void){
	for (int i = 0; i < 6; i++){                                         // 6 presets en total.

		if (GETBUTTONSTATUS(i + IN_PRESET0, buttonFall)){
			midi_msg[0] = 0x0C;	//0x0C == type event (program change)
			midi_msg[1] = 0xC0 | 1;
			midi_msg[2] = i;
			midi_msg[3] = 0;	//last param == 0 (unused in program change).
			MIDI_SendBuffer(midi_msg, 4);
			if (i == 5){
				leds_presets = 0x1;
			}else{
				leds_presets = 0x1 << (LED_PS0 + i);
			}
		} //end if GETBUTTONSTATUS

	} //end for

} //end inputPresets()


void inputNotes(void){

  uint8_t pressedNote;

  for (int i = 0; i <= 12; i++){

//    buttonState[i + IN_DO] = HAL_GPIO_ReadPin(notePort_list[i], notePin_list[i]); //read current button pin.

//    if(!buttonState[i + IN_DO] && lastState[i + IN_DO]){  // If we read 1, and the last time we read 0, means button was just pressed.
    if (GETBUTTONSTATUS(teclas[i], buttonFall)){
        pressedNote = firstDoTunning + i + 12 * octava; //detects the pressed note.

    	if (acorde){ //If chord mode is on...
            lastSendNote[i] = makeChord(pressedNote, i); // saves the current chord config. on pressed note.

        }else{ //If chord mode is off...

            lastSendNote[i] = pressedNote; // saves nothing but pressed note.
        } //end if acorde


        if (sustainMode == 1){ // If proprietary sustain mode on...

			if (lastSustainNote != lastSendNote[i]){ // If last sustained note differs from the current pressed one...

				sendChord(lastSustainNote, 0, 1); // mute current chord. 0 in second param means "noteOff".
				lastSustainNote = lastSendNote[i]; // and saves the current sustained note.
				sustainFlag = 1; // turn sustained note on

			}else{ // If last sustained note and the current pressed one are the same...

				sustainFlag = !sustainFlag; // turn sustained note off.

			}//end if lastSustainNote...
        }//end if sustainMode


        sendChord(lastSendNote[i], 127 * sustainFlag, 1); //send the appropriate midi message.

        if (sustainFlag != 0){
        	osc_setNote(pressedNote);
        }else{
        	osc_setNote(0);
        }

    }else if(GETBUTTONSTATUS(teclas[i], buttonRise)){

    	if (sustainMode != 1){ // If proprietary sustain mode off...
			sendChord(lastSendNote[i], 0, 1); // 0 in second param means "noteOff".
			osc_setNote(0); // turn DAC off.
        }
    }

  }
}


void inputSaveChord(void){

uint8_t flag = 0;

	for (int i = 0; i <= 12; i++){

//		buttonState[i + IN_DO] = HAL_GPIO_ReadPin(notePort_list[i], notePin_list[i]);

//		if(!buttonState[i + IN_DO] && lastState[i + IN_DO]){       // If we read 1, and the last time we read 0, means button was just pressed
		if (GETBUTTONSTATUS(teclas[i], buttonFall)){

			//storing the selected chord:
			setChord(i);
			footKey = NORMAL;
			screenNum = POPUP_SAVED_CHORD;
			menu = CHORD_SCREEN; 				//return point to chordSelect().
			lastKey_pos = i; 					// to show in screen the foot key pressed.
			flag = 1;
			break; 								 //it must exit from function.
		} //end if getStatusButton
	} //end for

	if (!flag){


		if (GETBUTTONSTATUS(IN_LEFT, buttonFall)){
			footKey = NORMAL;
			screenNum = POPUP_CANCEL;
			menu = CHORD_SCREEN;
		}
	} //end if !flag

} //end inputSaveChord()

void chordSelect(void){

  uint8_t flag = 0;

  switch (chordSelect_cursor){

		case 0:

			if (GETBUTTONSTATUS(IN_DOWN, buttonFall)){
				chordSelect_cursor++;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_LEFT, buttonFall)){
				chord--;
				if (chord < 0)
					chord = 7;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_RIGHT, buttonFall)){
				chord++;
				if (chord > 7)
					chord = 0;
				flag = 1;
			}

		break;
		case 1:

			if (GETBUTTONSTATUS(IN_UP, buttonFall)){
				chordSelect_cursor--;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_DOWN, buttonFall)){
				chordSelect_cursor++;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_LEFT, buttonFall)){
				chordInv--;
				if (chordInv < 0)
					chordInv = 2;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_RIGHT, buttonFall)){
				chordInv++;
				if (chordInv > 2)
					chordInv = 0;
				flag = 1;
			}

		break;
		case 2:

			if (GETBUTTONSTATUS(IN_UP, buttonFall)){
				chordSelect_cursor--;
				flag = 1;
			}
			if (GETBUTTONSTATUS(IN_RIGHT, buttonFall)){
				footKey = SAVE_CHORD; //
				menu = RECUEST_FOOTKEY_SCREEN;
				screenNum = RECUEST_FOOTKEY_SCREEN;
			}

		break;
	} //fin switch chordSelect_cursor

  if (flag){        ///flag de impresion

      screenNum = CHORD_SCREEN;
  }
}


void tunneSelect(void){
	uint8_t flag = 0;

	if (GETBUTTONSTATUS(IN_LEFT, buttonFall)){
		firstDoTunning--;
		if (firstDoTunning < MIDI_C2)
			firstDoTunning = MIDI_B2;
		flag = 1;
	}
	if (GETBUTTONSTATUS(IN_RIGHT, buttonFall)){
		firstDoTunning++;
		if (firstDoTunning >MIDI_B2)
			firstDoTunning = MIDI_C2;
		flag = 1;
	}

	if (flag){
		screenNum = TUNNE_SCREEN;
	}
}


void sendChord(unsigned long bnotes, int velocity, int channel){

  uint8_t note1;
  uint8_t note2;
  uint8_t note3;

  note1 = (bnotes & 0x7f);
  note2 = (bnotes >> 8) & 0x7f;
  note3 = (bnotes >> 16) & 0x7f;

  midi_msg[0] = 0x09;	//0x09 == type event (noteOn).
  midi_msg[1] = 0x90 | 1;
  midi_msg[2] = note1;
  midi_msg[3] = velocity;
  MIDI_SendBuffer(midi_msg, 4);

  if (note2 && note3){

	  midi_msg[0] = 0x09;	//0x09 == type event (noteOn).
	  midi_msg[1] = 0x90 | 1;
	  midi_msg[2] = note2;
	  midi_msg[3] = velocity;
	  MIDI_SendBuffer(midi_msg, 4);

	  midi_msg[0] = 0x09;	//0x09 == type event (noteOn).
	  midi_msg[1] = 0x90 | 1;
	  midi_msg[2] = note3;
	  midi_msg[3] = velocity;
	  MIDI_SendBuffer(midi_msg, 4);
  }
} //fin sendChord()


void showLeds(void){

	uint16_t demora = 50;
	uint8_t posLed[] = {	LED_OCT_G,
							LED_OCT_M,
							LED_OCT_A,
							LED_PS0,
							LED_PS1,
							LED_PS2,
							LED_PS3,
							LED_PS4,
							LED_PS5,
							LED_SUST_P,
							LED_SUST_M,
							LED_CHORD };

	uint16_t indexLed = 0x1 << LED_OCT_G;
	indexLed = ~indexLed;
	spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
	HAL_Delay(demora);

	indexLed = (0x1 << posLed[0]) | (0x1 << posLed[1]);
	indexLed = ~indexLed;
	spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
	HAL_Delay(demora);

	for (uint8_t i=0; i<10; i++){
		indexLed = (0x1 << posLed[i]) | (0x1 << posLed[i + 1]) | (0x1 << posLed[i + 2]);
		indexLed = ~indexLed;
		spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
		HAL_Delay(demora);
	}

	indexLed = (0x1 << posLed[10]) | (0x1 << posLed[11]);
	indexLed = ~indexLed;
	spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
	HAL_Delay(demora);

	indexLed = 0x1 << LED_CHORD;
	indexLed = ~indexLed;
	spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
	HAL_Delay(demora);

	indexLed = 0xFFFF;
	spi_74HC595_Transmit((uint8_t *)&indexLed, 2/*bytes*/);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM3){ // dac
		flag_dac = 1;
	}else if (htim->Instance == TIM2){ //tick
		flag_tick = 1;
	}

}


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	_lcd_ISR();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	flag_adc = 1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
