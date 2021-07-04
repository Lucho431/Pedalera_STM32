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
#define MIDI_TX_Pin GPIO_PIN_6
#define MIDI_TX_GPIO_Port GPIOC
#define MIDI_RX_Pin GPIO_PIN_7
#define MIDI_RX_GPIO_Port GPIOC
#define PL_bot_Pin GPIO_PIN_5
#define PL_bot_GPIO_Port GPIOD
#define CE_bot_Pin GPIO_PIN_6
#define CE_bot_GPIO_Port GPIOD
#define ST_leds_Pin GPIO_PIN_7
#define ST_leds_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
