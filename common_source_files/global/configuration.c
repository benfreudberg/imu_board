/*
 * configuration.c
 *
 *  Created on: Nov 17, 2020
 *      Author: Fongberg
 */

#include <math.h>
#include "math_functions.h"

static const float rotation_list_q[10][4] = {
    {1, 0, 0, 0},                   //     0
    {sqrt(2)/2, -sqrt(2)/2, 0, 0},  // -x 90
    {sqrt(2)/2, sqrt(2)/2, 0, 0},   // +x 90
    {sqrt(2)/2, 0, -sqrt(2)/2, 0},  // -y 90
    {sqrt(2)/2, 0, sqrt(2)/2, 0},   // +y 90
    {sqrt(2)/2, 0, 0, -sqrt(2)/2},  // -z 90
    {sqrt(2)/2, 0, 0, sqrt(2)/2},   // +z 90
    {0, 1, 0, 0},                   // +x 180
    {0, 0, 1, 0},                   // +y 180
    {0, 0, 0, 1},                   // +z 180
};

enum {
  Q_ZERO = 0,
  Q_MINUS_X,
  Q_PLUS_X,
  Q_MINUS_Y,
  Q_PLUS_Y,
  Q_MINUS_Z,
  Q_PLUS_Z,
  Q_180_X,
  Q_180_Y,
  Q_180_Z,
};

// rotation for side that is up (sides marked like a game die - 1 is top, 2 is forwards (opposite usb port))
static const float* const rotation_table0_qp[6] = {
    &rotation_list_q[Q_ZERO][0],    // side 1
    &rotation_list_q[Q_PLUS_X][0],  // side 2
    &rotation_list_q[Q_PLUS_Y][0],  // side 3
    &rotation_list_q[Q_MINUS_Y][0], // side 4
    &rotation_list_q[Q_MINUS_X][0], // side 5
    &rotation_list_q[Q_180_Y][0],   // side 6
};

// rotation for side that is forwards
static const float* const rotation_table1_qp[6][6] = {
    // side 1 up
    {&rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_ZERO][0],
     &rotation_list_q[Q_MINUS_Z][0],
     &rotation_list_q[Q_PLUS_Z][0],
     &rotation_list_q[Q_180_Z][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
    },
    // side 2 up
    {&rotation_list_q[Q_180_Y][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_MINUS_Y][0],
     &rotation_list_q[Q_PLUS_Y][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_ZERO][0],
    },
    // side 3 up
    {&rotation_list_q[Q_MINUS_X][0],
     &rotation_list_q[Q_ZERO][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_180_X][0],
     &rotation_list_q[Q_PLUS_X][0],
    },
    // side 4 up
    {&rotation_list_q[Q_MINUS_X][0],
     &rotation_list_q[Q_ZERO][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_180_X][0],
     &rotation_list_q[Q_PLUS_X][0],
    },
    // side 5 up
    {&rotation_list_q[Q_ZERO][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_PLUS_Y][0],
     &rotation_list_q[Q_MINUS_Y][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_180_Y][0],
    },
    // side 6 up
    {&rotation_list_q[Q_ZERO][0],   //invalid
     &rotation_list_q[Q_ZERO][0],
     &rotation_list_q[Q_MINUS_Z][0],
     &rotation_list_q[Q_PLUS_Z][0],
     &rotation_list_q[Q_180_Z][0],
     &rotation_list_q[Q_ZERO][0],   //invalid
    },
};

void Configuration_SetBaseQ(uint8_t top_face_number, uint8_t front_face_number, float q_base[4]) {
  const float* q1 = rotation_table0_qp[top_face_number - 1];
  const float* q2 = rotation_table1_qp[top_face_number - 1][front_face_number - 1];
  float q_temp[4];
  const float* const q0 = &rotation_list_q[0][0];
  quatProd(q_temp, q1, q0);
  quatProd(q_base, q2, q_temp);
}
