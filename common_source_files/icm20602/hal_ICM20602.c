/*
 * hal_ICM20602.c
 *
 *  Created on: April 16, 2018
 *      Author: Ben Freudberg
 */

#include "hal_ICM20602.h"
#include "spi_wrapper.h"
#include "various_functions.h"

const int GYROSENS[] = {250, 500, 1000, 2000};

static void imu_reading_to_data(int16_t data_out[7], uint8_t reading[14]) {
  for (int i = 0; i < 7; i++) {
    data_out[i] = (int16_t) (reading[2*i] << 8 | reading[2*i + 1]);
  }
}

// todo: don't normalize
static void imu_int_to_norm_float(ICM20602* ICM, int16_t input[7], float output[7]) {
  float* beta_acc = ICM->accbeta;
  float* beta_gyro = ICM->gyrobeta;
  for (int i = 0; i < 3; i++) {
    // add offset, multiply by scale -> output in g's
    output[i] = ((float) input[i] - beta_acc[i]) * beta_acc[i + 3];
  }
  vectNormalize(output, 3);

  output[3] = input[3] / 326.8 + 25; //degrees C

  for (int i = 4; i < 7; i++) {
    // scale to rad/sec (from 250/500/1000/2000 deg/s) and then add offset
    output[i] = ((float) input[i]) / 32768 * GYROSENS[ICM->GyroSens] / 180 * PI  - beta_gyro[i - 4];
  }
}



HAL_StatusTypeDef ICM20602_Init(ICM20602* ICM, float abeta[6], float gbeta[3]) {
  SPI_HandleTypeDef* SPI_Bus = ICM->SPI_Bus;
  GPIO_TypeDef* CS_Port = ICM->CS_Port;
  uint16_t CS_Pin = ICM->CS_Pin;
  AccelerometerSensitivity AccSens = ICM->AccSens;
  GyroscopeSensitivity GyroSens = ICM->GyroSens;
  GyroscopeFilter GyroFilt = ICM->GyroFilt;
  
  for (int i = 0; i<6; i++) {
	  ICM->accbeta[i] = abeta[i];
  }

  for (int i = 0; i<3; i++) {
	  ICM->gyrobeta[i] = gbeta[i];
  }

  uint8_t aTxBuffer[2] = {0};
  uint8_t aRxBuffer[2] = {0};
  
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 1, 5);
  
  
  aTxBuffer[0] = WhoAmI_Reg | SPIReadMask; //check WhoAmI
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  if (aRxBuffer[1] != WhoAmI_Value) {
    return HAL_ERROR;
  }
  
  
  aTxBuffer[0] = I2CEnable_Reg;
  aTxBuffer[1] = I2C_Setting; //disable I2C
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  aTxBuffer[0] = PwrMgmt1_Reg;
  aTxBuffer[1] = PwrMgmt1_Setting; //wake up device
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  aTxBuffer[0] = PwrMgmt2_Reg;
  aTxBuffer[1] = PwrMgmt2_Setting; //enable everything
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  aTxBuffer[0] = Config_Reg;
  aTxBuffer[1] = GyroFilt; //dlpf (for gyro)
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);

  aTxBuffer[0] = AccConfig2_Reg;
  aTxBuffer[1] = Acc_Config2_Setting; //dlpf (for acc)
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  aTxBuffer[0] = GyroConfig_Reg | SPIReadMask;
  aTxBuffer[1] = 0;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  aTxBuffer[0] = GyroConfig_Reg;
  aTxBuffer[1] = (aRxBuffer[1] & 0xE7) | (uint8_t)GyroSens << 3; //set Gyro FSR to GyroSens
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  aTxBuffer[0] = AccConfig_Reg | SPIReadMask;
  aTxBuffer[1] = 0;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  aTxBuffer[0] = AccConfig_Reg;
  aTxBuffer[1] = (aRxBuffer[1] & 0xE7) | (uint8_t)AccSens << 3; //set Acc FSR to AccSens
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  return HAL_OK;
}


void ICM20602_Read(ICM20602* ICM, float IMU_floats[7]) {
  uint8_t aTxBuffer[15] = {0};
  uint8_t aRxBuffer[15] = {0};
  int16_t IMU_data[7];

  aTxBuffer[0] = FirstData_Reg | SPIReadMask;
  HAL_GPIO_WritePin(ICM->CS_Port, ICM->CS_Pin, GPIO_PIN_RESET);
  SPI_TransmitReceive_DMA_Blocking(ICM->SPI_Bus, aTxBuffer, aRxBuffer, 15);

  imu_reading_to_data(IMU_data, aRxBuffer + 1);
  imu_int_to_norm_float(ICM, IMU_data, IMU_floats);
}
