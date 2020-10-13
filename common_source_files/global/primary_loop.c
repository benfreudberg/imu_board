/*
 * primary_loop.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "calibration.h"
#include "global_variables.h"
#include "tim.h"

volatile bool tim1_int = false;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    tim1_int = true;
  }
}

void primary_loop(void) {
  static bool mmc_data = true;
  static int time = 0;

  if (tim1_int) {
    tim1_int = false;
    mmc_data = !mmc_data;
    float IMU_floats[7], MMC_floats[3];
    int16_t IMU_raw[7], MMC_raw[3];

    ICM20602_Read(&ICM0, IMU_floats, IMU_raw);
    if (time % 5 == 0) {
      CAL_Imu20BufferPush(IMU_raw);
    }

    if (mmc_data) {
      MMC5983MA_Read(&MMC0, MMC_floats, MMC_raw);
    } else {
      MMC5983MA_Start(&MMC0);
    }

    //todo: continue here
    time++;
    time %= 1000;
  }
}
