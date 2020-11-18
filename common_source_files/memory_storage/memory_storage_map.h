/*
 * memory_storage_map.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Fongberg
 */

#ifndef MEMORY_STORAGE_MAP_H_
#define MEMORY_STORAGE_MAP_H_

#include "calibration.h"
#include "configuration.h"

#define MEMORY_START_ADDR     0x0
#define MEMORY_MAP            ((MEM_Storage_Map_t*)MEMORY_START_ADDR)
#define DATA_VERSION_CODE     (0x7ac69b55) //to be changed if the data structure changes

typedef struct _MEM_Storage_Map_t {
  uint32_t data_version_code;
  CAL_Data_t cal_data;
  Config_Data_t config_data;
} MEM_Storage_Map_t;

void MEM_WriteDataVersionCode(uint32_t *data_version_code);
void MEM_ReadDataVersionCode(uint32_t *data_version_code);

void MEM_WriteCalData(CAL_Data_t *cal_data);
void MEM_ReadCalData(CAL_Data_t *cal_data);
void MEM_WriteGyro0CalData(float gyro0beta[3]);
void MEM_WriteAcc0CalData(float acc0beta[6]);
void MEM_WriteMmc0CalData(float mag0beta[6]);

void MEM_WriteConfigData(Config_Data_t *config_data);
void MEM_ReadConfigData(Config_Data_t *config_data);

#endif /* MEMORY_STORAGE_MAP_H_ */
