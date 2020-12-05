/*
 * initialize.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

//#include "translation.h"
#include "global_variables.h"
#include "spi.h"
#include "tim.h"
#include "memory_storage_map.h"
#include "FM25L16B.h"
#include "leds.h"

void Init(void) {
  // set default values in case there's nothing in memory
  CAL_Data_t cal_data = {
      .acc0beta  = {0, 0, 0,
          2.4414e-004, 2.4414e-004, 2.4414e-004},
      .gyro0beta = {0, 0, 0},
      .mag0beta  = {0, 0, 0,
          0.000517, 0.000517, 0.000517},
  };
  Config_Data_t config_data = {
      .top_face_number   = 1,
      .front_face_number = 2,
  };

  FM25.SPI_Bus = &hspi1;
  FM25.cs_port = CS_FRAM0_GPIO_Port;
  FM25.cs_pin  = CS_FRAM0_Pin;

  // check memory for calibration values
  uint32_t data_version_code;
  MEM_ReadDataVersionCode(&data_version_code);
  if (data_version_code != DATA_VERSION_CODE) {
    MEM_WriteCalData(&cal_data);
    MEM_WriteConfigData(&config_data);

    data_version_code = DATA_VERSION_CODE;
    MEM_WriteDataVersionCode(&data_version_code);
  } else {
    MEM_ReadCalData(&cal_data);
    MEM_ReadConfigData(&config_data);
  }

  //set base_q
  Configuration_SetBaseQ(config_data.top_face_number, config_data.front_face_number, q0_base_rot);

  //imu initialization
  ICM0.SPI_Bus = &hspi1;
  ICM0.CS_Port = CS_ICM0_GPIO_Port;
  ICM0.CS_Pin = CS_ICM0_Pin;
  ICM0.AccSens = Accelerometer_8G;
  ICM0.GyroSens = Gyroscope_1000s;
  ICM0.GyroFilt = Gyroscope_176Hz;

  HAL_GPIO_WritePin(ICM0.CS_Port, ICM0.CS_Pin, GPIO_PIN_SET);

  HAL_Delay(100);
  while (ICM20602_Init(&ICM0, cal_data.acc0beta, cal_data.gyro0beta) != HAL_OK);


  //mmc initialization
  MMC0.SPI_Bus = &hspi1;
  MMC0.CS_Port = CS_MMC0_GPIO_Port;
  MMC0.CS_Pin = CS_MMC0_Pin;
  MMC0.ODR = ODR800;

  while (MMC5983MA_Init(&MMC0, cal_data.mag0beta) != HAL_OK);



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

  KalmanFilter_Init(&kf0, Q0_vals, R0_vals, P_post0_vals, dt);

  //translation pid initialization
//  TRANS_initPID();


  //1 kHz interrupt timer initialization
  HAL_TIM_Base_Start_IT(&htim1);

  //blinking leds
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
  LED_SetPWM(&led0, 1000, 127);
  LED_SetPWM(&led1, 100, 0);
  LED_SetPWM(&led2, 100, 0);
}
