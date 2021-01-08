/*
 * Common definitions
 *
 *  Author: SinuX
 *  License: CC BY-NC-SA
 */ 

#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include <avr/sleep.h>

#define byte uint8_t
#define word uint16_t

#define output_low(pin) PORTB &= ~(1 << pin)
#define output_high(pin) PORTB |= (1 << pin)

#define set_power_off_bit(state) state |= 0x80
#define toggle_power_off_bit(state) state ^= 0x80
#define clear_power_off_bit(state) state &= 0x7F
#define has_power_off_bit(state) (state & 0x80)
#define power_off_init() do { set_sleep_mode(SLEEP_MODE_PWR_DOWN); sleep_enable(); } while (0)
#define power_off() do { PORTB = 0; sleep_cpu(); } while (0)

/* ============================== Modes =============================== */

#define STATE_COLOR				0
#define STATE_RAINBOW_ANIMATION	1
#define STATE_JUMP_ANIMATION	2
#define STATE_FADE_ANIMATION	3
#define STATE_STROBE_ANIMATION	4

/* ========================== PWM settings ============================ */

#define PWM_MAX_VALUE	86	// Max value for PWM (<= 255, lower value - bigger switching frequency)

/* =========================== IR settings ============================ */

// NEC command triggering settings (use "Timer setup.xlsx" to calculate)
#define IR_START_SEQ_TICKS  64
#define IR_HIGH_BIT_TICKS   32
#define IR_VALIDATE_ADDRESS	// Comment out to disable

// IR key codes
#define IR_KEY_POWER_ON			0xE0
#define IR_KEY_POWER_OFF		0x60
// --
#define IR_KEY_COLOR_WHITE		0xD0
// --
#define IR_KEY_COLOR_RED		0x90
#define IR_KEY_COLOR_RED_1		0xB0
#define IR_KEY_COLOR_RED_2		0xA8
#define IR_KEY_COLOR_RED_3		0x98
#define IR_KEY_COLOR_RED_4		0x88
// --
#define IR_KEY_COLOR_GREEN		0x10
#define IR_KEY_COLOR_GREEN_1	0x30
#define IR_KEY_COLOR_GREEN_2	0x28
#define IR_KEY_COLOR_GREEN_3	0x18
#define IR_KEY_COLOR_GREEN_4	0x08
// --
#define IR_KEY_COLOR_BLUE		0x50
#define IR_KEY_COLOR_BLUE_1		0x70
#define IR_KEY_COLOR_BLUE_2		0x68
#define IR_KEY_COLOR_BLUE_3		0x58
#define IR_KEY_COLOR_BLUE_4		0x48
// --
#define IR_KEY_ANIM_RAINBOW		0xC8
#define IR_KEY_ANIM_JUMP		0xF0
#define IR_KEY_ANIM_FADE		0xD8
#define IR_KEY_ANIM_STROBE		0xE8
// --
#define IR_KEY_UP				0xA0
#define IR_KEY_DOWN				0x20

/* ================== Color and animation settings ==================== */

// Animation rate settings
#define ANIM_RATE_INITIAL_VALUE	32
#define ANIM_RATE_MAX_VALUE		255
#define ANIM_RATE_MIN_VALUE		1
#define JUMP_ANIMATION_DIVIDER	5

// Brightness settings
#define BRIGHTNESS_INITIAL_VALUE	128
#define BRIGHTNESS_MAX_VALUE		128 // Don't change!
#define BRIGHTNESS_MIN_VALUE		16

// Initial state
#define INITIAL_STATE	STATE_COLOR
//#define INITIAL_STATE	set_power_off_bit(STATE_COLOR) // Use this to sleep after power on

// Color IR key codes with color spectrum coords
#define COLORS_COUNT 15						// IR key code			  | Spectrum position
static const byte colors[COLORS_COUNT][2] = {{ IR_KEY_COLOR_RED,		0 },
											 { IR_KEY_COLOR_RED_1,		2 },
											 { IR_KEY_COLOR_RED_2,		5 },
											 { IR_KEY_COLOR_RED_3,		10 },
											 { IR_KEY_COLOR_RED_4,		20 },
														 
											 { IR_KEY_COLOR_GREEN,		85 },
											 { IR_KEY_COLOR_GREEN_1,	95 },
											 { IR_KEY_COLOR_GREEN_2,	105 },
											 { IR_KEY_COLOR_GREEN_3,	115 },
											 { IR_KEY_COLOR_GREEN_4,	128 },
														 
											 { IR_KEY_COLOR_BLUE,		170 },
											 { IR_KEY_COLOR_BLUE_1,		185 },
											 { IR_KEY_COLOR_BLUE_2,		200 },
											 { IR_KEY_COLOR_BLUE_3,		215 },
											 { IR_KEY_COLOR_BLUE_4,		230 }};

/* ======================== I/O settings ========================== */

/************************************************************************/
/* WARNING: if you change RGB or RGBW pin orders, you must modify       */
/* pwm_init_rgb() or pwm_init_rgbw() macros in led_pwm.h too            */
/************************************************************************/

#define RED_PIN			PB1	// Output pin for RED channel
#define GREEN_PIN		PB2	// Output pin for GREEN channel
#define BLUE_PIN		PB0	// Output pin for BLUE channel
#define WHITE_PIN		PB3	// Output pin for WHITE channel
#define IR_SENSOR_PIN	PB4	// Input pin for IR Sensor

#endif /* COMMON_H_ */