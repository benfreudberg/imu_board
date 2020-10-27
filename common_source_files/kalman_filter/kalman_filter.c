/*
 * kalman_filter.c
 *
 *  Created on: Jun 12, 2018
 *      Author: Ben Freudberg
 */

#include "kalman_filter.h"
#include "math_functions.h"
#include <math.h>
#include <arm_math.h>

void KalmanFilter_AccStep(float qa[4], float q[4], float acc[3]) {
  float gN[3] = {0, 0, 1};
  float ua = 0.9;
  float qg[4] = {0, 0, 0, 0};
  for (int i = 0; i < 3; i++) {
    qg[i+1] = acc[i];
  }

  float qtemp[4];
  float qconj[4];
  quatConj(qconj, q);
  quatProd(qtemp, qg, qconj);
  float qgn[4];
  quatProd(qgn, q, qtemp);
  vectNormalize(qgn, 4);

  float na[3];
  vectCross(na, qgn+1, gN);
  vectNormalize(na, 3);

  float dot = vectDot(qgn+1 ,gN, 3);
  if (dot > 1) dot = 1;
  if (dot < -1) dot = -1;
  float dtheta = acos_nv(dot);
  float cosd2;
  float sind2;
  sin_cos(ua*dtheta/2.0*180/PI, &sind2, &cosd2);
  float qae[4];
  qae[0] = cosd2;
  for (int i = 0; i <3; i++) {
    qae[i+1] = na[i]*sind2;
  }
  quatProd(qa, qae, q);
  vectNormalize(qa, 4);
}

void KalmanFilter_MagStep(float qa[4], float mag[3]) {
  float mN[3] = {.22892, .97345, 0};
  float qm[4] = {0, 0, 0, 0};
  for (int i = 0; i < 3; i++) {
    qm[i+1] = mag[i];
  }

  float qtemp[4];
  float qaconj[4];
  quatConj(qaconj, qa);
  quatProd(qtemp, qm, qaconj);
  float vmq[4];
  quatProd(vmq, qa, qtemp);
  vectNormalize(vmq, 4);

  float vmxy[3] = {0, 0, 0};
  float sqrt_temp;
  arm_sqrt_f32(vmq[1]*vmq[1] + vmq[2]*vmq[2], &sqrt_temp);
  vmxy[0] = vmq[1]/sqrt_temp;
  vmxy[1] = vmq[2]/sqrt_temp;

  float nm[3];
  vectCross(nm, vmxy, mN);
  vectNormalize(nm, 3);

  float dot = vectDot(vmxy,mN,3);
  if (dot > 1) dot = 1;
  if (dot < -1) dot = -1;
  float dtheta = acos_nv(dot);
  float cosd2;
  float sind2;
  sin_cos(dtheta/2.0*180/PI, &sind2, &cosd2);
  float qme[4];
  qme[0] = cosd2;
  for (int i = 0; i <3; i++) {
    qme[i+1] = nm[i]*sind2;
  }
  quatProd(qa, qme, qa);
  vectNormalize(qa, 4);
}

void KalmanFilter_Init(KalmanFilter* KF, float Q_init_vals[16], float R_init_vals[16], float P_post_init_vals[16], float dt) {
	for (int i = 0; i < 16; i++) {
		KF->Q_vals[i] = Q_init_vals[i];
		KF->R_vals[i] = R_init_vals[i];
		KF->P_post_vals[i] = P_post_init_vals[i];
	}
	KF->dt = dt;

	arm_mat_init_f32(&(KF->Q), 4, 4, KF->Q_vals);
	arm_mat_init_f32(&(KF->R), 4, 4, KF->R_vals);
	arm_mat_init_f32(&(KF->P_post), 4, 4, KF->P_post_vals);

}

void KalmanFilter_Step(KalmanFilter* KF, float qk[4], float qak[4], float gyro[3]) {
	float32_t q_pre[4];
	arm_matrix_instance_f32 q_pre_vect;
	arm_matrix_instance_f32 qk_vect;
	arm_matrix_instance_f32 qak_vect;
	arm_matrix_instance_f32 P_pre; //P_pre Matrix 4x4 covariance
	float32_t P_pre_vals[16];
	arm_matrix_instance_f32 F;
	float32_t F_vals[16];
	arm_matrix_instance_f32 K;
	float32_t K_vals[16];

	F_vals[0] = F_vals[5] = F_vals[10] = F_vals[15] = 1;
	F_vals[1] = F_vals[14] = -gyro[0]*.5*(KF->dt); //-wx
	F_vals[2] = F_vals[7]  = -gyro[1]*.5*(KF->dt); //-wy
	F_vals[3] = F_vals[9]  = -gyro[2]*.5*(KF->dt); //-wz
	F_vals[4] = F_vals[11] =  gyro[0]*.5*(KF->dt); //wx
	F_vals[8] = F_vals[13] =  gyro[1]*.5*(KF->dt); //wy
	F_vals[12] = F_vals[6] =  gyro[2]*.5*(KF->dt); //wz
	arm_mat_init_f32(&F, 4, 4, F_vals);

	arm_mat_init_f32(&P_pre, 4, 4, P_pre_vals);
	arm_mat_init_f32(&K, 4, 4, K_vals);
	arm_mat_init_f32(&q_pre_vect, 4, 1, q_pre);
	arm_mat_init_f32(&qk_vect, 4, 1, qk);
	arm_mat_init_f32(&qak_vect, 4, 1, qak);

	arm_mat_mult_f32(&F, &qk_vect, &q_pre_vect); //calc q_pre

	arm_matrix_instance_f32 Ft;
	float32_t Ft_vals[16];
	arm_mat_init_f32(&Ft, 4, 4, Ft_vals);
	arm_mat_trans_f32(&F, &Ft); //create F'

	arm_matrix_instance_f32 Temp;
	float32_t Temp_vals[16];
	arm_mat_init_f32(&Temp, 4, 4, Temp_vals); //create temp 4x4 matrix for calculation steps

	arm_matrix_instance_f32 Temp2;
	float32_t Temp2_vals[16];
	arm_mat_init_f32(&Temp2, 4, 4, Temp2_vals); //create a second temp 4x4 matrix for calculation steps

	arm_mat_mult_f32(&F, &(KF->P_post), &Temp);
	arm_mat_mult_f32(&Temp, &Ft, &Temp);
	arm_mat_add_f32(&Temp, &(KF->Q), &P_pre); //calc P_pre

	arm_mat_add_f32(&P_pre, &(KF->R), &Temp);
	arm_mat_inverse_f32(&Temp, &Temp2); //gives identity??
	arm_mat_mult_f32(&P_pre, &Temp2, &K); //calc K

	arm_matrix_instance_f32 Tempv;
	float32_t Tempv_vals[4];
	arm_mat_init_f32(&Tempv, 4, 1, Tempv_vals); //create temp 4x1 vector for calculation steps

	arm_mat_sub_f32(&qak_vect, &q_pre_vect, &Tempv);
	arm_mat_mult_f32(&K, &Tempv, &Tempv);
	arm_mat_add_f32(&q_pre_vect, &Tempv, &qk_vect);
	vectNormalize(qk, 4); //calc new q

	arm_matrix_instance_f32 Eye4;
	float32_t eye4_vals[16] = {	1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, 0, 1 };
	arm_mat_init_f32(&Eye4, 4, 4, eye4_vals);

	arm_mat_sub_f32(&Eye4, &K, &Temp);
	arm_mat_mult_f32(&Temp, &P_pre, &(KF->P_post)); //calc new P_post
}
