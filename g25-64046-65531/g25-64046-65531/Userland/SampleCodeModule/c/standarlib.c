#include "../include/standarlib.h"
#include <stdarg.h>
#include "../include/standarlib.h"

static uint64_t size(uint8_t *s);
static void reset_input_buffer();
static void reverse(uint8_t *str, uint64_t length);
static int int_to_string(uint64_t num, uint8_t *str);
static uint64_t auxfd_printf(uint64_t fd, const uint8_t *fmt, va_list args);

static uint64_t auxfd_printf(uint64_t fd, const uint8_t *fmt, va_list args) {
    uint8_t buffer[BUFFER_SIZE];
    int i = 0;
    int j = 0;

    while(fmt[i] != '\0' && j < BUFFER_SIZE - 1) {
        if (fmt[i] != '%'){
            buffer[j++] = fmt[i++];
        } else {

            i++;

            if (fmt[i] == '\0') {
                va_end(args);
                buffer[j] = '\0';
                return _syscall(SYS_WRITE, STDERR, "Format error: incomplete format specifier\n", 39);
            }

            switch (fmt[i]) {
            case 'd':
                int num = va_arg(args, int);
                uint8_t numBuffer[NUM_BUFFER_SIZE];
                int_to_string(num, numBuffer);
                for (uint64_t k = 0; numBuffer[k] != '\0' && j < BUFFER_SIZE - 1; k++) {
                    buffer[j++] = numBuffer[k];
                }
                break;

            case 's':
                uint8_t * s = va_arg(args, uint8_t *);
                for (uint64_t k = 0; s[k] != '\0'; k++) {
                    buffer[j++] = s[k];
                }
                break;
            case 'c':
                uint8_t c = (uint8_t)va_arg(args, int);
                buffer[j++] = c;
                break;
            default:
                buffer[j++] = '%';
                buffer[j++] = fmt[i];
                break;
            }
            i++;
        }
    }

    buffer[j] = '\0';

    return _syscall(SYS_WRITE, fd, (uint64_t)buffer, j);
}

static int int_to_string(uint64_t num, uint8_t *str) {
    uint64_t i = 0;
    uint8_t isNegative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str, i);

    return i;    
}

static void reverse(uint8_t *str, uint64_t length) {
    uint64_t start = 0;
    uint64_t end = length - 1;
    while (start < end) {
        uint8_t aux = str[start];
        str[start] = str[end];
        str[end] = aux;
        start++;
        end--;
    }
}

uint64_t fd_printf(uint64_t fd, const uint8_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    uint64_t result = auxfd_printf(fd, fmt, args);
    va_end(args);
    reset_input_buffer();
    return result;
}

uint64_t printf(const uint8_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    uint64_t result = auxfd_printf(STDOUT, fmt, args);
    va_end(args);
    reset_input_buffer();
    return result;
}

static uint64_t size(uint8_t *s) {
    uint64_t count = 0;
    while (*s) {
        count++;
        s++;
    }

    return count;
}

void putc(uint8_t c) {

    uint8_t cP[] = {c};

    _syscall(SYS_WRITE, STDOUT, cP, 1);
    reset_input_buffer();


}

void puts(uint8_t * s) {
    uint64_t stringSize = size(s);
    _syscall(SYS_WRITE, STDOUT, s, stringSize);
    uint8_t newLine[] = {'\n'};
    _syscall(SYS_WRITE, STDOUT, newLine, 1);
    reset_input_buffer();

}

static uint8_t inputBuffer[BUFFER_SIZE];
static uint64_t inputPos = 0;
static uint64_t inputSize = 0;

uint8_t getc() {

    if (inputPos >= inputSize) {
        inputSize = _syscall(SYS_READ, STDIN, inputBuffer, BUFFER_SIZE);

        inputPos= 0;

        if (inputSize <= 0) {
            inputSize = 0;
            return '\0';
        }
    }

    uint8_t c = inputBuffer[inputPos++];
    return c;
}

static void reset_input_buffer() {
    inputPos = 0;
    inputSize = 0;
}

uint8_t * gets() {
    static uint8_t buffer[BUFFER_SIZE];
    _syscall(SYS_READ, STDIN, buffer, BUFFER_SIZE - 1);
    return buffer;
}

uint64_t scanf(const uint8_t *fmt, ...) {
    uint8_t * input = gets();
    va_list args;
    uint64_t fmtpos = 0;
    uint64_t inputpos = 0;
    uint64_t cantArgs = 0;

    va_start(args, fmt);

    while(fmt[fmtpos] != '\0') {
        if (fmt[fmtpos] != '%') {
            fmtpos++;
        } else {
            while(input[inputpos] == ' ') {
                inputpos++;
            }
            fmtpos++;
            switch (fmt[fmtpos]) {
                case 'd': {
                    int * num_ptr = va_arg(args, int *);
                    int sign = 1;
                    if (input[inputpos] == '-') {
                        sign = -1;
                        inputpos++;
                    } else if (input[inputpos] == '+') {
                        inputpos++;
                    }
                    *num_ptr = 0;
                    while (input[inputpos] >= '0' && input[inputpos] <= '9') {
                        *num_ptr = (*num_ptr * 10) + (input[inputpos] - '0');
                        inputpos++;
                    }
                    *num_ptr *= sign;
                    cantArgs++;
                    break;
                }
                case 's': {
                    uint8_t * str = va_arg(args, uint8_t *);
                    int j = 0;
                    while (input[inputpos] != '\0' && input[inputpos] != ' ') {
                        str[j++] = input[inputpos++];
                    }
                    str[j] = '\0';
                    break;
                }
                case 'c': {
                    uint8_t * c = va_arg(args, uint8_t *);
                    *c = input[inputpos++];
                    break;
                }
                default:
                    va_end(args);
                    return _syscall(SYS_WRITE, STDERR, "Format error: invalid format specifier\n", 39);
            }
        }
    }
    va_end(args);
    reset_input_buffer();
    return cantArgs;
}

uint64_t strcmp(uint8_t* s1, uint8_t* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}
