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
#include "stm32g4xx_hal.h"

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
#define SWTCH0_Pin GPIO_PIN_0
#define SWTCH0_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_1
#define BTN1_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_2
#define BTN2_GPIO_Port GPIOA
#define BTN0_Pin GPIO_PIN_4
#define BTN0_GPIO_Port GPIOA
#define CS_ICM0_Pin GPIO_PIN_0
#define CS_ICM0_GPIO_Port GPIOB
#define CS_FRAM0_Pin GPIO_PIN_8
#define CS_FRAM0_GPIO_Port GPIOA
#define T8_2N_LED0_Pin GPIO_PIN_4
#define T8_2N_LED0_GPIO_Port GPIOB
#define T8_3N_LED1_Pin GPIO_PIN_5
#define T8_3N_LED1_GPIO_Port GPIOB
#define T4_1_LED2_Pin GPIO_PIN_6
#define T4_1_LED2_GPIO_Port GPIOB
#define CS_MMC0_Pin GPIO_PIN_7
#define CS_MMC0_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
