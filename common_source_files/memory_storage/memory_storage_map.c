/*
 * memory_storage_map.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Fongberg
 */

#include "memory_storage_map.h"
#include "global_variables.h"

void MEM_WriteDataVersionCode(uint32_t *data_version_code) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->data_version_code), (uint8_t*) data_version_code, sizeof(uint32_t));
}

void MEM_ReadDataVersionCode(uint32_t *data_version_code) {
  FM_ReadData(&FM25, (uint32_t) &(MEMORY_MAP->data_version_code), (uint8_t*) data_version_code, sizeof(uint32_t));
}


void MEM_WriteCalData(CAL_Data_t *cal_data) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->cal_data), (uint8_t*) cal_data, sizeof(CAL_Data_t));
}

void MEM_ReadCalData(CAL_Data_t *cal_data) {
  FM_ReadData(&FM25, (uint32_t) &(MEMORY_MAP->cal_data), (uint8_t*) cal_data, sizeof(CAL_Data_t));
}

void MEM_WriteGyro0CalData(float gyro0beta[3]) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->cal_data.gyro0beta), (uint8_t*) gyro0beta, 3*sizeof(float));
}

void MEM_WriteAcc0CalData(float acc0beta[6]) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->cal_data.acc0beta), (uint8_t*) acc0beta, 6*sizeof(float));
}

void MEM_WriteMmc0CalData(float mag0beta[6]) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->cal_data.mag0beta), (uint8_t*) mag0beta, 6*sizeof(float));
}


void MEM_WriteConfigData(Config_Data_t *config_data) {
  FM_WriteData(&FM25, (uint32_t) &(MEMORY_MAP->config_data), (uint8_t*) config_data, sizeof(Config_Data_t));
}

void MEM_ReadConfigData(Config_Data_t *config_data) {
  FM_ReadData(&FM25, (uint32_t) &(MEMORY_MAP->config_data), (uint8_t*) config_data, sizeof(Config_Data_t));
}
