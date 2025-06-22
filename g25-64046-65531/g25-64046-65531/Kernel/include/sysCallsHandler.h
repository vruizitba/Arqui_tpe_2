#include <stdint.h>

#ifndef SYS_CALLS_HANDLER_H
#define SYS_CALLS_HANDLER_H

#define SYS_WRITE 4
#define SYS_READ 0
#define SYS_IOCTL 16
#define SYS_TIME 96
#define SYS_EXIT 60
#define SYS_GET_REGISTERS 335
#define CANT_REGISTERS 19
#define SYS_CLEAR_SCREEN 100
#define SYS_GET_SCREEN_INFO 101
#define SYS_DRAW_FIGURE 102
#define SYS_BEEP 103
#define SYS_DRAW_TEXT 104
#define SYS_TOGGLE 105
#define SYS_READ_BY_KEY 106
#define SYS_NANOSLEEP 35
#define RECTANGLE 0
#define CIRCLE 1
#define X_POS 0
#define Y_POS 1
#define RADIUS 2
#define WIDTH 2
#define HEIGHT 3
#define FREC_STD_PIT 1193180


#define IOCTL_SET_FONT_SIZE 0x4B01

#define STDOUT 1
#define STDERR 2

#define FONT_INCREASE 1
#define FONT_DECREASE 0

uint64_t syscall_dispatcher(uint64_t syscall_number, uint64_t fd, uint64_t buffer, uint64_t count);

void sys_write(uint64_t fd, uint8_t *buffer, uint64_t count);

uint64_t sys_read(uint64_t fd, uint8_t *buffer, uint64_t count);

void sys_ioctl(uint64_t cmd, uint64_t arg);

uint64_t sys_time(int timezone);

uint8_t sys_get_registers(uint64_t * buffer);

uint64_t sys_exit(uint64_t exit_code);

void sys_clear_screen(uint64_t color);

void sys_draw_figure(uint64_t id, uint64_t * info, uint64_t color);

void sys_beep(uint64_t frec, uint64_t ms);

void sys_draw_text(uint64_t* xy, uint8_t* buffer, uint64_t* format);

uint64_t sys_get_screen_info();

void sys_nanosleep(uint64_t ms);

void sys_toggle(uint64_t state);

uint64_t sys_read_by_keys(uint8_t * str, uint8_t * buffer, uint64_t count);

#endif //SYS_CALLS_HANDLER_H
