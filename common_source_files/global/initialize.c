/*
 * initialize.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"
#include "spi.h"
#include "tim.h"

void Init(void) {
  float acc0beta[6] = {0, 0, 0,
      6.09e-005, 6.09e-005, 6.09e-005};
  float gyro0beta[3] = {0, 0, 0};
  float mag0beta[6] = {0, 0, 0,
      0.000517, 0.000517, 0.000517};
  //imu initialization
  ICM0.SPI_Bus = &hspi1;
  ICM0.CS_Port = CS_ICM0_GPIO_Port;
  ICM0.CS_Pin = CS_ICM0_Pin;
  ICM0.AccSens = Accelerometer_2G;
  ICM0.GyroSens = Gyroscope_250s;
  ICM0.GyroFilt = Gyroscope_176Hz;

  HAL_GPIO_WritePin(ICM0.CS_Port, ICM0.CS_Pin, GPIO_PIN_SET);

  HAL_Delay(100);
  while (ICM20602_Init(&ICM0, acc0beta, gyro0beta) != HAL_OK);


  //mmc initialization
  MMC0.SPI_Bus = &hspi1;
  MMC0.CS_Port = CS_MMC0_GPIO_Port;
  MMC0.CS_Pin = CS_MMC0_Pin;
  MMC0.ODR = ODR800;

  while (MMC5983MA_Init(&MMC0, mag0beta) != HAL_OK);



  //kalman filter initialization
  float32_t var0 = 1e-6;//9e-7; //higher means trust gyro less
  float32_t Q0_vals[16] = { 3*var0, -var0, -var0, -var0,
                        -var0, 3*var0, -var0, -var0,
              -var0, -var0, 3*var0, -var0,
              -var0, -var0, -var0, 3*var0, }; //differs from paper
  float32_t R0_vals[16] = { 3, 0, 0, 0,//.2
              0, 3, 0, 0,
              0, 0, 3, 0,
              0, 0, 0, 3 }; //differs from paper. Bigger numbers means trust acc/mag less
  float32_t P_post0_vals[16] = { 2, 0, 0, 0,
                 0, 2, 0, 0,
                 0, 0, 2, 0,
                 0, 0, 0, 2 };

  KalmanFilter_init(&KF0, Q0_vals, R0_vals, P_post0_vals, dt);


  //1 kHz interrupt timer initialization
  HAL_TIM_Base_Start_IT(&htim1);

  //blinking leds
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
//  HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
//  HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
}
