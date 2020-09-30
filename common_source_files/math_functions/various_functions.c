/*
 * various_functions.c
 *
 *  Created on: April 23, 2018
 *      Author: Ben Freudberg
 */

#include "stm32g4xx_hal.h"
#include "various_functions.h"
#include "global_variables.h"
#include <math.h>


extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim3;


//void imu_int_to_norm_float(float* output, int16_t* input, ICM20602* ICM) {
//  float* beta_acc = ICM->accbeta;
//  float* beta_gyro = ICM->gyrobeta;
//  for (int i = 0; i < 3; i++) {
//    // add offset, multiply by scale -> output in g's
//    *(output + i) = (((float) (*(input + i))) - (*(beta_acc + i))) * (*(beta_acc + i + 3));
//  }
//  vectNormalize(output, 3);
//
//  *(output + 3) = (*(input + 3)) / 326.8 + 25; //degrees C
//
//  for (int i = 4; i < 7; i++) {
//    // scale to rad/sec (from 250d/s) and then add offset
//    *(output + i) = ((float) (*(input + i))) / 32768 * 250 / 180 * PI  - (*(beta_gyro + i - 4));
//  }
//}

//void mag_int_to_norm_float(float* output, int16_t* input, MMC5883MA* MMC) {
//  float* beta = MMC->beta;
//  float temp[3];
//  for (int i = 0; i < 3; i++) {
//    // add offset, multiply by scale -> output normalized
//	  temp[i] = (((float) (*(input + i))) - (*(beta + i))) * (*(beta + i + 3));
//  }
//  // fix axes to match imu
//  *(output + 0) = temp[1];
//  *(output + 1) = -temp[0];
//  *(output + 2) = -temp[2];
//  vectNormalize(output, 3);
//}

void cal_gyro(int16_t* data, ICM20602* ICM) {
  float* beta = ICM->gyrobeta;
  for (int j = 0; j < 3; j++) {
    *(beta + j) = 0;
    for (int i = 0; i < 20; i++) {
      *(beta + j) += ((float) (*(data + i*3 + j))) / 32768 * 250 / 180 * PI   * 1/20;
    }
  }
}

