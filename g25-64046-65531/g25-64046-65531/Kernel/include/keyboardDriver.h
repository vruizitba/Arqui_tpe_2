#include <stdint.h>

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_SIZE 256

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define CAPS_LOCK 0x3A
#define LEFT_SHIFT_RELEASE 0xAA
#define RIGHT_SHIFT_RELEASE 0xB6

void keyboard_handler();

void free_buffer();

uint8_t * get_buffer();

void start_input();

void end_input();

uint8_t is_activated_keyboard();

void enable_discriminated_keys();

void disable_discriminated_keys();

void set_discrimanted_keys(uint8_t* keys);


#endif //KEYBOARD_H
