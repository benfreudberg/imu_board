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
#include <stdbool.h>

#define SIGN(x)   x/fabs(x)

void GaussNewton(const float32_t* data, float32_t* beta);
void quatProd(float result[4], const float qa[4], const float qb[4]);
void quatConj(float result[4], const float q[4]);
void vectRot(float v_out[4], const float q[4], const float v_in[4]);
float vectMag(const float vec[], int len);
void vectNormalize(float vec[], int len);
void vectCross(float result[3], const float u[3], const float v[3]);
float vectDot(const float u[], const float v[], int len);
float acos_nv(float x);
float asin_nv(float x);
void rel_rot(float q_rel[4], const float qa[4], const float qb[4]);
void sin_cos(float32_t theta,float32_t * pSinVal,float32_t * pCosVal);

#endif
