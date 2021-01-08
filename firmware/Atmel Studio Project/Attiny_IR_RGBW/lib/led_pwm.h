/*
 * Software PWM implementation to drive RGB or RGBW LEDs by Attiny13A
 *
 * Usage:
 * Define output pins by RED_PIN, GREEN_PIN, BLUE_PIN and optionally WHITE_PIN
 * Call pwm_init_rgb() or pwm_init_rgbw() once after start
 * Poll pwm_rgb(), pwm_rgb_fast(), pwm_rgbw() or pwm_rgbw_fast()
 *
 *  Author: SinuX
 *  License: CC BY-NC-SA
 */ 

#ifndef LED_PWM_H_
#define LED_PWM_H_

#include "common.h"

// Some macros and definitions
#define pwm_init_rgb() DDRB |= (1 << BLUE_PIN) | (1 << RED_PIN) | (1 << GREEN_PIN)
#define pwm_init_rgbw() DDRB |= (1 << BLUE_PIN) | (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << WHITE_PIN)

/**
 * Software PWM for RGB
 */
static inline void pwm_rgb(byte r_value, byte g_value, byte b_value) {
	if (r_value > 0) output_high(RED_PIN);
	if (g_value > 0) output_high(GREEN_PIN);
	if (b_value > 0) output_high(BLUE_PIN);
	byte i;
	for (i = 0; i <= PWM_MAX_VALUE; i++) {
		if (i == r_value) output_low(RED_PIN);
		if (i == g_value) output_low(GREEN_PIN);
		if (i == b_value) output_low(BLUE_PIN);
	}
}

/**
 * Faster RGB version (weighs less, but flickers)
 */
static inline void pwm_rgb_fast(byte r_value, byte g_value, byte b_value) {
	output_low(RED_PIN);
	output_low(GREEN_PIN);
	output_low(BLUE_PIN);
	byte i = PWM_MAX_VALUE;
	do {
		if (i == r_value) output_high(RED_PIN);
		if (i == g_value) output_high(GREEN_PIN);
		if (i == b_value) output_high(BLUE_PIN);
	} while (--i);
}

/**
 * Software PWM for RGBW
 */
#ifdef WHITE_PIN
static inline void pwm_rgbw(byte r_value, byte g_value, byte b_value, byte w_value) {
	if (r_value != PWM_MAX_VALUE) output_low(RED_PIN);
	if (g_value != PWM_MAX_VALUE) output_low(GREEN_PIN);
	if (b_value != PWM_MAX_VALUE) output_low(BLUE_PIN);
	if (w_value != PWM_MAX_VALUE) output_low(WHITE_PIN);
	byte i = PWM_MAX_VALUE;
	do {
		if (i == r_value) output_high(RED_PIN);
		if (i == g_value) output_high(GREEN_PIN);
		if (i == b_value) output_high(BLUE_PIN);
		if (i == w_value) output_high(WHITE_PIN);
	} while (--i);
}

/**
 * Faster RGBW version (weighs less, but flickers)
 */
static inline void pwm_rgbw_fast(byte r_value, byte g_value, byte b_value, byte w_value) {
	output_low(RED_PIN);
	output_low(GREEN_PIN);
	output_low(BLUE_PIN);
	if (w_value != PWM_MAX_VALUE) output_low(WHITE_PIN); // White channel PMW still must be flicker-free
	byte i = PWM_MAX_VALUE;
	do {
		if (i == r_value) output_high(RED_PIN);
		if (i == g_value) output_high(GREEN_PIN);
		if (i == b_value) output_high(BLUE_PIN);
		if (i == w_value) output_high(WHITE_PIN);
	} while (--i);
}
#endif /* WHITE_PIN */

#endif /* LED_PWM_H_ */