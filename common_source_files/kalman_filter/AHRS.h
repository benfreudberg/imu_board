/*
 * AHRS.h
 *
 *  Created on: Jun 12, 2018
 *      Author: Ben Freudberg
 */

#ifndef ahrs_h
#define ahrs_h

#include <arm_math.h>

typedef struct  {
	arm_matrix_instance_f32 Q; //Q Matrix 4x4 gyro variance (process)
	float32_t Q_vals[16];
	arm_matrix_instance_f32 R; //R Matrix 4x4 acc/mag variance (measurement) (tune for performance, not actual variance measurements)
	float32_t R_vals[16];
	//arm_matrix_instance_f32 P_pre; //P_pre Matrix 4x4 covariance
	//float32_t P_pre_vals[16];
	arm_matrix_instance_f32 P_post; //P_post Matrix 4x4 covariance
	float32_t P_post_vals[16];
	//float q_pre[4]; //predicted state quaternion
	float dt;
} KalmanFilter;


void accStep(float* qa, float* q, float* acc);
void magStep(float* qa, float* mag);
void KalmanFilter_init(KalmanFilter* KF, float* Q_init_vals, float* R_init_vals, float* P_post_init_vals, float dt);
void KalmanFilter_step(KalmanFilter* KF, float* qk, float* qak, float* gyro);

#endif
