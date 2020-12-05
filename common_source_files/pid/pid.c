/*
 * pid.c
 *
 *  Created on: Jul 8, 2018
 *      Author: Ben Freudberg
 */


#include "pid.h"

float pid_step(PID_t* pid, float error, float terms[3]) {
  float e_d = error - pid->e_p_last;

  pid->e_i = pid->e_i*.9999 + error;
  if (pid->i*pid->e_i > pid->i_max) pid->e_i = pid->i_max / pid->i;
  if (pid->i*pid->e_i < -pid->i_max) pid->e_i = -pid->i_max / pid->i;


  float p_term = pid->p*error;
  float i_term = pid->i*pid->e_i;
  float d_term = pid->d*e_d;

  if (terms) {
    terms[0] = p_term;
    terms[1] = i_term;
    terms[2] = d_term;
  }

  float result = p_term + i_term + d_term;
  pid->e_p_last = error;
  return result;
}

void pid_init(PID_t* pid, float p, float i, float d, float i_max) {
	pid->p = p;
	pid->i = i;
	pid->d = d;
	pid->e_p_last = 0;
	pid->e_i = 0;
	pid->i_max = i_max;
}

void pid_reset(PID_t* pid) {
	pid->e_p_last = 0;
	pid->e_i = 0;
}
