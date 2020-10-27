/*
 * state_interpretation.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Fongberg
 */


#include "state_interpretation.h"
#include "math_functions.h"

float q_zero[4];

void STATE_QtoYPR(float q[4], float ypr[3]) {
  float q_state[4];
  quatProd(q_state, q_zero, q);

}