//void cal_acc() {
//  static uint8_t cal_state = 0;
//  static int16_t dataset0[360];
//  static int16_t dataset1[360];
//  static int16_t dataset2[360];
//
//  for (int i = 0; i<60; i++) {
//    dataset0[i+(cal_state*60)] = *(((int16_t*) acc0data20)+i);
//    dataset1[i+(cal_state*60)] = *(((int16_t*) acc1data20)+i);
//    dataset2[i+(cal_state*60)] = *(((int16_t*) acc2data20)+i);
//  }
//  if (cal_state == 0) {
//    cal_gyro((int16_t*) gyro0data20, &ICM0);
//    cal_gyro((int16_t*) gyro1data20, &ICM1);
//    cal_gyro((int16_t*) gyro2data20, &ICM2);
//
//    uint8_t results_test = 1;
//    for (int i = 0; i<3; i++) {
//    	if ((fabs(ICM0.gyrobeta[i]) > 0.1) || (fabs(ICM1.gyrobeta[i]) > 0.1) || (fabs(ICM2.gyrobeta[i]) > 0.1)) {
//    		results_test = 0;
//    		break;
//    	}
//    }
//    if (results_test == 0) {
//    	cal_failed();
//    } else {
//		memcpy(VarDataTab+36, ICM0.gyrobeta, 12);
//		memcpy(VarDataTab+42, ICM1.gyrobeta, 12);
//		memcpy(VarDataTab+48, ICM2.gyrobeta, 12);
//		HAL_FLASH_Unlock();
//		for (int i = 36; i<54; i++) {
//			EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
//		}
//		HAL_FLASH_Lock();
//    }
//  }
//  if (cal_state == 5) {
//	static float32_t dataset0_f32[360];
//    static float32_t dataset1_f32[360];
//    static float32_t dataset2_f32[360];
//    for (int i = 0; i < 360; i++) {
//      dataset0_f32[i] = ((float32_t) dataset0[i]);
//      dataset1_f32[i] = ((float32_t) dataset1[i]);
//      dataset2_f32[i] = ((float32_t) dataset2[i]);
//    }
//
//    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
//    gn(dataset0_f32, ICM0.accbeta);
//    gn(dataset1_f32, ICM1.accbeta);
//    gn(dataset2_f32, ICM2.accbeta);
//
//    uint8_t results_test = 1;
//    for (int i = 0; i<3; i++) {
//    	if ((fabs(ICM0.accbeta[i]) > 1000) /*|| (fabs(ICM1.accbeta[i]) > 1000)*/ || (fabs(ICM2.accbeta[i]) > 1000)) {
//    		results_test = 0;
//    		break;
//    	}
//    	if ((ICM0.accbeta[i+3] > 6.2e-5) /*|| (ICM1.accbeta[i+3] > 6.2e-5)*/ || (ICM2.accbeta[i+3] > 6.2e-5)) {
//    		results_test = 0;
//    		break;
//    	}
//    	if ((ICM0.accbeta[i+3] < 6e-5) /*|| (ICM1.accbeta[i+3] < 6e-5)*/ || (ICM2.accbeta[i+3] < 6e-5)) {
//    		results_test = 0;
//    		break;
//    	}
//    }
//    if (results_test == 0) {
//    	cal_failed();
//    } else {
//		memcpy(VarDataTab+00, ICM0.accbeta, 24);
//		memcpy(VarDataTab+12, ICM1.accbeta, 24);
//		memcpy(VarDataTab+24, ICM2.accbeta, 24);
//		HAL_FLASH_Unlock();
//		for (int i = 0; i<36; i++) {
//			EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
//		}
//		HAL_FLASH_Lock();
//    }
//  }
//
//  cal_state++;
//  cal_state %= 6;
//}
//
//void cal_mag() {
//  static uint8_t cal_state = 0;
//  static int16_t dataset0[360];
//  static int16_t dataset1[360];
//
//  for (int i = 0; i<60; i++) {
//	  dataset0[i+(cal_state*60)] = *(((int16_t*) mag0data20)+i);
//	  dataset1[i+(cal_state*60)] = *(((int16_t*) mag1data20)+i);
//  }
//
//  if (cal_state == 5) {
//	static float32_t dataset0_f32[360];
//	static float32_t dataset1_f32[360];
//    for (int i = 0; i < 360; i++) {
//      dataset0_f32[i] = ((float32_t) dataset0[i]);
//      dataset1_f32[i] = ((float32_t) dataset1[i]);
//    }
//
//    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
//    gn(dataset0_f32, MMC0.beta);
//    gn(dataset1_f32, MMC1.beta);
//
//    uint8_t results_test = 1;
//    for (int i = 0; i<3; i++) {
//    	if ((fabs(MMC0.beta[i]) > 1000) || (fabs(MMC1.beta[i]) > 1000)) {
//    		results_test = 0;
//    		break;
//    	}
//    	if ((MMC0.beta[i+3] > 5.8e-4) || (MMC1.beta[i+3] > 5.8e-4)) {
//    		results_test = 0;
//    		break;
//    	}
//    	if ((MMC0.beta[i+3] < 4.8e-4) || (MMC1.beta[i+3] < 4.8e-4)) {
//    		results_test = 0;
//    		break;
//    	}
//    }
//    if (results_test == 0) {
//    	cal_failed();
//    } else {
//		memcpy(VarDataTab+54, MMC0.beta, 24);
//		memcpy(VarDataTab+66, MMC1.beta, 24);
//		HAL_FLASH_Unlock();
//		for (int i = 54; i<78; i++) {
//			EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
//		}
//		HAL_FLASH_Lock();
//    }
//  }
//
//  cal_state++;
//  cal_state %= 6;
//}

void cal_failed() {
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 5000);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 5000);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 5000);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 5000);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
//	HAL_TIM_Base_Stop_IT(&htim3);
}

void gn_resids(float32_t* resids, float32_t* data, float32_t* beta) {
  for (int i = 0; i < 120; i++) {
    *(resids + i) = 1;
    for (int j = 0; j < 3; j++) {
      *(resids + i) -= (*(beta+3+j)) * (*(beta+3+j)) * ((*(data+i*3+j)) - (*(beta+j))) * ((*(data+i*3+j)) - (*(beta+j)));
    }
  }
}

