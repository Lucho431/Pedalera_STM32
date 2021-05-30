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
#include "dac.h"
#include "i2c.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_midi_if.h"
#include "SSpedalBPMFX.h"

#include "usbd_core.h"
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

typedef enum{
	_DO,
	_REb,
	_RE,
	_MIb,
	_MI,
	_FA,
	_SOLb,
	_SOL,
	_LAb,
	_LA,
	_SIb,
	_SI,
	_DO8,
	_OCT_G,
	_OCT_M,
	_OCT_A,
	_SUST_PROP,
	_SUST_MIDI,
	_CHORD,
	_UP,
	_DOWN,
	_LEFT,
	_RIGHT,
	_ENTER,
	_PRESET0,
	_PRESET1,
	_PRESET2,
	_PRESET3,
	_PRESET4,
	_PRESET5,
	_TUNNE,
	STRUCT_LENGTH,
}BUTTON_T;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned long lastSendNote[] =  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};   // vector para apagar la ultima nota mandada por el pedal (salva el error del octavador)
unsigned long lastSustainNote = 0;		//

uint8_t sustainFlag = 1;   /// flag to switch on/off a sustained note.

uint8_t sustainMode = 0; // 0: none, 1: prop, 2: midi.


/*
const uint16_t buttonList[] = {_DO_Pin, _REb_Pin, _RE_Pin, _MIb_Pin, _MI_Pin, _FA_Pin, _SOLb_Pin, _SOL_Pin, _LAb_Pin, _LA_Pin,
						_SIb_Pin, _SI_Pin, _DO8_Pin, OCT_G_Pin, OCT_M_Pin, OCT_A_Pin, SUST_PROP_Pin, SUST_MIDI_Pin,
						CHORD_Pin, UP_Pin, DOWN_Pin, LEFT_Pin, RIGHT_Pin, _ENTER_Pin, PRESET0_Pin,
						PRESET1_Pin, PRESET2_Pin, PRESET3_Pin, PRESET4_Pin, PRESET5_Pin, TUNNE_Pin};
*/

const uint16_t cursorPin_list[] = {UP_Pin, DOWN_Pin, LEFT_Pin, RIGHT_Pin};
GPIO_TypeDef* cursorPort_list[] = {UP_GPIO_Port, DOWN_GPIO_Port, LEFT_GPIO_Port, RIGHT_GPIO_Port};

const uint16_t presetPin_list[] = {PRESET0_Pin, PRESET1_Pin, PRESET2_Pin,
									PRESET3_Pin, PRESET4_Pin, PRESET5_Pin};
GPIO_TypeDef* presetPort_list[] = {PRESET0_GPIO_Port, PRESET1_GPIO_Port, PRESET2_GPIO_Port,
									PRESET3_GPIO_Port, PRESET4_GPIO_Port, PRESET5_GPIO_Port};
const uint16_t notePin_list[] = {DO_Pin, REb_Pin, RE_Pin, MIb_Pin, MI_Pin, FA_Pin, SOLb_Pin,
								SOL_Pin, LAb_Pin, LA_Pin, SIb_Pin, SI_Pin, DO8_Pin};
GPIO_TypeDef* notePort_list[] = {DO_GPIO_Port, REb_GPIO_Port, RE_GPIO_Port, MIb_GPIO_Port, MI_GPIO_Port,
								FA_GPIO_Port, SOLb_GPIO_Port, SOL_GPIO_Port, LAb_GPIO_Port, LA_GPIO_Port,
								SIb_GPIO_Port, SI_GPIO_Port, DO8_GPIO_Port};

uint8_t buttonState [STRUCT_LENGTH];
uint8_t lastState [STRUCT_LENGTH];

uint32_t last_GPIOB_IDR = 0;


SCREEN_NUM menu = MAIN_SCREEN;

uint8_t presetLed = 0; // flag to manage the presets leds. 0 value is the first.

uint8_t acorde = 0;	// flag to switch on/off the chord mode

uint8_t tunne = 0;		// flag to switch on/off the tunne mode

unsigned int d_pad;  //las flechitas pulsadas: 0-ninguna, 1-arriba, 2-abajo, 3-izquierda, 4-derecha.

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

