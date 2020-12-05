/*
 * pid.h
 *
 *  Created on: Jul 8, 2018
 *      Author: Ben Freudberg
 */

/*
 * Instructions
 * 1. Declare a static or global PID object.
 * 2. Feed a pointer to the PID object along with desired P, I, D, and I_MAX terms to pid_int().
 * 3. On each loop iteration, run pid_step() feeding in the error relative to your setpoint.
 * 		"terms" is a pointer to the first float in an array of length 3 that will be filled with
 * 		the calculated p, i, and d contributions to the final output (used for debugging/tuning)
 */

#ifndef PID_H_
#define PID_H_

typedef struct _PID_t {
	float p;
	float i;
	float d;
	float e_p_last;
	float e_i;
	float i_max;
} PID_t;

float pid_step(PID_t* pid, float error, float * terms);
void pid_init(PID_t* pid, float p, float i, float d, float i_max);
void pid_reset(PID_t* pid);

#endif /* PID_H_ */
