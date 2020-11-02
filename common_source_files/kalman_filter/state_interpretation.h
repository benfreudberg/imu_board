/*
 * state_interpretation.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Fongberg
 */

#ifndef STATE_INTERPRETATION_H_
#define STATE_INTERPRETATION_H_

#include <stdint.h>

void STATE_QtoYPR(float q[4], int16_t ypr[3]);
void STATE_SetZero(float q_zero[4], float q[4]);

#endif /* STATE_INTERPRETATION_H_ */
