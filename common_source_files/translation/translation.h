/*
 * translation.h
 *
 *  Created on: Dec 3, 2020
 *      Author: Fongberg
 */

#ifndef TRANSLATION_TRANSLATION_H_
#define TRANSLATION_TRANSLATION_H_

/*
 * acc_vec (in units of "g") should already be rotated into the correct coordinates and 1g in the z direction subtracted
 * state[0][] is velocity, state[1][] is position (units are m/s and m)
 */
void TRANS_update(float curr_state[2][3], float const acc_vec_g[3], float dt, float const curr_rot_q[4]);
void TRANS_zero(float curr_state[2][3]);
void TRANS_initPID(void);

#endif /* TRANSLATION_TRANSLATION_H_ */
