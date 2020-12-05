/*
 * primary_loop.c
 *
 *  Created on: Sep 29, 2020
 *      Author: Fongberg
 */

#include "kalman_filter.h"
//#include "translation.h"
#include "calibration.h"
#include "math_functions.h"
#include "state_interpretation.h"
#include "head_tracker_usb_packet.h"
#include "global_variables.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_customhid.h"
//debug
#include "usart.h"

volatile bool tim1_int = false;
static USB_TrackerPacket_t usb_packet;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim1) {
    tim1_int = true;
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  }
}

static void collect_data_and_run_kf(bool mmc_new_data_b, int time_ms) {
  float IMU0_floats[7], MMC0_floats[3], ACC0_rotated_q[4], GYRO0_rotated_q[4], MAG0_rotated_q[4];
  static int16_t IMU0_raw[7], MMC0_raw[3];

  //read imu
  ICM20602_Read(&ICM0, IMU0_floats, IMU0_raw);
  if (time_ms % 5 == 0) {
    CAL_Imu20BufferPush(IMU0_raw);
  }

  //read mag
  if (mmc_new_data_b) {
    MMC5983MA_Read(&MMC0, MMC0_floats, MMC0_raw);
  } else {
    MMC5983MA_Start(&MMC0);
  }

  float qa0[4];
  float acc0_mag = vectMag(IMU0_floats, 3);

  //kf acc update
  float ACC0_raw_q[4] = {0, IMU0_floats[0], IMU0_floats[1], IMU0_floats[2]};
  vectRot(ACC0_rotated_q, q0_base_rot, ACC0_raw_q);
  bool acc0_good_b = false;
  if (acc0_mag > 0.98 && acc0_mag < 1.02) {
    acc0_good_b = true;
    KalmanFilter_AccStep(qa0, q0, ACC0_rotated_q + 1);
  }

  //kf mag update
  if (mmc_new_data_b && acc0_good_b)  {
    float mag0_mag = vectMag(MMC0_floats, 3);
    if (mag0_mag > 0.95 && mag0_mag < 1.05) {
      float MAG0_raw_q[4] = {0, MMC0_floats[0], MMC0_floats[1], MMC0_floats[2]};
      vectRot(MAG0_rotated_q, q0_base_rot, MAG0_raw_q);
      KalmanFilter_MagStep(qa0, MAG0_rotated_q + 1);
    }
  }

  float GYRO0_raw_q[4] = {0, IMU0_floats[4], IMU0_floats[5], IMU0_floats[6]};
  vectRot(GYRO0_rotated_q, q0_base_rot, GYRO0_raw_q);

  //kf gyro update
  if (acc0_good_b) {
    KalmanFilter_Step(&kf0, q0, qa0, (GYRO0_rotated_q + 1));
  } else {
    KalmanFilter_Step(&kf0, q0, q0, (GYRO0_rotated_q + 1));
  }

  if (mmc_new_data_b && collect_mag_data_b) {
    CAL_Mag20BufferPush(MMC0_raw);
  }

  //translation update
//  float gravity_g_q[4] = {0, 0, 0, 1};
//  float q_state[4];
//  quatProd(q_state, q0_yawzero, q0);
//  vectRot(ACC0_rotated_q, q_state, ACC0_rotated_q);
//  for (int i=0; i<3; i++) {
//    ACC0_rotated_q[i+1] -= gravity_g_q[i+1];
//  }
//  static bool do_translation_update = false;
//  if (!do_translation_update && time_ms > 990) {
//    do_translation_update = true;
//  }
//  if (do_translation_update) {
//    TRANS_update(translation_state, ACC0_rotated_q + 1, dt, q0);
////    float q_state_inv[4], q0_base_rot_inv[4];
////    quatConj(q_state_inv, q0);
////    quatConj(q0_base_rot_inv, q0_base_rot);
////    vectRot(ACC0_rotated_q, q_state_inv, ACC0_rotated_q);
////    vectRot(ACC0_rotated_q, q0_base_rot_inv, ACC0_rotated_q);
////    for (int i=0; i<3; i++) {
////      ICM0.accbeta[i] += ACC0_rotated_q[i + 1]/2000 / ICM0.accbeta[i + 3];
////    }
//  }

  //debug data
#define DEBUG_BUFFER_LENGTH   100
  static int16_t buffer[2][DEBUG_BUFFER_LENGTH][9];
  static bool buffer_bool = 0;

  for (int i = 0; i < 3; i++) {
    buffer[buffer_bool][time_ms % DEBUG_BUFFER_LENGTH][i + 0] = IMU0_raw[i];
    buffer[buffer_bool][time_ms % DEBUG_BUFFER_LENGTH][i + 3] = IMU0_raw[i + 4];
    buffer[buffer_bool][time_ms % DEBUG_BUFFER_LENGTH][i + 6] = MMC0_raw[i];
  }
  if (time_ms % DEBUG_BUFFER_LENGTH == DEBUG_BUFFER_LENGTH - 5) {
    static uint64_t sync_marker = 0x0123456789abcdef;
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)(&sync_marker), sizeof(sync_marker));
  }
  if (time_ms % DEBUG_BUFFER_LENGTH == DEBUG_BUFFER_LENGTH - 1) {
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)(&(buffer[buffer_bool][0][0])), sizeof(buffer)/2);
    buffer_bool = !buffer_bool;
  }
  //end debug data
}


void primary_loop(void) {
  static bool mmc_new_data_b = true;
  static int time_ms = 0;

  if (tim1_int) {
    tim1_int = false;
    mmc_new_data_b = !mmc_new_data_b;
//    mmc_new_data_b = false;

    collect_data_and_run_kf(mmc_new_data_b, time_ms);

    int16_t ypr[3];
    STATE_QtoYPR(q0, ypr);
    usb_packet.yaw = (int16_t)ypr[0];
    usb_packet.pitch = (int16_t)ypr[1];
    usb_packet.roll = (int16_t)ypr[2];
//    usb_packet.x =  (int16_t)(translation_state[1][0] * (1<<15));
//    usb_packet.y =  (int16_t)(translation_state[1][1] * (1<<15));
//    usb_packet.z =  (int16_t)(translation_state[1][2] * (1<<15));
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&usb_packet, sizeof(USB_TrackerPacket_t));


    time_ms++;
    time_ms %= 1000;
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  }
}
