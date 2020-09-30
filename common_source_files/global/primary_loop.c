/*
 * primary_loop.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"
#include "tim.h"

volatile bool tim1_int = false;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    tim1_int = true;
  }
}

void primary_loop(void) {
  if (tim1_int) {
    tim1_int = false;
    float IMU_floats[7];
    ICM20602_Read(&ICM0, IMU_floats);
    //todo: continue here
  }
}
