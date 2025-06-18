#include <stdint.h>


#ifndef SHELL_H
#define SHELL_H


#define BUFFER_SIZE 256
#define CMD_COUNT 10
#define MAX_ARGS 10
#define SYS_IOCTL 16
#define SYS_TIME 96
#define SYS_EXIT 60
#define SYS_GET_REGISTERS 335
#define SYS_CLEAR_SCREEN 100
#define SYS_GET_SCREEN_INFO 101
#define SYS_DRAW_FIGURE 102
#define SYS_BEEP 103
#define SYS_DRAW_TEXT 104
#define SYS_NANOSLEEP 35
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
#define CANT_REGISTERS 19


void start_shell();

void exit_shell();

void inc_font();

void man_shell();

void time_shell(uint64_t argc, uint8_t* argv[]);

void registers_shell();

void args_shell(uint64_t argc, uint8_t* argv[]);

void font_shell(uint64_t argc, uint8_t* argv[]);

void div_zero_shell();

void invalid_opcode_shell();

void clear_shell();

void prueba();

#endif //SHELL_H
