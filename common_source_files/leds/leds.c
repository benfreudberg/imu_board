/*
 * timer_pwm.c
 *
 *  Created on: Oct 23, 2020
 *      Author: Fongberg
 */


#include "leds.h"

const led_t led0 = {
  .htim = &htim4,
  .channel = TIM_CHANNEL_1
};
const led_t led1 = {
  .htim = &htim8,
  .channel = TIM_CHANNEL_2
};
const led_t led2 = {
  .htim = &htim8,
  .channel = TIM_CHANNEL_3
};

// todo: there's a delay before it takes effect. Why?
// assumes clock/timer are setup such that the timer frequency is 10kHz
void LED_SetPWM(const led_t *led, uint32_t period_ms, uint8_t duty_cycle) {
  led->htim->Instance->ARR = period_ms*10 - 1;
  volatile uint32_t * crr;
  switch (led->channel) {
    case TIM_CHANNEL_1 :
      crr = &led->htim->Instance->CCR1;
      break;
    case TIM_CHANNEL_2 :
      crr = &led->htim->Instance->CCR2;
      break;
    case TIM_CHANNEL_3 :
      crr = &led->htim->Instance->CCR3;
      break;
    case TIM_CHANNEL_4 :
      crr = &led->htim->Instance->CCR4;
      break;
    case TIM_CHANNEL_5 :
      crr = &led->htim->Instance->CCR5;
      break;
    case TIM_CHANNEL_6 :
      crr = &led->htim->Instance->CCR6;
      break;
  }
  *crr = led->htim->Instance->ARR * duty_cycle / 0xff;
}
