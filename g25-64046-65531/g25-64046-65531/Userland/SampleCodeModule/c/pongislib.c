#include "../include/pongislib.h"

void make_sound(uint64_t frec, uint64_t ms) {
    _syscall(SYS_BEEP, frec, 0, ms);
}

void draw_rectangle(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t color) {

    uint64_t info[RECTANGLE_ELEMS];
    info[X_POS] = x;
    info[Y_POS] = y;
    info[WIDTH] = w;
    info[HEIGHT] = h;

    _syscall(SYS_DRAW_FIGURE, RECTANGLE, info,color);
}

void draw_circle(uint64_t x, uint64_t y, uint64_t r, uint64_t color) {
    uint64_t info[CIRCLE_ELEMS];
    info[X_POS] = x;
    info[Y_POS] = y;
    info[RADIUS] = r;
    _syscall(SYS_DRAW_FIGURE, CIRCLE, info,color);
}

void print_text(uint64_t x, uint64_t y, uint8_t* str, uint64_t color) {
    uint64_t info[TEXT_ELEMS];
    info[X_POS] = x;
    info[Y_POS] = y;
    uint64_t format[FORMAT_TEXT];
    format[COLOR_POSITION] = color;
    format[BACKGROUND_POSITION] = 0x004AE3C1;
    _syscall(SYS_DRAW_TEXT, info, str,format);

}

void sleep(uint64_t ms) {
    _syscall(SYS_NANOSLEEP, 0, 0, ms);
}

uint64_t get_screen_size() {
    return _syscall(SYS_GET_SCREEN_INFO, 0, 0, 0);
}

void get_keys(uint8_t* buffer) {

    uint8_t keys[CANT_KEYS] = {'a','w','s','d','i','j','k','l',' '};

    _syscall(SYS_READ_BY_KEY, keys, buffer, BUFFER_SIZE);

}

void paint_screen(uint64_t color) {
    _syscall(SYS_CLEAR_SCREEN, 0, 0, color);
}

void disable_toggle() {
    _syscall(SYS_TOGGLE, 0, 0, 0);
}

void enable_toggle() {
    _syscall(SYS_TOGGLE, 1, 0, 0);
}