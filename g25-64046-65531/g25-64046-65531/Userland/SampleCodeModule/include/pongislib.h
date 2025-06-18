#include <stdint.h>

#ifndef PONGISLIB_H
#define PONGISLIB_H

#define SYS_TOGGLE 105
#define BUFFER_SIZE 256
#define CMD_COUNT 10
#define MAX_ARGS 10
#define SYS_IOCTL 16
#define SYS_TIME 96
#define SYS_GET_REGISTERS 335
#define SYS_CLEAR_SCREEN 100
#define SYS_GET_SCREEN_INFO 101
#define SYS_DRAW_FIGURE 102
#define SYS_BEEP 103
#define SYS_DRAW_TEXT 104
#define SYS_NANOSLEEP 35
#define SYS_READ_BY_KEY 106
#define IOCTL_SET_FONT_SIZE 0x4B01
#define MAX_HEXA_LENGTH 20 // "0x" + 16 digits + null terminator
#define RECTANGLE 0
#define CIRCLE 1
#define X_POS 0
#define Y_POS 1
#define RADIUS 2
#define WIDTH 2
#define HEIGHT 3
#define RECTANGLE_ELEMS 4
#define CIRCLE_ELEMS 3
#define TEXT_ELEMS 2
#define BUFFER_KEYS_SIZE 256
#define CANT_KEYS 9
#define FORMAT_TEXT 2
#define BACKGROUND_POSITION 1
#define COLOR_POSITION 0
#define PONGIS_BACKGROUND 0x004AE3C1

extern uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void make_sound(uint64_t frec, uint64_t ms);

void draw_rectangle(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t color);

void draw_circle(uint64_t x, uint64_t y, uint64_t r, uint64_t color);

void print_text(uint64_t x, uint64_t y, uint8_t* str, uint64_t color);

void sleep(uint64_t ms);

uint64_t get_screen_size();

void get_keys(uint8_t* buffer);

void paint_screen(uint64_t color);

void disable_toggle();

void enable_toggle();


#endif //PONGISLIB_H
