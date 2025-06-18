#include "../../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "fonts.h"

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

static void new_line();
void static print_black(uint64_t x, uint64_t y);
void static delete();
void static scroll_screen();

static uint64_t x = 0;
static uint64_t y = 0;
static uint8_t cursorVisible = 1;
static uint32_t cursorColor = 0xFFFFFFFF;
static uint64_t fontPos = 0;
static uint64_t prevFontPos = 0;
static uint8_t isToggleActive = 1;
static uint64_t backgroundColor = 0x00000000;

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y) {
	if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
		return;
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}
void static scroll_screen() {
	uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
	uint32_t line_size = VBE_mode_info->pitch;
	uint32_t screen_size = line_size * VBE_mode_info->height;

	for (uint32_t i = 0; i < screen_size - line_size*fonts[fontPos].height; i++) {
		framebuffer[i] = framebuffer[i + line_size*fonts[fontPos].height];
	}

	for (uint32_t i = screen_size - line_size*fonts[fontPos].height; i < screen_size; i++) {
		framebuffer[i] = 0x00;
	}
}

void print_char_color(uint8_t c, uint32_t hexColor) {
	if (isToggleActive) {
		toggle_cursor();
	}

	if (c == '\n') {
		new_line();
		return;
	}

	if (c == '\b') {

		uint8_t * buffer = get_buffer();
		int size = 0;
		while (buffer[size] != 0) {
			size++;
		}
		if (size > 0) {
			delete();
		}
		return;
	}

	for (uint64_t i = 0; i < fonts[fontPos].height; i++) {
		for (uint64_t j = 0; j < fonts[fontPos].width; j++) {
			if ((fonts[fontPos].bitmap[(c * fonts[fontPos].height) + i] >> (fonts[fontPos].width - 1 - j)) & 0x01) {
				put_pixel(hexColor, x + j, y + i);
			} else {
				put_pixel(backgroundColor, x + j, y + i);
			}
		}
	}

	x += fonts[fontPos].width;
	if (x >= VBE_mode_info->width) {
		x = 0;
		y += fonts[fontPos].height;
		if (y >= VBE_mode_info->height) {
			scroll_screen();
			y -= fonts[fontPos].height;
		}
	}
	if (isToggleActive) {
		toggle_cursor();
	}
}

void print_char(uint8_t c) {
	print_char_color(c, 0x00FFFFFF);
}

void static delete() {
	if (!cursorVisible) {
        toggle_cursor();
    }
    uint64_t current_line = y/fonts[fontPos].height;
    
    if (x > 0) {
        x -= fonts[fontPos].width;
        print_black(x, y);
    } else if (current_line > 0) {
        y -= fonts[fontPos].height;
        print_black(x, y);
    }
	toggle_cursor();
}

void static print_black(uint64_t x, uint64_t y) {
	for (uint64_t i = 0; i < fonts[fontPos].height; i++) {
		for (uint64_t j = 0; j < fonts[fontPos].width; j++) {
			put_pixel(0x00000000, x + j, y + i);
		}
	}
}

static void new_line() {
	if (!cursorVisible) {
		toggle_cursor();
	}
    x = 0;
    y += fonts[fontPos].height;
    if (y >= VBE_mode_info->height) {
		scroll_screen();
        y -= fonts[fontPos].height;
    }
	toggle_cursor();
}

void print_string_color(const uint8_t * str, uint32_t hexColor) {
	for (int i = 0; str[i]; i++) {
		print_char_color(str[i], hexColor);
	}
}

void print_string(const uint8_t * str) {
	while (*str) {
		print_char(*str);
		str++;
	}
}

void toggle_cursor() {
	if (isToggleActive) {
		if (cursorVisible) {
			for (uint64_t i = 0; i < (uint64_t)(fonts[fontPos].height * 0.875); i++) {
				put_pixel(cursorColor, x, y + i);
			}
		} else {
			for (uint64_t i = 0; i < (uint64_t)(fonts[fontPos].height * 0.875); i++) {
				put_pixel(0x00000000, x, y + i);
			}
		}
		cursorVisible = !cursorVisible;
	}
}

void modify_font_size(uint64_t size) {
	if (size != fontPos) {
		if (size < FONT_COUNT) {
			clear_screen(0x00000000);
			prevFontPos = fontPos;
			fontPos = size;
		}
	}
}

void enable_cursor() {
	isToggleActive = 1;
}

void disable_cursor() {
	isToggleActive = 0;
}

uint8_t get_is_toggle_active() {
	return isToggleActive;
}

void clear_screen(uint64_t color) {
	x = 0;
	y = 0;;
	for (uint64_t i = 0; i < VBE_mode_info->height; i++) {
		for (uint64_t j = 0; j < VBE_mode_info->width; j++) {
			put_pixel(color, j, i);
		}
	}
}

void print_text_in_screen(uint64_t xScreen, uint64_t yScreen, uint8_t* buffer, uint64_t backGround, uint64_t color) {

	uint8_t auxIsToggleActive = isToggleActive;
	isToggleActive = 0;

	uint8_t prevfont = fontPos;
	fontPos = FONT_COUNT - 1;

	uint64_t oldBackgrund = backgroundColor;
	backgroundColor = backGround;

	uint64_t oldX = x;
	uint64_t oldY = y;

	x = xScreen;
	y = yScreen;

	print_string_color(buffer, color);

	x = oldX;
	y = oldY;

	backgroundColor = oldBackgrund;
	fontPos = prevfont;

	isToggleActive = auxIsToggleActive;

}

uint16_t get_screen_width() {
	return VBE_mode_info->width;
}

uint16_t get_screen_height() {
	return VBE_mode_info->height;
}

void draw_rectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color) {
    for (uint64_t i = 0; i < height; i++) {
        for (uint64_t j = 0; j < width; j++) {
            put_pixel(color, x + j, y + i);
        }
    }
}

void draw_circle(uint64_t x, uint64_t y, uint64_t radius, uint64_t color) {
    int64_t cx = x;
    int64_t cy = y;
    int64_t r = radius;

    uint64_t r2 = r * r;

    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r2) {
                put_pixel(color, cx + dx, cy + dy);
            }
        }
    }
}

uint64_t print_int(uint64_t value) {
	return print_int_color(value, 0x00FFFFFF);
}

uint64_t print_int_color(uint64_t value, uint32_t hexColor) {
	if (value == 0) {
		print_char_color('0', hexColor);
		return 1;
	}

	uint8_t buffer[21];
	int i = 0;
	uint64_t temp = value;

	while (temp > 0) {
		buffer[i++] = (temp % 10) + '0';
		temp /= 10;
	}

	uint64_t digits = i;
	while (i > 0) {
		print_char_color(buffer[--i], hexColor);
	}
	return digits;
}
