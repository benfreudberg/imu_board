/*
 * translation.c
 *
 *  Created on: Dec 3, 2020
 *      Author: Fongberg
 */

#include "pid.h"
#include "stddef.h"

const float g = 9.80665;
static PID_t pid[2][3];

void TRANS_initPID(void) {
  for (int i=0; i<3; i++) {
    pid_init(&pid[0][i], .00003, .000001, .0001, .01); //speed
    pid_init(&pid[1][i], .00003, .00000, 0, 1); //position
  }
}

void TRANS_update(float curr_state[2][3], float const acc_vec_g[3], float dt, float const curr_rot_q[4]) {
  float acc_vec_mpss[3];
  for (int i = 0; i<3; i++) {
    acc_vec_mpss[i] = acc_vec_g[i] * g;
    curr_state[1][i] += curr_state[0][i]*dt + 0.5*acc_vec_mpss[i]*dt*dt;
    curr_state[0][i] += acc_vec_mpss[i] * dt;

    curr_state[0][i] -= pid_step(&pid[0][i], curr_state[0][i], NULL);
    curr_state[0][i] -= pid_step(&pid[1][i], curr_state[1][i], NULL);
  }
}

void TRANS_zero(float curr_state[2][3]) {
  for (int j=0; j<2; j++) {
    for (int i=0; i<3; i++) {
      curr_state[j][i] = 0;
    }
  }
}
