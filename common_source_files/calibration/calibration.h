/*
 * calibration.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Fongberg
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <stdint.h>

extern uint8_t cal_acc_state;

void CAL_Imu20BufferPush(int16_t IMU_raw[7]);
void CAL_AccStep(void);

#endif /* CALIBRATION_H_ */
