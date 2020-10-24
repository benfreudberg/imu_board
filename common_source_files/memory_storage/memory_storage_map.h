/*
 * memory_storage_map.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Fongberg
 */

#ifndef MEMORY_STORAGE_MAP_H_
#define MEMORY_STORAGE_MAP_H_

#include "calibration.h"

#define MEMORY_START_ADDR     0x0
#define MEMORY_MAP            ((MEM_Storage_Map_t*)MEMORY_START_ADDR)

typedef struct _MEM_Storage_Map_t {
  CAL_Data_t cal_data;
} MEM_Storage_Map_t;

void MEM_WriteCalData(CAL_Data_t *cal_data);
void MEM_ReadCalData(CAL_Data_t *cal_data);
void MEM_WriteGyro0CalData(float gyro0beta[3]);
void MEM_WriteAcc0CalData(float acc0beta[6]);
void MEM_WriteMmc0CalData(float mag0beta[6]);

#endif /* MEMORY_STORAGE_MAP_H_ */
