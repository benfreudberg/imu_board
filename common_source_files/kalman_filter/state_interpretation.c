/*
 * state_interpretation.c
 *
 *  Created on: Oct 24, 2020
 *      Author: Fongberg
 */


#include "state_interpretation.h"
#include "math_functions.h"
#include "global_variables.h"

void STATE_QtoYPR(float q[4], int16_t ypr[3]) {
  float q_state[4], ypr_f[3], sin, cos;
  quatProd(q_state, q0_base_rot, q);
  quatProd(q_state, q0_yawzero, q_state);

  //yaw
  float q_v0[4] = {0, 0, 1, 0}; //forward vector
  vectRot(q_state, q_v0);
  ypr_f[0] = atan2(-q_v0[1], q_v0[2]);

  //pitch
  sin_cos(ypr_f[0]/2*180/PI, &sin, &cos);
  float q_remove_y[4] = {cos, 0, 0, -sin};
  quatProd(q_state, q_remove_y, q_state);
  float q_v1[4] = {0, 0, 1, 0}; //forward vector
  vectRot(q_state, q_v1);
  ypr_f[1] = atan2(q_v1[3], q_v1[2]);

  //roll
  sin_cos(ypr_f[1]/2*180/PI, &sin, &cos);
  float q_remove_p[4] = {cos, -sin, 0, 0};
  quatProd(q_state, q_remove_p, q_state);
  float q_v2[4] = {0, 0, 0, 1}; //up vector
  vectRot(q_state, q_v2);
  ypr_f[2] = atan2(q_v2[1], q_v2[3]);

  ypr[0] = (int16_t)(ypr_f[0] * 32767 / PI);
  ypr[1] = (int16_t)(ypr_f[1] * 32767 / PI);
  ypr[2] = (int16_t)(ypr_f[2] * 32767 / PI);
}

