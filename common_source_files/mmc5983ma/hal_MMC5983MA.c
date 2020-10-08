/*
 *  hal_MMC5983MA.c
 *
 *  Created on: April 25, 2018
 *      Author: Ben Freudberg
 */
 
#include "hal_MMC5983MA.h"
#include "spi_wrapper.h"
#include "various_functions.h"


static void mag_reading_to_data(int16_t data_out[3], uint8_t reading[6]) {
  for (int i = 0; i < 3; i++) {
    uint16_t temporary_variable = (uint16_t) (reading[2*i]) << 8 | (reading[2*i + 1]);
    data_out[i] = (int16_t) (temporary_variable - 32768);
  }
}

static void mag_int_to_norm_float(MMC5983MA* MMC, int16_t input[3], float output[3]) {
  float* beta = MMC->beta;
  float temp[3];
  for (int i = 0; i < 3; i++) {
    // add offset, multiply by scale -> output normalized
    temp[i] = (((float) (input[i])) - (beta[i])) * (beta[i + 3]);
  }
  // fix axes to match imu
  output[0] =  temp[1];
  output[1] = -temp[0];
  output[2] = -temp[2];
  vectNormalize(output, 3);
}


HAL_StatusTypeDef MMC5983MA_Init(MMC5983MA* MMC, float beta[6]) {
  SPI_HandleTypeDef* SPI_Bus = MMC->SPI_Bus;
  GPIO_TypeDef* CS_Port = MMC->CS_Port;
  uint16_t CS_Pin = MMC->CS_Pin;
  MMC_read_speed ODR_setting = MMC->ODR;
  
  for (int i = 0; i<6; i++) {
	  MMC->beta[i] = beta[i];
  }

  uint8_t aTxBuffer[2] = {0};
  uint8_t aRxBuffer[2] = {0};
  
  //check prod id value
  aTxBuffer[0] = ProdID_Reg | SPIReadMask;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  if (aRxBuffer[1] != ProdID_Value) {
    return HAL_ERROR;
  }
  
  //set read speed
  aTxBuffer[0] = Config_Reg1;
  aTxBuffer[1] = ODR_setting;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  //send "reset" command
  aTxBuffer[0] = Config_Reg0;
  aTxBuffer[1] = Config0_Reset;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  HAL_Delay(100);
  
  //send "set" command
  aTxBuffer[0] = Config_Reg0;
  aTxBuffer[1] = Config0_Set;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  HAL_Delay(5);
  
  //send start command
  aTxBuffer[0] = Config_Reg0;
  aTxBuffer[1] = Config0_Start;
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(SPI_Bus, (uint8_t*) &aTxBuffer, (uint8_t*) &aRxBuffer, 2, 5);
  HAL_GPIO_WritePin(CS_Port, CS_Pin, GPIO_PIN_SET);
  
  HAL_Delay(2);
  
  return HAL_OK;
}

void MMC5983MA_Start(MMC5983MA * MMC) {
  //must be static so that this memory remains allocated after the function returns - otherwise NonBlocking transmit might not work
  static uint8_t aTxBuffer[2] = {0};
  static uint8_t aRxBuffer[2] = {0};

  aTxBuffer[0] = Config_Reg0;
  aTxBuffer[1] = Config0_Start;
  HAL_GPIO_WritePin(MMC->CS_Port, MMC->CS_Pin, GPIO_PIN_RESET);
  SPI_TransmitReceive_DMA_NonBlocking(MMC->SPI_Bus, aTxBuffer, aRxBuffer, 2);
}

void MMC5983MA_Read(MMC5983MA * MMC, float MMC_floats[3]) {
  uint8_t aTxBuffer[7] = {0};
  uint8_t aRxBuffer[7] = {0};
  int16_t MMC_data[3];

  aTxBuffer[0] = MMC_Data_Reg | SPIReadMask;
  HAL_GPIO_WritePin(MMC->CS_Port, MMC->CS_Pin, GPIO_PIN_RESET);
  SPI_TransmitReceive_DMA_Blocking(MMC->SPI_Bus, aTxBuffer, aRxBuffer, 7);

  mag_reading_to_data(MMC_data, aRxBuffer + 1);
  mag_int_to_norm_float(MMC, MMC_data, MMC_floats);
}
