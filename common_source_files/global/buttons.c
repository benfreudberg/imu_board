/*
 * buttons.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"
#include "calibration.h"
#include "main.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
    case BTN0_Pin :
      CAL_SetYawZero();
      __HAL_GPIO_EXTI_CLEAR_IT(BTN0_Pin);
      HAL_NVIC_ClearPendingIRQ(BTN0_EXTI_IRQn);
      break;
    case BTN1_Pin :
      if (cal_acc_state == 0) {
        // if BTN2 was already held down, do imu calibration
        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin)) {
          CAL_AccStep();
        }
      } else {
        CAL_AccStep();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(BTN1_Pin);
      HAL_NVIC_ClearPendingIRQ(BTN1_EXTI_IRQn);
      break;
    case BTN2_Pin :
      if (cal_mag_state == 0) {
        // if BTN1 was already held down, do mag calibration
        if (HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)) {
          CAL_MagStartStep();
        }
      } else {
        CAL_MagStartStep();
      }
      __HAL_GPIO_EXTI_CLEAR_IT(BTN2_Pin);
      HAL_NVIC_ClearPendingIRQ(BTN2_EXTI_IRQn);
      break;
  }
}
