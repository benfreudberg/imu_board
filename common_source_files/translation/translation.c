/*
 * translation.c
 *
 *  Created on: Dec 3, 2020
 *      Author: Fongberg
 */

const float g = 9.80665;


void TRANS_update(float curr_state[2][3], float const acc_vec_g[3], float dt, float const curr_rot_q[4]) {
  float acc_vec_mpss[3];
  for (int i = 0; i<3; i++) {
    acc_vec_mpss[i] = acc_vec_g[i] * g;
    curr_state[1][i] += curr_state[0][i]*dt + 0.5*acc_vec_mpss[i]*dt*dt;
    curr_state[0][i] += acc_vec_mpss[i] * dt;
  }
}

void TRANS_zero(float curr_state[2][3]) {
  for (int j=0; j<2; j++) {
    for (int i=0; i<3; i++) {
      curr_state[j][i] = 0;
    }
  }
}
