#include "../include/sysCallsHandler.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "../include/interrupts.h"
#include "../include/time.h"
#include "../include/soundDriver.h"

static uint8_t bcd_to_dec(uint8_t bcd);

extern void haltcpu(void);
extern uint64_t userland_registers_state[17];
extern  uint8_t inb(uint16_t port);
extern  void outb(uint16_t port, uint8_t val);
extern  void outw(uint16_t port, uint16_t val);

uint64_t syscall_dispatcher(uint64_t syscall_number, uint64_t fd, uint64_t buffer, uint64_t count) {
    switch (syscall_number) {
        case SYS_WRITE:
            sys_write(fd, buffer, count);
            break;
        case SYS_READ:
            sys_read(fd, buffer, count);
            break;
        case SYS_IOCTL:
            sys_ioctl(fd, count);
            break;
        case SYS_TIME:
            return sys_time(count);
            break;
        case SYS_EXIT:
            return sys_exit(fd);
        case SYS_GET_REGISTERS:
            sys_get_registers((uint64_t *)buffer);
            break;
        case SYS_CLEAR_SCREEN:
            clear_screen(count);
            break;
        case SYS_DRAW_TEXT:
            sys_draw_text(fd, buffer, count);
            break;
        case SYS_NANOSLEEP:
            sys_nanosleep(count);
            break;
        case SYS_BEEP:
            sys_beep(fd, count);
            break;
        case SYS_DRAW_FIGURE:
            sys_draw_figure(fd, buffer, count);
            break;
        case SYS_GET_SCREEN_INFO:
            return sys_get_screen_info();
        case SYS_TOGGLE:
            sys_toggle(fd);
            break;
        case SYS_READ_BY_KEY:
            return sys_read_by_keys(fd,buffer , count);
    }
}

void sys_write(uint64_t fd, uint8_t *buffer, uint64_t count) {
    uint64_t color = 0x00FFFFFF;
    if (fd == STDERR) {
        color = 0x00FF0000;
    }

    for (uint64_t i = 0; i < count; i++) {
        print_char_color(buffer[i], color);
    }
}

uint64_t sys_read(uint64_t fd, uint8_t *buffer, uint64_t count) {
    uint64_t readBytes = 0;
    if (fd == STDOUT) {
        return 0;
    }

    _sti();
    start_input();
	while (is_activated_keyboard());
    _cli();

    uint8_t * bufferKeyBoard = get_buffer();
    for (uint64_t i = 0; i < count; i++) {
        if (bufferKeyBoard[i] == 0) {
            buffer[readBytes] = 0;
            free_buffer();
            return readBytes;
        }
        buffer[readBytes++] = bufferKeyBoard[i];
    }

    buffer[readBytes] = 0;

    free_buffer();

    return readBytes;
}

void sys_ioctl(uint64_t cmd, uint64_t arg) {
    switch(cmd) {
        case IOCTL_SET_FONT_SIZE:
            modify_font_size(arg);
            break;
    }
}

static uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint64_t sys_time(int timezone) {
    char seconds, minutes, hours;

	outb(0x70, 0x00);
    seconds = inb(0x71);
    seconds = bcd_to_dec(seconds);

    outb(0x70, 0x02);
    minutes = inb(0x71);
    minutes = bcd_to_dec(minutes);

    outb(0x70, 0x04);
    hours = inb(0x71);
    hours = bcd_to_dec(hours) + timezone;

    if (hours >= 24) {
        hours -= 24; //
    } else if (hours < 0) {
        hours += 24;
    }

    return (hours * 3600) + (minutes * 60) + seconds;
}

void sys_get_registers(uint64_t * buffer) {
    for (int i = 0; i < CANT_REGISTERS; i++)
        (buffer)[i] = userland_registers_state[i];
}

uint64_t sys_exit(uint64_t exit_code) {

    disable_cursor();

    print_string_color("Apagando el sistema... ", 0x00FFFF00);
    print_string_color("Codigo de salida: ", 0x00FFFF00);

    print_int_color(exit_code, 0x00FFFF00);

    print_string("\n Apagado en: \n");

    _sti();

    uint64_t oldTicks = ticks_elapsed();
    uint64_t wait = 3;
    while (wait > 0) {
        uint64_t currentTick = ticks_elapsed();
        if ((currentTick - oldTicks) >= 18) {
            print_string("     ");
            print_int(wait);
            print_char('\n');
            oldTicks = currentTick;
            wait--;
        }
    }

    oldTicks = ticks_elapsed();

    while ((ticks_elapsed() - oldTicks)  <= 9);

    print_string_color("\nSistema apagado", 0x00FF0000);

    oldTicks = ticks_elapsed();

    while ((ticks_elapsed() - oldTicks)  <= 9);

    _cli();

    outw(0x604, 0x2000);

    outw(0xB004, 0x0 | 0x2000);


    _sti();
    haltcpu();
    _cli();

    return exit_code;
}

void sys_clear_screen(uint64_t color) {
    clear_screen(color);
}

void sys_draw_figure(uint64_t id, uint64_t * info, uint64_t color) {
    switch (id) {
        case RECTANGLE:
            draw_rectangle(info[X_POS], info[Y_POS], info[WIDTH], info[HEIGHT], color);
            break;
        case CIRCLE:
            draw_circle(info[X_POS], info[Y_POS], info[RADIUS], color);
            break;
        default:
            print_string_color("Figura no reconocida", 0x00FF0000);
            break;
    }
}

void sys_beep(uint64_t frec, uint64_t ms) {
    play_sound(frec, ms);
}

void sys_draw_text(uint64_t* xy, uint8_t * buffer, uint64_t* format) {
    print_text_in_screen(xy[0], xy[1] , buffer, format[1], format[0]);
}

uint64_t sys_get_screen_info() {
    uint64_t screen_info = 0;
    screen_info |= (uint64_t)get_screen_width() << 32;
    screen_info |= (uint64_t)get_screen_height();
    return screen_info;
}

void sys_nanosleep(uint64_t ms) {
    if (ms == 0) {
        return;
    }
    _sti();
    uint64_t oldTicks = ticks_elapsed();
    uint64_t ticks_to_wait = (ms * 18) / 1000;
    while ((ticks_elapsed() - oldTicks) < ticks_to_wait);
    _cli();
}

void sys_toggle(uint64_t state) {
    if (state == 0) {
        disable_cursor();
    } else if (state == 1) {
        enable_cursor();
    }
}

uint64_t sys_read_by_keys(uint8_t * str, uint8_t * buffer, uint64_t count) {

    uint64_t readBytes = 0;

    _sti();
    start_input();
    enable_discriminated_keys();
    set_discrimanted_keys(str);
    while (is_activated_keyboard());
    _cli();

    uint8_t * bufferKeyBoard = get_buffer();

    for (int i = 0; i < count; i++) {
        if (bufferKeyBoard[i] == 0) {
            buffer[readBytes] = 0;
            free_buffer();
            disable_discriminated_keys();
            return readBytes;
        }
        buffer[readBytes++] = bufferKeyBoard[i];
    }

    buffer[readBytes] = 0;
    free_buffer();
    disable_discriminated_keys();

    return readBytes;

}