void gn_jacobian(float32_t* jacobian_data, float32_t* data, float32_t* beta) {
  for (int i = 0; i < 120; i++) {
    for (int j = 0; j < 3; j++) {
      *(jacobian_data + i*6 + j) = 2*(*(beta+3+j)) * (*(beta+3+j)) * ((*(data + i*3 + j)) - (*(beta+j)));
      *(jacobian_data + i*6 + j+3) = -2*(*(beta+3+j)) * ((*(data + i*3 + j)) - (*(beta+j))) * ((*(data + i*3 + j)) - (*(beta+j)));
    }
  }
}

void gn_step(float32_t* data, float32_t* beta) {
  static float32_t resids[120];
  gn_resids(resids, data, beta);
  arm_matrix_instance_f32 R;
  arm_mat_init_f32(&R, 120, 1, resids);
  
  static float32_t J_f32[720];
  gn_jacobian(J_f32, data, beta);
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
    *(beta + i) -= *(D_f32 + i);
  }
}

void gn(float32_t* data, float32_t* beta) {
  float32_t change = 100;
  int step = 0;
  while (change > 0.00004 && step++ < 100) {
    float32_t old_beta[6];
    for (int i = 0; i < 6; i++) {
      *(old_beta + i) = *(beta + i);
    }
    gn_step(data, beta);
    change = 0;
    for (int i = 0; i < 6; i++) {
      change += fabs((*(beta + i) - old_beta[i])/old_beta[i]);
    }
  }
}

void quatProd(float* result, float* qa, float* qb) {
  float temp[4];
  temp[0] = (*(qa+0))*(*(qb+0)) - (*(qa+1))*(*(qb+1)) - (*(qa+2))*(*(qb+2)) - (*(qa+3))*(*(qb+3));
  temp[1] = (*(qa+0))*(*(qb+1)) + (*(qa+1))*(*(qb+0)) + (*(qa+2))*(*(qb+3)) - (*(qa+3))*(*(qb+2));
  temp[2] = (*(qa+0))*(*(qb+2)) - (*(qa+1))*(*(qb+3)) + (*(qa+2))*(*(qb+0)) + (*(qa+3))*(*(qb+1));
  temp[3] = (*(qa+0))*(*(qb+3)) + (*(qa+1))*(*(qb+2)) - (*(qa+2))*(*(qb+1)) + (*(qa+3))*(*(qb+0));
  for (int i = 0; i<4; i++) {
	  *(result+i) = temp[i];
  }
}

void quatConj(float* result, float* q) {
  *(result+0) = *(q+0);
  *(result+1) = -*(q+1);
  *(result+2) = -*(q+2);
  *(result+3) = -*(q+3);
}

float vectMag(float* vec, int len) {
  float sum_sqrs = 0;
  for (int i = 0; i < len; i++) {
	  sum_sqrs += (*(vec+i))*(*(vec+i));
  }
  float mag;
  arm_sqrt_f32(sum_sqrs, &mag);
  return mag;
}

void vectNormalize(float* vec, int len) {
  float mag = vectMag(vec, len);
  for (int i = 0; i < len; i++) {
    *(vec+i) = (*(vec+i))/mag;
  }
}

void vectCross(float* result, float* u, float* v) {
  float temp[3];
  temp[0] = (*(u+1))*(*(v+2)) - (*(u+2))*(*(v+1));
  temp[1] = (*(u+2))*(*(v+0)) - (*(u+0))*(*(v+2));
  temp[2] = (*(u+0))*(*(v+1)) - (*(u+1))*(*(v+0));
  for (int i = 0; i<3; i++) {
  	  *(result+i) = temp[i];
    }
}

float vectDot(float* u, float* v, int len) {
  float result = 0;
  for (int i = 0; i<len; i++) {
    result += (*(u+i))*(*(v+i));
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

void rel_rot(float* q_rel, float* qa, float* qb) {
	float qa_conj[4];
	quatConj(qa_conj, qa);
	quatProd(q_rel, qa_conj, qb);
}

uint8_t check_rot(float* qa, float* qb) {
	uint8_t ret_val = 0;
	float q_rel[4];
	rel_rot(q_rel, qa, qb);
	float ang = 2*acos_nv(q_rel[0]);
	if (ang > 0.3307) ret_val = 1;
	return ret_val;
}

void sin_cos(float32_t theta,float32_t * pSinVal,float32_t * pCosVal) {
	if (fabs(theta) < 0.0000001) {
		*pSinVal = 0;
		*pCosVal = 1;
	}
	else arm_sin_cos_f32(theta,pSinVal,pCosVal);
}


