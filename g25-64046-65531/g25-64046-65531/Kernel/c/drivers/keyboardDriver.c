#include "../include/keyboardDriver.h"
#include "../include/videoDriver.h"

const uint8_t scancodeToChar[128][3] = {
    {0, 0, 0},           // 0
    {0, 0, 0},           // 1  ESC
    {'1', '!', '1'},     // 2
    {'2', '@', '2'},     // 3
    {'3', '#', '3'},     // 4
    {'4', '$', '4'},     // 5
    {'5', '%', '5'},     // 6
    {'6', '^', '6'},     // 7
    {'7', '&', '7'},     // 8
    {'8', '*', '8'},     // 9
    {'9', '(', '9'},     // 10
    {'0', ')', '0'},     // 11
    {'-', '_', '-'},     // 12
    {'=', '+', '='},     // 13
    {'\b', '\b', '\b'},  // 14
    {'\t', '\t', '\t'},  // 15
    {'q', 'Q', 'Q'},     // 16
    {'w', 'W', 'W'},     // 17
    {'e', 'E', 'E'},     // 18
    {'r', 'R', 'R'},     // 19
    {'t', 'T', 'T'},     // 20
    {'y', 'Y', 'Y'},     // 21
    {'u', 'U', 'U'},     // 22
    {'i', 'I', 'I'},     // 23
    {'o', 'O', 'O'},     // 24
    {'p', 'P', 'P'},     // 25
    {'[', '{', '['},     // 26
    {']', '}', ']'},     // 27
    {'\n', '\n', '\n'},  // 28
    {0, 0, 0},           // 29
    {'a', 'A', 'A'},     // 30
    {'s', 'S', 'S'},     // 31
    {'d', 'D', 'D'},     // 32
    {'f', 'F', 'F'},     // 33
    {'g', 'G', 'G'},     // 34
    {'h', 'H', 'H'},     // 35
    {'j', 'J', 'J'},     // 36
    {'k', 'K', 'K'},     // 37
    {'l', 'L', 'L'},     // 38
    {';', ':', ';'},     // 39
    {'\'', '"', '\''},   // 40
    {'`', '~', '`'},     // 41
    {0, 0, 0},           // 42
    {'\\', '|', '\\'},   // 43
    {'z', 'Z', 'Z'},     // 44
    {'x', 'X', 'X'},     // 45
    {'c', 'C', 'C'},     // 46
    {'v', 'V', 'V'},     // 47
    {'b', 'B', 'B'},     // 48
    {'n', 'N', 'N'},     // 49
    {'m', 'M', 'M'},     // 50
    {',', '<', ','},     // 51
    {'.', '>', '.'},     // 52
    {'/', '?', '/'},     // 53
    {0, 0, 0},           // 54
    {'*', '*', '*'},     // 55
    {0, 0, 0},           // 56
    {' ', ' ', ' '},     // 57
    {0, 0, 0},           // 58
};

extern uint8_t inb(uint16_t port);
extern void _update_registers_userland();

void static print_scancode(uint8_t c);
void static buffer(uint8_t c);

static uint8_t isActivated = 0;
static uint8_t shiftPressed = 0;
static uint8_t capsLockActive = 0;
static uint8_t discriminateKeys = 0;
static uint8_t * keyToDiscriminate;
static uint8_t bufferArray[BUFFER_SIZE];
static uint64_t bufferElements = 0;

void enable_discriminated_keys() {
    discriminateKeys = 1;
}

void disable_discriminated_keys() {
    discriminateKeys = 0;
}

void set_discrimanted_keys(uint8_t* keys) {
    keyToDiscriminate = keys;
}

void keyboard_handler() {

    uint8_t key = inb(0x60);

    if (key == 0x01) {
        _update_registers_userland();
    }

    uint8_t keyAscii  = scancodeToChar[key][0];

    if (key & 0x80) {
        if (discriminateKeys == 1) {
            uint8_t auxKey = scancodeToChar[key & 0x7F][0];
            for (uint64_t i = 0; keyToDiscriminate[i]; i++) {
                if (auxKey == keyToDiscriminate[i]) {
                    end_input();
                    return;
                }
            }
        }
        key &= 0x7F;
        if (key == LEFT_SHIFT || key == RIGHT_SHIFT) {
            shiftPressed = 0;
        }
        return;
    }

    switch(key) {
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            shiftPressed = 1;
            return;
        case CAPS_LOCK:
            capsLockActive = !capsLockActive;
            return;
    }

    if (shiftPressed) {
        keyAscii = scancodeToChar[key][1];
    } else if (capsLockActive) {
        keyAscii = scancodeToChar[key][2];
    }

    if (discriminateKeys) {
        uint8_t found = 0;
        for (uint64_t i = 0; keyToDiscriminate[i] && !found; i++) {
            if (keyToDiscriminate[i] == keyAscii) {
                found = 1;
            }
        }
        if (!found) {
            return;
        }
    }

    if (keyAscii == 0) {
        return;
    }
    
    if (keyAscii == '\b' && isActivated) {
        print_scancode(keyAscii);
        buffer(keyAscii);
        return;
    }

    if (isActivated) {
        buffer(keyAscii);
    }

    if (!discriminateKeys) {
        print_scancode(keyAscii);
    }
}

void static print_scancode(uint8_t c) {

    if (c && isActivated) {
        print_char(c);
    }

    if (c == '\n') {
        end_input();
    }
}

void start_input() {
    isActivated = 1;
}

void end_input() {
    isActivated = 0;
}

uint8_t is_activated_keyboard() {
    return isActivated;
}

void static buffer(uint8_t c) {

    if (c == '\b') {
        if (bufferElements != 0) {
            bufferArray[bufferElements-- - 1] = 0;
        }
        return;
    }

    bufferArray[bufferElements++] = c;
}

void free_buffer() {
    for (uint64_t i = 0; i < bufferElements; i++) {
        bufferArray[i] = 0;
    }
    bufferElements = 0;
}

uint8_t * get_buffer() {
    return bufferArray;
}
