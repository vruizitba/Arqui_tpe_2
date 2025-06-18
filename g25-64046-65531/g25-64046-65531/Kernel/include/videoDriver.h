#include <stdint.h>

#define MAX_HEIGHT 2160  // Soporta hasta resoluciones 4K
#define MAX_LINES (MAX_HEIGHT/MIN_FONT_HEIGHT) // Tomo maxima resolución y mínima altura por linea
#define MAX_WIDTH 3840
#define MIN_FONT_WIDTH 8
#define MAX_COLUMNS (MAX_WIDTH/MIN_FONT_WIDTH)

#define VIDEODRIVER_H


void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);

void print_char(uint8_t c);

void print_char_color(uint8_t c, uint32_t hexColor);

void print_string(const uint8_t * str);

void print_string_color(const uint8_t * str, uint32_t hexColor);

void toggle_cursor();

void modify_font_size(uint64_t size);

void enable_cursor();

void disable_cursor();

uint8_t get_is_toggle_active();

void clear_screen(uint64_t color);

void print_text_in_screen(uint64_t xScreen, uint64_t yScreen, uint8_t* buffer, uint64_t backGround, uint64_t color);

uint16_t get_screen_width();

uint16_t get_screen_height();

void draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);

void draw_circle(uint64_t x, uint64_t y, uint64_t radius, uint64_t color);

uint64_t print_int(uint64_t value);

uint64_t print_int_color(uint64_t value, uint32_t hexColor);

#ifndef VIDEODRIVER_H

#endif //VIDEODRIVER_H
