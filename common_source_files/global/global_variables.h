/*
 * global_variables.h
 *
 *  Created on: April 24, 2018
 *      Author: Ben Freudberg
 */

#include "kalman_filter.h"
#include "hal_ICM20602.h"
#include "hal_MMC5983MA.h"
#include "FM25L16B.h"
#include <stdbool.h>

extern float dt;
extern ICM20602 ICM0;
extern MMC5983MA MMC0;
extern FM25L16B_t FM25;
extern volatile uint8_t send_data_imu;
extern volatile uint8_t send_data_mag;
extern int16_t acc0data20[20][3];
extern int16_t gyro0data20[20][3];
extern int16_t mag0data20[20][3];
extern KalmanFilter kf0;
extern float q0[4];
extern float q0_yawzero[4];
extern float q0_base_rot[4];
extern float translation_state[2][3];

