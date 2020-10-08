/*
 * hal_ICM20602.h
 *
 *  Created on: April 16, 2018
 *      Author: Ben Freudberg
 */
#ifndef hal_ICM20602_h
#define hal_ICM20602_h

#include "spi_control.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_spi.h"

//device registers
#define Config_Reg      26
#define GyroConfig_Reg  27
#define AccConfig_Reg   28
#define AccConfig2_Reg  29
#define FirstData_Reg   59
#define PwrMgmt1_Reg    107
#define PwrMgmt2_Reg    108
#define I2CEnable_Reg   112
#define WhoAmI_Reg      117


//device parameters
//#define Config_Setting          0 //more research required
#define PwrMgmt1_Setting        1
#define Acc_Config2_Setting     2 //more research required
#define PwrMgmt2_Setting        0
#define I2C_Setting             64
#define WhoAmI_Value            18


typedef enum  {
  Accelerometer_2G  = 0x00, /*!< Range is +- 2G */
  Accelerometer_4G  = 0x01, /*!< Range is +- 4G */
  Accelerometer_8G  = 0x02, /*!< Range is +- 8G */
  Accelerometer_16G = 0x03 /*!< Range is +- 16G */
} AccelerometerSensitivity;

typedef enum {
  Gyroscope_250s  = 0x00,  /*!< Range is +- 250 degrees/s */
  Gyroscope_500s  = 0x01,  /*!< Range is +- 500 degrees/s */
  Gyroscope_1000s = 0x02, /*!< Range is +- 1000 degrees/s */
  Gyroscope_2000s = 0x03  /*!< Range is +- 2000 degrees/s */
} GyroscopeSensitivity;

typedef enum {
  Gyroscope_250Hz  = 0x00,
  Gyroscope_176Hz  = 0x01,
  Gyroscope_92Hz   = 0x02,
  Gyroscope_41Hz   = 0x03,
	Gyroscope_20Hz   = 0x04,
	Gyroscope_10Hz   = 0x05,
	Gyroscope_5Hz    = 0x06,
	Gyroscope_3281Hz = 0x07,
} GyroscopeFilter;

typedef struct  {
  SPI_HandleTypeDef* SPI_Bus;
  GPIO_TypeDef* CS_Port;
  uint16_t CS_Pin;
  AccelerometerSensitivity AccSens;
  GyroscopeSensitivity GyroSens;
  GyroscopeFilter GyroFilt;
  float accbeta[6];
  float gyrobeta[3];
} ICM20602;

//typedef struct  {
//    uint16_t x_acc;
//    uint16_t y_acc;
//    uint16_t z_acc;
//    uint16_t temp;
//    uint16_t x_gy;
//    uint16_t y_gy;
//    uint16_t z_gy;
//} IMU_data;

//typedef enum  {
//    ICM20602_Result_Ok = 0x00,          /*!< Everything OK */
//    ICM20602_Result_Error,              /*!< Unknown error */
//} ICM20602_Result;

HAL_StatusTypeDef ICM20602_Init(ICM20602* ICM, float abeta[6], float gbeta[3]);
void ICM20602_Read(ICM20602* ICM, float IMU_floats[7]);

#endif
