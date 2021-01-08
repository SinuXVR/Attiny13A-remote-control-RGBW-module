/*
 * NEC protocol IR receiver logic
 *
 * Usage:
 * Connect IR receiver output to any pin defined as IR_SENSOR_PIN
 * Call ir_init() once after start
 * Poll ir_has_new_command(), it will return 1 when the next command is received
 * Call ir_get_received_command() and process the command
 *
 *  Author: SinuX
 *  License: CC BY-NC-SA
 */ 

#include "common.h"
#include <avr/interrupt.h>

#ifndef IR_H_
#define IR_H_

// Timer control macros
#define ir_enable_timer() TCCR0B = (1 << CS02)
#define ir_timer_has_overflow() TIFR0 & (1 << TOV0)
#define ir_reset_timer_overflow() TIFR0 = (1 << TOV0)
#define ir_reset_timer_counter() TCNT0 = 1
#define ir_overflow_timer() TCNT0 = 255
#define ir_timer_counter TCNT0

// Some variables
static volatile byte ir_received_command = 0;
static volatile byte has_new_command = 0;

/**
 * Set up interrupts for IR receiver
 */
static void ir_init() {
	// Enable timer in normal mode with prescaler 256
	ir_enable_timer();
	
	// Enable PCIE interrupt on IR_SENSOR_PIN
	GIMSK |= (1 << PCIE);
	PCMSK |= (1 << IR_SENSOR_PIN);
	
	// Enable global interrupts
	sei();
}

/**
 * Check if there is no data reception at this time
 */
static byte ir_is_idle_state() {
	return ir_timer_has_overflow();
}

/**
 * Check if new data exists
 */
static byte ir_has_new_command() {
	return has_new_command;
}

/**
 * Return last received data
 */
static byte ir_get_received_command() {
	has_new_command = 0;
	return ir_received_command;
}

/**
 * PCINT0 interrupt from IR_SENSOR_PIN - decode data
 */
ISR (PCINT0_vect) {
	static byte bit_counter;
	static word data_buffer;
	
	// Consider only falling edge
	if (PINB & (1 << IR_SENSOR_PIN)) return;
	
	// Buffer current timer counter value and reset it
	byte timer_counter = ir_timer_counter;
	ir_reset_timer_counter();
	
	// Start of new data
	if (ir_timer_has_overflow()) {
		bit_counter = 0;
		timer_counter = 0;
		ir_reset_timer_overflow();
		return;
	}
	
	// Ignore start sequence
	if (timer_counter >= IR_START_SEQ_TICKS) return;

	// Set next bit to high or low
	data_buffer <<= 1;
	if (timer_counter >= IR_HIGH_BIT_TICKS) data_buffer |= 1;
	bit_counter++;
	
	// Current high and low byte values of data
	byte data_high_byte = data_buffer >> 8;
	byte data_low_byte = ~data_buffer;
	
	// End of address
	#ifdef IR_VALIDATE_ADDRESS
		// Cancel reception if address isn't correct
		if (bit_counter == 16) {
			if (data_high_byte != data_low_byte) {
				bit_counter = 0;
			}
	}
	#endif
	
	// End of data
	if (bit_counter == 32) {
		// Validate and save received command
		if (data_high_byte == data_low_byte) {
			ir_received_command = data_high_byte;
			has_new_command = 1;
		}
		ir_overflow_timer();
	}
	
	
}

#endif /* IR_H_ */