/*
 * calibration.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"
#include "various_functions.h"
#include "memory_storage_map.h"
#include "leds.h"

uint8_t cal_acc_state = 0;
uint8_t cal_mag_state = 0;
volatile bool collect_mag_data_b = false;

static void cal_failed() {
  LED_SetPWM(&led0, 200, 127);
  LED_SetPWM(&led1, 200, 127);
  LED_SetPWM(&led2, 200, 127);
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

static bool check_rot_since_last_data(float qa[4], float qb[4]) {
  bool ret_val = false;
  float q_rel[4];
  rel_rot(q_rel, qa, qb);
  float ang = 2*acos_nv(q_rel[0]);
  if (ang > 0.3307) ret_val = true;
  return ret_val;
}

void CAL_AccStep(void) {
  static int16_t dataset0[360];

  LED_SetPWM(&led1, 1000, 0xff * (cal_acc_state + 1) / 6);

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
      MEM_WriteGyro0CalData(ICM0.gyrobeta);
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
      MEM_WriteAcc0CalData(ICM0.accbeta);
    }
    LED_SetPWM(&led1, 1000, 0);
  }

  cal_acc_state++;
  cal_acc_state %= 6;
}

void CAL_MagStartStep(void) {
  LED_SetPWM(&led2, 1000, 0xff * (cal_mag_state + 1) / 6);
  collect_mag_data_b = true;
}

static void CAL_MagStep(void) {
  static int16_t dataset0[360];

  for (int i = 0; i<60; i++) {
    dataset0[i+(cal_mag_state*60)] = *(((int16_t*) mag0data20)+i);
  }

  if (cal_mag_state == 5) {
  static float32_t dataset0_f32[360];
    for (int i = 0; i < 360; i++) {
      dataset0_f32[i] = ((float32_t) dataset0[i]);
    }

    GaussNewton(dataset0_f32, MMC0.beta);

    bool results_valid = true;
    for (int i = 0; i<3; i++) {
      if (fabs(MMC0.beta[i]) > 1000) {
        results_valid = false;
        break;
      }
      if (MMC0.beta[i+3] > 5.8e-4) {
        results_valid = false;
        break;
      }
      if (MMC0.beta[i+3] < 4.0e-4) { //was 4.8
        results_valid = false;
        break;
      }
    }
    if (!results_valid) {
      cal_failed();
    } else {
      MEM_WriteMmc0CalData(MMC0.beta);
    }
    LED_SetPWM(&led2, 1000, 0);
  }

  cal_mag_state++;
  cal_mag_state %= 6;
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

void CAL_Mag20BufferPush(int16_t MAG_raw[3]) {
  static uint8_t mag_count = 0;
  static float q_last[4] = {1, 0, 0, 0};
  LED_SetPWM(&led2, 100, 127);

  bool save_mag_b = check_rot_since_last_data(q_last, q0);
  if (save_mag_b) {
    for (int i = 0; i<4; i++) {
      q_last[i] = q0[i];
    }
  }

  if (save_mag_b) {
    for (int i = 0; i < 3; i++) {
      mag0data20[mag_count][i] = MAG_raw[i];
    }
    mag_count++;
  }

  if (mag_count == 20) {
    mag_count = 0;
    collect_mag_data_b = false;
    LED_SetPWM(&led2, 1000, 0xff * (cal_mag_state + 1) / 6);
    CAL_MagStep();
  }
}
