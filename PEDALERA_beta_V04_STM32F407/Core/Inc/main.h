/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USE_USB_FS 1
#define MIb_Pin GPIO_PIN_2
#define MIb_GPIO_Port GPIOE
#define LA_Pin GPIO_PIN_3
#define LA_GPIO_Port GPIOE
#define RE_Pin GPIO_PIN_4
#define RE_GPIO_Port GPIOE
#define LAb_Pin GPIO_PIN_5
#define LAb_GPIO_Port GPIOE
#define REb_Pin GPIO_PIN_6
#define REb_GPIO_Port GPIOE
#define SOL_Pin GPIO_PIN_13
#define SOL_GPIO_Port GPIOC
#define DO_Pin GPIO_PIN_14
#define DO_GPIO_Port GPIOC
#define SOLb_Pin GPIO_PIN_15
#define SOLb_GPIO_Port GPIOC
#define BOTON_AZUL_Pin GPIO_PIN_0
#define BOTON_AZUL_GPIO_Port GPIOA
#define LED_VERDE_Pin GPIO_PIN_12
#define LED_VERDE_GPIO_Port GPIOD
#define LED_NARANJA_Pin GPIO_PIN_13
#define LED_NARANJA_GPIO_Port GPIOD
#define LED_ROJO_Pin GPIO_PIN_14
#define LED_ROJO_GPIO_Port GPIOD
#define LED_AZUL_Pin GPIO_PIN_15
#define LED_AZUL_GPIO_Port GPIOD
#define CHORD_Pin GPIO_PIN_9
#define CHORD_GPIO_Port GPIOC
#define PRESET5_Pin GPIO_PIN_8
#define PRESET5_GPIO_Port GPIOA
#define PRESET4_Pin GPIO_PIN_9
#define PRESET4_GPIO_Port GPIOA
#define PRESET3_Pin GPIO_PIN_10
#define PRESET3_GPIO_Port GPIOA
#define PRESET2_Pin GPIO_PIN_15
#define PRESET2_GPIO_Port GPIOA
#define PRESET0_Pin GPIO_PIN_10
#define PRESET0_GPIO_Port GPIOC
#define PRESET1_Pin GPIO_PIN_11
#define PRESET1_GPIO_Port GPIOC
#define LEFT_Pin GPIO_PIN_12
#define LEFT_GPIO_Port GPIOC
#define RIGHT_Pin GPIO_PIN_0
#define RIGHT_GPIO_Port GPIOD
#define UP_Pin GPIO_PIN_1
#define UP_GPIO_Port GPIOD
#define DOWN_Pin GPIO_PIN_2
#define DOWN_GPIO_Port GPIOD
#define TUNNE_Pin GPIO_PIN_3
#define TUNNE_GPIO_Port GPIOD
#define SUST_MIDI_Pin GPIO_PIN_5
#define SUST_MIDI_GPIO_Port GPIOD
#define OCT_A_Pin GPIO_PIN_6
#define OCT_A_GPIO_Port GPIOD
#define SUST_PROP_Pin GPIO_PIN_7
#define SUST_PROP_GPIO_Port GPIOD
#define OCT_M_Pin GPIO_PIN_3
#define OCT_M_GPIO_Port GPIOB
#define DO8_Pin GPIO_PIN_4
#define DO8_GPIO_Port GPIOB
#define OCT_G_Pin GPIO_PIN_5
#define OCT_G_GPIO_Port GPIOB
#define FA_Pin GPIO_PIN_8
#define FA_GPIO_Port GPIOB
#define SI_Pin GPIO_PIN_9
#define SI_GPIO_Port GPIOB
#define MI_Pin GPIO_PIN_0
#define MI_GPIO_Port GPIOE
#define SIb_Pin GPIO_PIN_1
#define SIb_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
