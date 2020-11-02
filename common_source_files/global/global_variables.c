/*
 * global_variables.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "global_variables.h"

float dt = 0.001;
ICM20602 ICM0;
MMC5983MA MMC0;
FM25L16B_t FM25;
volatile uint8_t send_data_imu = 0;
volatile uint8_t send_data_mag = 0;
int16_t acc0data20[20][3];
int16_t gyro0data20[20][3];
int16_t mag0data20[20][3];
KalmanFilter kf0;
float q0[4] = {1, 0, 0, 0};
float q0_zero[4] = {1, 0, 0, 0};