volatile uint32_t contador_loop = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void inputCursor(void);
void inputSustains(void);
void inputChord(void);
void inputOctave(void);
void inputTunne(void);
void inputPresets(void);
void inputNotes(void);
void inputSaveChord(void);
void chordSelect(void);
void tunneSelect(void);
void sendChord(unsigned long, int, int);

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
  firstDoTunning = 48;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_DAC_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  SSD1306_Init();
  __turnOnScreen();
  __mainScreen();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  	switch (screenNum){
	  		case MAIN_SCREEN:
	  			__mainScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case CHORD_SCREEN:
	  			__chordScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case TUNNE_SCREEN:
	  			__tunneScreen();
	  			last_screenNum = screenNum;
	  		break;
	  		case RECUEST_FOOTKEY_SCREEN:
	  			__recuest_footkey();
	  			last_screenNum = screenNum; //this variable must store the CHORD_SCREEN value, to use it as return point.
	  		break;
	  		case POPUP_OCTAVE:
	  			__on_off_octave();
	  			popup_time = 2000;
	  		break;
	  		case POPUP_SAVED_CHORD:
	  			__pop_up_savedChord(lastKey_pos);
	  			last_screenNum = CHORD_SCREEN;
	  			popup_time = 2000;
	  		break;
	  		case POPUP_SUSTAIN:
	  			__pop_up_sustain();
	  			popup_time = 2000;
	  		break;
	  		case POPUP_CANCEL:
	  			__pop_up_cancel();
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


	  	inputCursor();
	  	inputSustains();
	  	inputChord();
	  	inputOctave();
	  	inputTunne();
	  	inputPresets();

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
	  			inputSaveChord();
	  		default:
	  		break;

	    }
	    MIDI_SendCheck();

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


void inputCursor(void){


	for (int i = 0; i < 4; i++){     //from the lowest CURSOR value, to the highest one.                                    // 4 presets en total.

	/*
	//BIT FIELD METHOD:
	buttonFlag = digitalRead(i + CURSOR_UP);

    //buttonState[1] = buttonFlag << (i + _UP);    //bit field method
	*/

	buttonState[i + _UP] = HAL_GPIO_ReadPin(cursorPort_list[i], cursorPin_list[i]);//digitalRead(i + CURSOR_UP);

    if(!buttonState[i + _UP] && lastState[i + _UP]){                                // If we read 0, and the last time we read 1, means button was just pressed

		d_pad = i + _UP;
        lastState[i + _UP] = buttonState[i + _UP];
		break;
    }else{
		d_pad = 0;
	}

    lastState[i + _UP] = buttonState[i + _UP];                                        // Update last button state.
  }
}


void inputSustains (void){

	buttonState[_SUST_PROP] = HAL_GPIO_ReadPin(SUST_PROP_GPIO_Port, SUST_PROP_Pin);

	if(!buttonState[_SUST_PROP] && lastState[_SUST_PROP]){

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
				MIDI_SendBuffer(midi_msg, 4);
			case 0:
				lastSustainNote = 0;
				sustainMode = 1; //prop
			break;

			case 1:
				sustainFlag = 1;
				sustainMode = 0;
			break;
		} //end switch

		screenNum = POPUP_SUSTAIN;
	}
	lastState[_SUST_PROP] = buttonState[_SUST_PROP];


	buttonState[_SUST_MIDI] = HAL_GPIO_ReadPin(SUST_MIDI_GPIO_Port, SUST_MIDI_Pin);

	if(!buttonState[_SUST_MIDI] && lastState[_SUST_MIDI]){

		switch (sustainMode){
			case 0:
			case 1:
				sustainFlag = 1;	// needed to normal notes functioning.
				//event = {0x0B, 0xB0 | MIDI_CHANNEL, 64, 127}; //64 == sustain.
				//MidiUSB.sendMIDI(event);
				midi_msg[0] = 0x0B;		//0x0B == type event (control change),
				midi_msg[1] = 0xB0 | 1;
				midi_msg[2] = 64;	//64 == sustain.
				midi_msg[3] = 127;
				MIDI_SendBuffer(midi_msg, 4);
				sustainMode = 2;
			break;
			case 2:
				//event = {0x0B, 0xB0 | MIDI_CHANNEL, 64, 0};
				//MidiUSB.sendMIDI(event);
				midi_msg[0] = 0x0B;	//0x0B == type event (control change)
				midi_msg[1] = 0xB0 | 1;
				midi_msg[2] = 64;	//64 == sustain.
				midi_msg[3] = 0;
				MIDI_SendBuffer(midi_msg, 4);
				sustainMode = 0;
			break;
		} //end switch

		screenNum = POPUP_SUSTAIN;
	}
	lastState[_SUST_MIDI] = buttonState[_SUST_MIDI];
}


