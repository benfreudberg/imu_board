/*
 * global_variables.h
 *
 *  Created on: April 24, 2018
 *      Author: Ben Freudberg
 */

#include "ahrs.h"
#include "hal_ICM20602.h"
#include "hal_MMC5983MA.h"
#include <stdbool.h>

extern float dt;
extern ICM20602 ICM0;
extern MMC5983MA MMC0;
//extern int16_t IMU0_data[7];
//extern int16_t MMC0_data[3];
//extern float IMU0_floats[7];
//extern float MMC0_floats[3];
extern volatile uint8_t send_data_imu;
extern volatile uint8_t send_data_mag;
extern int16_t acc0data20[20][3];
extern int16_t gyro0data20[20][3];
extern int16_t mag0data20[20][3];
extern KalmanFilter kf0;
extern float q0[4];

