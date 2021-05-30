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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define DO_Pin GPIO_PIN_2
#define DO_GPIO_Port GPIOE
#define DO8_Pin GPIO_PIN_3
#define DO8_GPIO_Port GPIOE
#define REb_Pin GPIO_PIN_4
#define REb_GPIO_Port GPIOE
#define OCT_G_Pin GPIO_PIN_5
#define OCT_G_GPIO_Port GPIOE
#define RE_Pin GPIO_PIN_6
#define RE_GPIO_Port GPIOE
#define MIb_Pin GPIO_PIN_8
#define MIb_GPIO_Port GPIOI
#define OCT_M_Pin GPIO_PIN_13
#define OCT_M_GPIO_Port GPIOC
#define MI_Pin GPIO_PIN_14
#define MI_GPIO_Port GPIOC
#define OCT_A_Pin GPIO_PIN_15
#define OCT_A_GPIO_Port GPIOC
#define FA_Pin GPIO_PIN_9
#define FA_GPIO_Port GPIOI
#define SUST_PROP_Pin GPIO_PIN_10
#define SUST_PROP_GPIO_Port GPIOI
#define SOLb_Pin GPIO_PIN_11
#define SOLb_GPIO_Port GPIOI
#define SUST_MIDI_Pin GPIO_PIN_0
#define SUST_MIDI_GPIO_Port GPIOF
#define SOL_Pin GPIO_PIN_1
#define SOL_GPIO_Port GPIOF
#define CHORD_Pin GPIO_PIN_2
#define CHORD_GPIO_Port GPIOF
#define LAb_Pin GPIO_PIN_3
#define LAb_GPIO_Port GPIOF
#define UP_Pin GPIO_PIN_4
#define UP_GPIO_Port GPIOF
#define LA_Pin GPIO_PIN_5
#define LA_GPIO_Port GPIOF
#define DOWN_Pin GPIO_PIN_6
#define DOWN_GPIO_Port GPIOF
#define SIb_Pin GPIO_PIN_7
#define SIb_GPIO_Port GPIOF
#define LEFT_Pin GPIO_PIN_8
#define LEFT_GPIO_Port GPIOF
#define SI_Pin GPIO_PIN_9
#define SI_GPIO_Port GPIOF
#define RIGHT_Pin GPIO_PIN_10
#define RIGHT_GPIO_Port GPIOF
#define PRESET0_Pin GPIO_PIN_0
#define PRESET0_GPIO_Port GPIOC
#define PRESET1_Pin GPIO_PIN_1
#define PRESET1_GPIO_Port GPIOC
#define PRESET2_Pin GPIO_PIN_2
#define PRESET2_GPIO_Port GPIOC
#define PRESET3_Pin GPIO_PIN_3
#define PRESET3_GPIO_Port GPIOC
#define TUNNE_Pin GPIO_PIN_2
#define TUNNE_GPIO_Port GPIOA
#define PRESET4_Pin GPIO_PIN_2
#define PRESET4_GPIO_Port GPIOH
#define PRESET5_Pin GPIO_PIN_3
#define PRESET5_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
