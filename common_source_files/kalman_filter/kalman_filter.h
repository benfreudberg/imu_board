/*
 * kalman_filter.h
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
	arm_matrix_instance_f32 P_post; //P_post Matrix 4x4 covariance
	float32_t P_post_vals[16];
	float dt;
} KalmanFilter;


void KalmanFilter_AccStep(float qa[4], const float q[4], const float acc[3]);
void KalmanFilter_MagStep(float qa[4], const float mag[3]);
void KalmanFilter_Init(KalmanFilter* KF,
                       const float Q_init_vals[16],
                       const float R_init_vals[16],
                       const float P_post_init_vals[16],
                       float dt);
void KalmanFilter_Step(KalmanFilter* KF, float qk[4], float qak[4], const float gyro[3]);

#endif
