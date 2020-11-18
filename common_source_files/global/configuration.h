/*
 * configuration.h
 *
 *  Created on: Nov 17, 2020
 *      Author: Fongberg
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

typedef struct _Config_Data_t {
    uint8_t top_face_number;
    uint8_t front_face_number;
} Config_Data_t;

void Configuration_SetBaseQ(uint8_t top_face_number, uint8_t front_face_number, float q_base[4]);

#endif /* CONFIGURATION_H_ */
