/*
 * state_interpretation.h
 *
 *  Created on: Oct 24, 2020
 *      Author: Fongberg
 */

#ifndef STATE_INTERPRETATION_H_
#define STATE_INTERPRETATION_H_

#include <stdint.h>

void STATE_QtoYPR(float q[4], uint16_t ypr[3]);

#endif /* STATE_INTERPRETATION_H_ */
