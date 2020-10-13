/*
 * calibration.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"
#include "various_functions.h"

uint8_t cal_acc_state = 0;

static void cal_failed() {
//  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 5000);
//  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 5000);
//  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 5000);
//  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
//  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
//  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  while(1);
}

static void cal_gyro(int16_t* data, ICM20602* ICM) {
  float* beta = ICM->gyrobeta;
  for (int j = 0; j < 3; j++) {
    beta[j] = 0;
    for (int i = 0; i < 20; i++) {
      beta[j] += ((float) data[i*3 + j]) / 32768 * GYROSENS[ICM->GyroSens] / 180 * PI;
    }
    beta[j] = beta[j] / 20;
  }
}

void CAL_AccStep(void) {
  static int16_t dataset0[360];

  for (int i = 0; i<20; i++) {
    for (int j = 0; j<3; j++) {
    dataset0[i*3+j+(cal_acc_state*60)] = acc0data20[i][j];
    }
  }
  if (cal_acc_state == 0) {
    cal_gyro((int16_t*) gyro0data20, &ICM0);

    bool success = true;
    for (int i = 0; i<3; i++) {
      if (fabs(ICM0.gyrobeta[i]) > 0.1) {
        success = false;
        break;
      }
    }
    if (!success) {
      cal_failed();
    } else {
//    memcpy(VarDataTab+36, ICM0.gyrobeta, 12);
//    HAL_FLASH_Unlock();
//    for (int i = 36; i<54; i++) {
//      EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
//    }
//    HAL_FLASH_Lock();
    }
  }
  if (cal_acc_state == 5) {
    static float32_t dataset0_f32[360];

    for (int i = 0; i < 360; i++) {
      dataset0_f32[i] = ((float32_t) dataset0[i]);
    }

//    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    GaussNewton(dataset0_f32, ICM0.accbeta);

    bool success = true;
    for (int i = 0; i<3; i++) {
      if (fabs(ICM0.accbeta[i]) > 1000) {
        success = false;
        break;
      }
      if (ICM0.accbeta[i+3] > 6.2e-5) {
        success = false;
        break;
      }
      if (ICM0.accbeta[i+3] < 6e-5) {
        success = false;
        break;
      }
    }
    if (!success) {
      cal_failed();
    } else {
//    memcpy(VarDataTab+00, ICM0.accbeta, 24);
//    HAL_FLASH_Unlock();
//    for (int i = 0; i<36; i++) {
//      EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
//    }
//    HAL_FLASH_Lock();
    }
  }

  cal_acc_state++;
  cal_acc_state %= 6;
}

void CAL_Imu20BufferPush(int16_t IMU_raw[7]) {
  static int i = 0;
  for (int j = 0; j < 3; j++) {
    acc0data20[i][j]  = IMU_raw[j];
    gyro0data20[i][j] = IMU_raw[4 + j];
  }
  i++;
  i %= 20;
}