void inputChord (void){

	buttonState[_CHORD] = HAL_GPIO_ReadPin(CHORD_GPIO_Port, CHORD_Pin);

	if(!buttonState[_CHORD] && lastState[_CHORD]){

      acorde = !acorde;

	  if (acorde){
		screenNum = CHORD_SCREEN;
		menu = CHORD_SCREEN;
	  }else{
		screenNum = MAIN_SCREEN;
		menu = MAIN_SCREEN;
	  }
	}
	/*
	if (acorde){
		chordSelect();
	}
*/
	lastState[_CHORD] = buttonState[_CHORD];
}


void inputOctave (void){

  buttonState[_OCT_G] = HAL_GPIO_ReadPin(OCT_G_GPIO_Port, OCT_G_Pin); //octava grave
  if(!buttonState[_OCT_G] && lastState[_OCT_G]){
      octava = -1;
	  screenNum=POPUP_OCTAVE;
  }

  lastState[_OCT_G] = buttonState[_OCT_G];


  buttonState[_OCT_M] = HAL_GPIO_ReadPin(OCT_M_GPIO_Port, OCT_M_Pin);//octava central
  if(!buttonState[_OCT_M] && lastState[_OCT_M]){
      octava = 0;
	  screenNum=POPUP_OCTAVE;
  }

  lastState[_OCT_M] = buttonState[_OCT_M];


  buttonState[_OCT_A] = HAL_GPIO_ReadPin(OCT_A_GPIO_Port, OCT_A_Pin);//octava aguda
  if(!buttonState[_OCT_A] && lastState[_OCT_A]){
      octava = 1;
      screenNum=POPUP_OCTAVE;
  }

  lastState[_OCT_A] = buttonState[_OCT_A];
}


void inputTunne(void){
  	buttonState[_TUNNE] = HAL_GPIO_ReadPin(TUNNE_GPIO_Port, TUNNE_Pin);

	if(!buttonState[_TUNNE] && lastState[_TUNNE]){
      tunne = !tunne;

	  if (tunne){
		screenNum = TUNNE_SCREEN;
		menu = TUNNE_SCREEN;
	  }else{
		screenNum = MAIN_SCREEN;
		menu = MAIN_SCREEN;
	  }
	}
	/*
	if (acorde){
		menu = TUNNE_SCREEN;
	}
*/
	lastState[_TUNNE] = buttonState[_TUNNE];
}


void inputPresets (void){

  for (int i = 0; i < 6; i++){                                         // 6 presets en total.

    buttonState[i + _PRESET0] = HAL_GPIO_ReadPin(presetPort_list[i], presetPin_list[i]);  //internal pullup

    if(!buttonState[i + _PRESET0] && lastState[i + _PRESET0]){                                // If we read 0, and the last time we read 1, means button was just pressed

        //MIDI.sendProgramChange(i - PRESET0, MIDI_CHANNEL);

		//event = {0x0C, 0xC0 | MIDI_CHANNEL, i, 0};
		//MidiUSB.sendMIDI(event);
		midi_msg[0] = 0x0C;	//0x0C == type event (program change)
		midi_msg[1] = 0xC0 | 1;
		midi_msg[2] = i;
		midi_msg[3] = 0;	//last param == 0 (unused in program change).
		MIDI_SendBuffer(midi_msg, 4);

    } //end if

    lastState[i + _PRESET0] = buttonState[i + _PRESET0];                                        // Update last button state.
  } //end for

} //end inputPresets


