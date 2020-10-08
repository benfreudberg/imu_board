/*
 *  hal_MMC5883MA.h
 *
 *  Created on: April 25, 2018
 *      Author: Ben Freudberg
 */

#ifndef hal_MMC5883MA_h
#define hal_MMC5883MA_h

#include "spi_control.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_spi.h"

//device registers
#define Config_Reg0     0x09
#define Config_Reg1     0x0A
#define Config_Reg2     0x0B
#define Config_Reg3     0x0C
#define ProdID_Reg      0x2F
#define MMC_Data_Reg    0x00

//device parameters
#define Config0_Set     0x08
#define Config0_Reset   0x10
#define Config0_Start   0x01
#define ProdID_Value    0x30


typedef enum {
  ODR100 = 0x00, //8ms, 100Hz
  ODR200 = 0x01, //4ms, 200Hz
  ODR400 = 0x02, //2ms, 400Hz
  ODR800 = 0x03  //0.5ms, 800Hz
} MMC_read_speed;

typedef struct  {
  SPI_HandleTypeDef* SPI_Bus;
  GPIO_TypeDef* CS_Port;
  uint16_t CS_Pin;
  MMC_read_speed ODR;
  float beta[6];
} MMC5983MA;

HAL_StatusTypeDef MMC5983MA_Init(MMC5983MA* MMC, float beta[6]);
//void mag_reading_to_data(int16_t* data_out, uint8_t* reading);
void MMC5983MA_Start(MMC5983MA * MMC);
void MMC5983MA_Read(MMC5983MA * MMC, float MMC_floats[3]);

#endif
