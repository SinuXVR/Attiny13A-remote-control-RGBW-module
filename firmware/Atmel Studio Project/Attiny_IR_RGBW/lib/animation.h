/*
 * Common color and animation settings with some visual effect implementations
 *
 * Created: 02.09.2018 23:55:51
 *  Author: SinuX
 */ 

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <math.h>
#include "common.h"

// Main color values
static byte r_value = 0;
static byte g_value = 0;
static byte b_value = 0;
static byte w_value = W_MAX_VALUE;
	
// Current color values
static byte r_current_value = 0;
static byte g_current_value = 0;
static byte b_current_value = 0;
static byte w_current_value = W_MAX_VALUE;

/* ==================================== Control methods ============================================= */

/**
 * Apply current brightness value
 */
extern void anim_set_brightness(byte val) {
	r_current_value = r_value * val >> 8;
	g_current_value = g_value * val >> 8;
	b_current_value = b_value * val >> 8;
	w_current_value = w_value * val >> 8;
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
 * Set color from array by given index
 */
extern void anim_set_color_by_index(byte index) {
	w_value = index == 0 ? W_MAX_VALUE : 0;
	r_value = pgm_read_byte(&colors[index][1]);
	g_value = pgm_read_byte(&colors[index][2]);
	b_value = pgm_read_byte(&colors[index][3]);
}

/**
 * Search color by given key and set it as current
 * Returns 1 if color was changed, otherwise - 0
 */
static inline byte anim_set_color_by_key(byte key) {
	byte i;
	for (i = 0; i < COLORS_COUNT; i++) {
		if (pgm_read_byte(&colors[i][0]) == key) {
			anim_set_color_by_index(i);
			return 1;
		}
	}
	return 0;
}

/**
 * Set next non-white color
 */
extern void anim_set_next_color() {
	static byte color = 1;
	if (++color >= COLORS_COUNT) color = 1; // Skip white color
	anim_set_color_by_index(color);
}

/* ==================================== Animation algorithms ============================================= */

/**
 * Rainbow fade animation
 */
static inline void anim_rainbow() {
	byte finished = 1;
	if (r_value > r_current_value) { r_current_value++; finished = 0; } else
	if (r_value < r_current_value) { r_current_value--; finished = 0; }
	if (g_value > g_current_value) { g_current_value++; finished = 0; } else
	if (g_value < g_current_value) { g_current_value--; finished = 0; }
	if (b_value > b_current_value) { b_current_value++; finished = 0; } else
	if (b_value < b_current_value) { b_current_value--; finished = 0; }
	if (finished) anim_set_next_color();
}

/**
 * All colors jump animation
 */
static inline void anim_jump(byte counter) {
	if (counter == 0) {
		anim_set_next_color();
		anim_set_brightness(BRIGHTNESS_MAX_VALUE);
	}
}

/**
 * Current color fade animation
 */
static inline void anim_fade(byte counter) {
	anim_set_brightness(counter <= BRIGHTNESS_MAX_VALUE / 2 ? counter * 2 : ~counter * 2);
}

/**
 * Current color strobe animation
 */
static inline void anim_strobe(byte counter) {
	anim_set_brightness(counter >> 7 << 7);
}

#endif /* ANIMATION_H_ */