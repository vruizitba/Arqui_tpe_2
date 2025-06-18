#include <time.h>
#include "../include/videoDriver.h"

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	if (get_is_toggle_active()) {
		cursor_refresh();
	}
}

uint64_t ticks_elapsed() {
	return ticks;
}

uint64_t seconds_elapsed() {
	return ticks / 18;
}

void cursor_refresh() {
	if (ticks % 9 == 0) {
		toggle_cursor();
	}
}