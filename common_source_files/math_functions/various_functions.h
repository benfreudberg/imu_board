/*
 * various_functions.h
 *
 *  Created on: April 23, 2018
 *      Author: Ben Freudberg
 */

#ifndef various_functions_h
#define various_functions_h

#include "arm_math.h"
#include "hal_ICM20602.h"
//#include "hal_MMC5983MA.h"


//void imu_int_to_norm_float(float* output, int16_t* input, ICM20602* ICM);
//void mag_int_to_norm_float(float* output, int16_t* input, MMC5883MA* MMC);
//void cal_gyro(int16_t* data, ICM20602* ICM);
//void cal_acc();
//void cal_mag();
//void cal_failed();
//void gn_resids(float32_t* resids, float32_t* data, float32_t* beta);
//void gn_jacobian(float32_t* jacobian_data, float32_t* data, float32_t* beta);
//void gn_step(float32_t* data, float32_t* beta);
void GaussNewton(float32_t* data, float32_t* beta);
void quatProd(float* result, float* qa, float* qb);
void quatConj(float* result, float* q);
float vectMag(float* vec, int len);
void vectNormalize(float* vec, int len);
void vectCross(float* result, float* u, float* v);
float vectDot(float* u, float* v, int len);
float acos_nv(float x);
float asin_nv(float x);
void rel_rot(float* q_rel, float* qa, float* qb);
uint8_t check_rot(float* qa, float* qb);
void sin_cos(float32_t theta,float32_t * pSinVal,float32_t * pCosVal);

#endif
