/*
 * calibration.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Fongberg
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <stdint.h>
#include <stdbool.h>

extern uint8_t cal_acc_state;
extern uint8_t cal_mag_state;
extern volatile bool collect_mag_data_b;

void CAL_Imu20BufferPush(int16_t IMU_raw[7]);
void CAL_Mag20BufferPush(int16_t MAG_raw[3]);
void CAL_AccStep(void);
void CAL_MagStartStep(void);

#endif /* CALIBRATION_H_ */