void inputNotes(void){

  for (int i = 0; i <= 12; i++){

    buttonState[i + _DO] = HAL_GPIO_ReadPin(notePort_list[i], notePin_list[i]);

    if(!buttonState[i + _DO] && lastState[i + _DO]){                                // If we read 1, and the last time we read 0, means button was just pressed

        if (acorde){
            lastSendNote[i] = makeChord(firstDoTunning + i + 12 * octava, i);

        }else{

            lastSendNote[i] = firstDoTunning + i + 12 * octava;
        }

        if (sustainMode == 1){

			if (lastSustainNote != lastSendNote[i]){

				sendChord(lastSustainNote, 0, 1);
				lastSustainNote = lastSendNote[i];
				sustainFlag = 1;

			}else{

				sustainFlag = !sustainFlag;

			}

        }





        sendChord(lastSendNote[i], 127 * sustainFlag, 1);



    }
    else if(buttonState[i] && !lastState[i]){                           // If we read 0, and the last time we read 1, means button was just released
        if (sustainMode != 1){
			sendChord(lastSendNote[i], 0, 1);           // 0 in second param meens "noteOff".
        }
    }

    lastState[i] = buttonState[i];                                        // Update last button state.
  }
}


void inputSaveChord(void){

uint8_t flag = 0;

	for (int i = 0; i <= 12; i++){

		buttonState[i + _DO] = HAL_GPIO_ReadPin(notePort_list[i], notePin_list[i]);

		if(!buttonState[i + _DO] && lastState[i + _DO]){       // If we read 1, and the last time we read 0, means button was just pressed

			//storing the selected chord:
			setChord(i);
			footKey = NORMAL;
			screenNum = POPUP_SAVED_CHORD;
			menu = CHORD_SCREEN; 				//return point to chordSelect().
			lastKey_pos = i; 					// to show in screen the foot key pressed.
			lastState[i] = buttonState[i];       // Update last button state.
			flag = 1;
			break; 								 //it must exit from function.
		}
		else if(buttonState[i] && !lastState[i]){                           // If we read 0, and the last time we read 1, means button was just released
			//POR AHORA NADA
		}
		lastState[i] = buttonState[i];                                        // Update last button state.
	}

	if (!flag){
		if (d_pad == _LEFT){
			footKey = NORMAL;
			screenNum = POPUP_CANCEL;
			menu = CHORD_SCREEN;
		}
	}
}

void chordSelect(void){

  uint8_t flag = 0;

  switch (chordSelect_cursor){

		case 0:

			switch (d_pad){
				case _DOWN:
					chordSelect_cursor++;
					flag = 1;
					break;
				case _LEFT:
					chord--;
					if (chord < 0)
						chord = 7;
					flag = 1;
					break;
				case _RIGHT:
					chord++;
					if (chord > 7)
						chord = 0;
					flag = 1;
					break;
			}
			break;
		case 1:

			switch (d_pad){
				case _UP:
					chordSelect_cursor--;
					flag = 1;
					break;
				case _DOWN:
					chordSelect_cursor++;
					flag = 1;
					break;
				case _LEFT:
					chordInv--;
					if (chordInv < 0)
						chordInv = 2;
					flag = 1;
					break;
				case _RIGHT:
					chordInv++;
					if (chordInv > 2)
						chordInv = 0;
					flag = 1;
					break;
			}
			break;
		case 2:

			switch (d_pad){
				case _UP:
					chordSelect_cursor--;
					flag = 1;
					break;
				case _RIGHT:
					footKey = SAVE_CHORD; //
					menu = RECUEST_FOOTKEY_SCREEN;
					screenNum = RECUEST_FOOTKEY_SCREEN;
					break;
			}
			break;
	}

  if (flag){        ///flag de impresion

      screenNum = CHORD_SCREEN;
  }
}


void tunneSelect(void){
	uint8_t flag = 0;

	switch (d_pad){
		case _RIGHT:
			firstDoTunning++;
			if (firstDoTunning >59)
				firstDoTunning = 48;
			flag = 1;
			break;
		case _LEFT:
			firstDoTunning--;
			if (firstDoTunning < 48)
				firstDoTunning = 59;
			flag = 1;
			break;
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
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (popup_time != 0){
		popup_time--;
		if (popup_time == 1) popup_timeUp = 1;
	}

	contador_loop = 0;
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
