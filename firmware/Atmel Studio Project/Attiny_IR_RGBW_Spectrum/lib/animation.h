/*
 * Common color and animation settings with some visual effect implementations
 *
 *  Author: SinuX
 *  License: CC BY-NC-SA
 */ 

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <math.h>
#include "common.h"

#define BRIGHTNESS_DIVIDER	(byte) ceil(log(BRIGHTNESS_MAX_VALUE) / log(2))

// Main color values
static byte r_value = 0;
static byte g_value = 0;
static byte b_value = 0;
static byte w_value = PWM_MAX_VALUE;
	
// Current color values
static byte r_current_value = 0;
static byte g_current_value = 0;
static byte b_current_value = 0;
static byte w_current_value = PWM_MAX_VALUE;

/* ==================================== Control methods ============================================= */

/**
 * Apply current brightness value
 */
extern void anim_set_brightness(byte val) {
	r_current_value = (r_value * val) >> BRIGHTNESS_DIVIDER;
	g_current_value = (g_value * val) >> BRIGHTNESS_DIVIDER;
	b_current_value = (b_value * val) >> BRIGHTNESS_DIVIDER;
	w_current_value = (w_value * val) >> BRIGHTNESS_DIVIDER;
}

/**
 * Increase and apply brightness value
 */
static inline byte anim_inc_brightness(byte brightness) {
	if (brightness <= BRIGHTNESS_MAX_VALUE / 2) brightness <<= 1;
	else brightness = BRIGHTNESS_MAX_VALUE;
	anim_set_brightness(brightness);
	return brightness;
}

/**
 * Decrease and apply brightness value
 */
static inline byte anim_dec_brightness(byte brightness) {
	if (brightness >= BRIGHTNESS_MIN_VALUE * 2) brightness >>= 1;
	else brightness = BRIGHTNESS_MIN_VALUE;
	anim_set_brightness(brightness);
	return brightness;
}

/**
 * Increase animation rate (decrease speed)
 */
static inline byte anim_inc_rate(byte anim_rate) {
	if (anim_rate <= ANIM_RATE_MAX_VALUE / 2) return anim_rate << 1;
	return ANIM_RATE_MAX_VALUE;
}

/**
 * Decrease animation rate (increase speed)
 */
static inline byte anim_dec_rate(byte anim_rate) {
	if (anim_rate >= ANIM_RATE_MIN_VALUE * 2) return anim_rate >> 1;
	return ANIM_RATE_MIN_VALUE;
}

/**
 * Interpolation between two values to create RGB spectrum
 */
extern void anim_interpolate_triangular(byte val, byte *c1, byte *c2) {
	if (val < 43) {
		*c1 = PWM_MAX_VALUE;
		*c2 = (PWM_MAX_VALUE / 43) * val;
	} else {
		*c1 = (PWM_MAX_VALUE / 43) * (85 - val);
		*c2 = PWM_MAX_VALUE;
	}
}

/**
 * Set color from spectrum by given coords
 */
extern void anim_set_color_from_spectrum(byte coords) {
	r_value = 0;
	g_value = 0;
	b_value = 0;
	w_value = 0;
	
	if (coords <= 85)
		anim_interpolate_triangular(coords, &r_value, &g_value);
	else
		if (coords <= 170)
			anim_interpolate_triangular(coords - 85, &g_value, &b_value);
		else
			anim_interpolate_triangular(coords - 170, &b_value, &r_value);
}

/**
 * Search color by given key and set it as current
 * Returns 1 if color was changed, otherwise - 0
 */
static inline byte anim_set_color_by_key(byte key) {
	// Try to set white
	if (key == IR_KEY_COLOR_WHITE) {
		r_value = 0;
		g_value = 0;
		b_value = 0;
		w_value = PWM_MAX_VALUE;
		return 1;
	}
	
	// Try to set RGB
	byte i = COLORS_COUNT - 1;
	do {
		if (colors[i][0] == key) {
			anim_set_color_from_spectrum(colors[i][1]);
			return 1;
		}
	} while(i--);
	
	return 0;
}

/* ==================================== Animation algorithms ============================================= */

/**
 * Rainbow fade animation
 */
static inline void anim_rainbow(byte counter) {
	anim_set_color_from_spectrum(counter);
	anim_set_brightness(BRIGHTNESS_MAX_VALUE);
}

/**
 * All colors jump animation
 */
static inline void anim_jump(byte counter) {
	anim_set_color_from_spectrum(counter >> JUMP_ANIMATION_DIVIDER << JUMP_ANIMATION_DIVIDER);
	anim_set_brightness(BRIGHTNESS_MAX_VALUE);
}

/**
 * Current color fade animation
 */
static inline void anim_fade(byte counter) {
	anim_set_brightness(counter <= BRIGHTNESS_MAX_VALUE ? counter : ~counter);
}

/**
 * Current color strobe animation
 */
static inline void anim_strobe(byte counter) {
	anim_set_brightness(counter >> BRIGHTNESS_DIVIDER << BRIGHTNESS_DIVIDER);
}

#endif /* ANIMATION_H_ */