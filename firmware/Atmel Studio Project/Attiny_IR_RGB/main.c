/**
 * Chip type: Attiny13
 * Clock frequency: 4.8MHz without divider
 * Flash command: avrdude -p t13 -c avrisp -b 19200 -u -Uflash:w:Attiny_IR_RGB.hex:a -Ulfuse:w:0x71:m -Uhfuse:w:0xFF:m
 *                       +--------+
 *        [        (PB5) |1*     8| (VCC)  Power ]
 *        [        (PB3) |2      7| (PB2)   BLUE ]
 *        [ VS1838 (PB4) |3      6| (PB1)  GREEN ]
 *        [ Ground (GND) |4      5| (PB0)    RED ]
 *                       +--------+
 */

#define F_CPU 4800000UL
#include <avr/io.h>
#include "lib/common.h"
#include "lib/ir.h"
#include "lib/led_pwm.h"
#include "lib/animation.h"

// Entry point
int main(void) {
	// Set up
	power_off_init();
	pwm_init_rgb();
	ir_init();
	
	// Initial state - STATE_COLOR with power off bit
	byte current_state = STATE_COLOR | 0x80;
	
	// Animation and brightness parameters
	byte anim_rate = ANIM_RATE_INITIAL_VALUE;
	byte anim_counter = 0;
	byte anim_local_counter = 0;
	byte brightness = BRIGHTNESS_INITIAL_VALUE;
	
	// Main loop
	while(1) {
		// Check if new command was received and change state
		if (ir_has_new_command()) {
			byte command = ir_get_received_command();

			// Power on and power off
			if (command == IR_KEY_POWER_ON) {
				clear_power_off_bit(current_state);
				continue;
			}
			if (command == IR_KEY_POWER_OFF) {
				set_power_off_bit(current_state);
				continue;
			}
			
			// Ignore other commands during sleep
			if (has_power_off_bit(current_state)) continue;
			
			// Change brightness or animation speed depending on current mode
			if (command == IR_KEY_UP) {
				if (current_state == STATE_COLOR) {
					brightness = anim_inc_brightness(brightness);
				}
				else {
					anim_rate = anim_dec_rate(anim_rate);
				}
				continue;
			}
			if (command == IR_KEY_DOWN) {
				if (current_state == STATE_COLOR) {
					brightness = anim_dec_brightness(brightness);
				}
				else {
					anim_rate = anim_inc_rate(anim_rate);
				}
				continue;
			}
			
			// Change animation modes
			if (command == IR_KEY_ANIM_RAINBOW) {
				current_state = STATE_RAINBOW_ANIMATION;
				continue;
			}
			if (command == IR_KEY_ANIM_JUMP) {
				current_state = STATE_JUMP_ANIMATION;
				continue;
			}
			if (command == IR_KEY_ANIM_FADE) {
				current_state = STATE_FADE_ANIMATION;
				continue;
			}
			if (command == IR_KEY_ANIM_STROBE) {
				current_state = STATE_STROBE_ANIMATION;
				// For some reason this continue increases code size
				// continue;
			}
			
			// Otherwise try to set color value
			if (anim_set_color_by_key(command)) {
				current_state = STATE_COLOR;
				anim_set_brightness(brightness);
			}
		}
		
		// If IR_KEY_POWER_OFF has been pressed - disable all outputs and go to sleep
		if (has_power_off_bit(current_state)) {
			if (ir_is_idle_state()) power_off();
			continue;
		}
		
		// Calculate animation step
		if (++anim_counter >= anim_rate) {
			if (current_state == STATE_JUMP_ANIMATION) anim_jump(anim_local_counter); else
			if (current_state == STATE_FADE_ANIMATION) anim_fade(anim_local_counter); else
			if (current_state == STATE_STROBE_ANIMATION) anim_strobe(anim_local_counter); else
			if (current_state == STATE_RAINBOW_ANIMATION) anim_rainbow();
			anim_local_counter++;
			anim_counter = 0;
		}
		
		// Do PWM
		pwm_rgb(r_current_value, g_current_value, b_current_value);

	}
	
	return 0;
}