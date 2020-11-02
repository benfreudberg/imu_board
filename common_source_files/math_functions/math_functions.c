/*
 * various_functions.c
 *
 *  Created on: April 23, 2018
 *      Author: Ben Freudberg
 */

#include "stm32g4xx_hal.h"
#include "math_functions.h"
#include "global_variables.h"
#include <math.h>

static void GaussNewton_Residuals(float32_t residuals[120], float32_t data[360], float32_t beta[6]) {
  for (int i = 0; i < 120; i++) {
    residuals[i] = 1;
    for (int j = 0; j < 3; j++) {
      residuals[i] -= beta[3 + j] * beta[3 + j] * (data[i*3 + j] - beta[j]) * (data[i*3 + j] - beta[j]);
    }
  }
}

static void GaussNewton_Jacobian(float32_t jacobian_data[360], float32_t data[360], float32_t beta[6]) {
  for (int i = 0; i < 120; i++) {
    for (int j = 0; j < 3; j++) {
      jacobian_data[i*6 + j] = 2*beta[3 + j] * beta[3+j] * (data[i*3 + j] - beta[j]);
      jacobian_data[i*6 + j + 3] = -2*beta[3 + j] * (data[i*3 + j] - beta[j]) * (data[i*3 + j] - beta[j]);
    }
  }
}

static void GaussNewton_Step(float32_t data[360], float32_t beta[6]) {
  static float32_t residuals[120];
  GaussNewton_Residuals(residuals, data, beta);
  arm_matrix_instance_f32 R;
  arm_mat_init_f32(&R, 120, 1, residuals);
  
  static float32_t J_f32[720];
  GaussNewton_Jacobian(J_f32, data, beta);
  arm_matrix_instance_f32 J;
  arm_mat_init_f32(&J, 120, 6, J_f32);
    
  static float32_t JT_f32[720];
  arm_matrix_instance_f32 JT;
  arm_mat_init_f32(&JT, 6, 120, JT_f32);
  arm_mat_trans_f32(&J, &JT);
  
  static float32_t JTJ_f32[36];
  arm_matrix_instance_f32 JTJ;
  arm_mat_init_f32(&JTJ, 6, 6, JTJ_f32);
  
  static float32_t JTJI_f32[36];
  arm_matrix_instance_f32 JTJI;
  arm_mat_init_f32(&JTJI, 6, 6, JTJI_f32);
  
  static float32_t JTJIJT_f32[720];
  arm_matrix_instance_f32 JTJIJT;
  arm_mat_init_f32(&JTJIJT, 6, 120, JTJIJT_f32);
  
  static float32_t D_f32[6];
  arm_matrix_instance_f32 D;
  arm_mat_init_f32(&D, 6, 1, D_f32);
  
  
  arm_mat_mult_f32(&JT, &J, &JTJ);
  arm_mat_inverse_f32(&JTJ, &JTJI);
  arm_mat_mult_f32(&JTJI, &JT, &JTJIJT);
  arm_mat_mult_f32(&JTJIJT, &R, &D);
  
  for (int i = 0; i < 6; i++) {
    beta[i] -= D_f32[i];
  }
}

void GaussNewton(float32_t data[360], float32_t beta[6]) {
  float32_t change = 100;
  int step = 0;
  while (change > 0.00004 && step++ < 100) {
    float32_t old_beta[6];
    for (int i = 0; i < 6; i++) {
      old_beta[i] = beta[i];
    }
    GaussNewton_Step(data, beta);
    change = 0;
    for (int i = 0; i < 6; i++) {
      change += fabs((beta[i] - old_beta[i])/old_beta[i]);
    }
  }
}

void quatProd(float result[4], float qa[4], float qb[4]) {
  float temp[4];
  temp[0] = qa[0]*qb[0] - qa[1]*qb[1] - qa[2]*qb[2] - qa[3]*qb[3];
  temp[1] = qa[0]*qb[1] + qa[1]*qb[0] + qa[2]*qb[3] - qa[3]*qb[2];
  temp[2] = qa[0]*qb[2] - qa[1]*qb[3] + qa[2]*qb[0] + qa[3]*qb[1];
  temp[3] = qa[0]*qb[3] + qa[1]*qb[2] - qa[2]*qb[1] + qa[3]*qb[0];
  for (int i = 0; i<4; i++) {
	  result[i] = temp[i];
  }
}

void quatConj(float result[4], float q[4]) {
  result[0] = q[0];
  result[1] = -q[1];
  result[2] = -q[2];
  result[3] = -q[3];
}

void vectRot(float q[4], float v[4]) {
  float q_inv[4], q_temp[4];
  quatConj(q_inv, q);
  quatProd(q_temp, q, v);
  quatProd(v, q_temp, q_inv);
}

float vectMag(float vec[], int len) {
  float sum_sqrs = 0;
  for (int i = 0; i < len; i++) {
	  sum_sqrs += vec[i]*vec[i];
  }
  float mag;
  arm_sqrt_f32(sum_sqrs, &mag);
  return mag;
}

void vectNormalize(float vec[], int len) {
  float mag = vectMag(vec, len);
  for (int i = 0; i < len; i++) {
    vec[i] = vec[i]/mag;
  }
}

void vectCross(float result[3], float u[3], float v[3]) {
  float temp[3];
  temp[0] = u[1]*v[2] - u[2]*v[1];
  temp[1] = u[2]*v[0] - u[0]*v[2];
  temp[2] = u[0]*v[1] - u[1]*v[0];
  for (int i = 0; i<3; i++) {
  	  result[i] = temp[i];
    }
}

float vectDot(float u[], float v[], int len) {
  float result = 0;
  for (int i = 0; i<len; i++) {
    result += u[i]*v[i];
  }
  return result;
}

float acos_nv(float x) {
  float negate = (x < 0);
  x = fabs(x);
  float ret = -0.0187293;
  ret = ret * x;
  ret = ret + 0.0742610;
  ret = ret * x;
  ret = ret - 0.2121144;
  ret = ret * x;
  ret = ret + 1.5707288;
  float sqrt_temp;
  arm_sqrt_f32(1.0-x, &sqrt_temp);
  ret = ret * sqrt_temp;
  ret = ret - 2 * negate * ret;
  return negate * 3.14159265358979 + ret;
}

float asin_nv(float x) {
  float negate = (x < 0);
  x = fabs(x);
  float ret = -0.0187293;
  ret = ret * x;
  ret = ret + 0.0742610;
  ret = ret * x;
  ret = ret - 0.2121144;
  ret = ret * x;
  ret = ret + 1.5707288;
  float sqrt_temp;
  arm_sqrt_f32(1.0-x, &sqrt_temp);
  ret = PI/2 - ret * sqrt_temp;
  return ret - 2 * negate * ret;
}

void rel_rot(float q_rel[4], float qa[4], float qb[4]) {
	float qa_conj[4];
	quatConj(qa_conj, qa);
	quatProd(q_rel, qa_conj, qb);
}

//theta in degrees
void sin_cos(float32_t theta, float32_t * pSinVal, float32_t * pCosVal) {
	if (fabs(theta) < 0.0000001) {
		*pSinVal = 0;
		*pCosVal = 1;
	}
	else arm_sin_cos_f32(theta, pSinVal, pCosVal);
}


