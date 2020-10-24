/*
 * timer_pwm.h
 *
 *  Created on: Oct 23, 2020
 *      Author: Fongberg
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "tim.h"

typedef struct _led_t {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
} led_t;

extern const led_t led0;
extern const led_t led1;
extern const led_t led2;

void LED_SetPWM(const led_t *led, uint32_t period_ms, uint8_t duty_cycle);

#endif /* LEDS_H_ */
