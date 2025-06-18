#include "../include/soundDriver.h"
#include "../include/time.h"
#include "../include/interrupts.h"

 
#define FREC_STD_PIT 1193180

typedef struct {
    uint64_t frequency;  // Frecuencia en Hz (0 para silencio)
    uint64_t duration;   // Duración en milisegundos
} Note;

extern  uint8_t inb(uint16_t port);

extern  void outb(uint16_t port, uint8_t val);

void play_sound(uint64_t frequency, uint64_t duration_ms) {

    if (frequency < 50 || frequency > 20000) {
        frequency = 800;
    }

    uint16_t divisor = FREC_STD_PIT / frequency;

    uint8_t port_state = inb(0x61);

    outb(0x43, 0xB6);
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);

    outb(0x61, port_state | 0x03);

    uint64_t oldTicks = ticks_elapsed();
    uint64_t ticks_to_wait = (duration_ms * 18) / 1000;

    _sti();
    while ((ticks_elapsed() - oldTicks) < ticks_to_wait);
    _cli();

    outb(0x61, port_state & 0xFC);
}
