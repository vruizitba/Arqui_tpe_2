
#include "../include/console.h"
#include "../include/standarlib.h"
#include "../include/pongis.h"
#include "../include/man.h"

typedef void (*cmd_function)(uint64_t arc, uint8_t* argv[]);

extern uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

static void process_cmd(uint8_t * input);
static uint8_t str_to_int(const uint8_t* str);
static void int_to_hex_char(uint64_t value, uint8_t * buffer);
static void split_string(uint8_t* input, uint8_t* args[], uint64_t* argc);

static uint8_t * cmdArray[] = {"exit", "args", "man", "time", "registers", "font", "divzero", "invalidop", "clear", "pongis"};
static cmd_function functionsArray[CMD_COUNT] = {exit_shell, args_shell, man_shell, time_shell, registers_shell, font_shell, div_zero_shell, invalid_opcode_shell, clear_shell, pongis_main};


void args_shell(uint64_t argc, uint8_t* argv[]) {
    for (uint64_t i = 0; i < argc; i++) {
        puts(argv[i]);
    }
}

void exit_shell() {
    _syscall(SYS_EXIT, 0, 0, 0);
}


void start_shell() {

    uint8_t cmd[BUFFER_SIZE];

    while (1) {

        printf("TPE_ARQUI_CONSOLE:~$ ");

        uint64_t i = 0;
        uint8_t c;

        while ((c = getc()) != '\n' && i < BUFFER_SIZE-1) {
            cmd[i++] = c;
        }

        cmd[i] = '\0';
        process_cmd(cmd);
    }
}

void process_cmd(uint8_t * input) {

    uint8_t * args[MAX_ARGS] = {0};
    uint64_t argc = 0;

    uint8_t buffer[BUFFER_SIZE];

    uint64_t i = 0;
    while (input[i] != '\0') {
        buffer[i] = input[i];
        i++;
    }
    buffer[i] = '\0';

    split_string(buffer, args, &argc);

    uint64_t cmdIndex = 0;
    uint8_t found = 0;

    for (i = 0; i < CMD_COUNT; i++) {
        if (strcmp(args[0], cmdArray[i]) == 0) {
            cmdIndex = i;
            found = 1;
        }
    }

    if (found == 1) {
        functionsArray[cmdIndex](argc, args);
    } else {
        puts("Command not valid");
    }
}

static void split_string(uint8_t* input, uint8_t* args[], uint64_t* argc) {
    *argc = 0;
    uint8_t i = 0;
    uint8_t inToken = 0;

    while (input[i] != '\0') {
        if (input[i] == ' ' || input[i] == '\t') {
            input[i] = '\0';
            inToken = 0;
        } else if (!inToken) {
            args[*argc] = &input[i];
            (*argc)++;
            inToken = 1;
        }
        i++;
    }
}

void man_shell(uint64_t argc, uint8_t* argv[]) {
    if (argc == 1) {
        puts("TPE_ARQUI_CONSOLE Manual Pages\n");
        puts("Available commands:");
        puts("  man        - display manual pages");
        puts("  time       - display current time");
        puts("  exit       - terminate the shell");
        puts("  registers  - display system registers");
        puts("  font       - change console font size");
        puts("  args       - display command arguments (debug)");
        puts("  divzero    - trigger division by zero exception");
        puts("  invalidop  - trigger invalid opcode exception");
        puts("  clear      - clear the terminal screen");
        puts("  pongis     - play pong game");
        puts("\nUse 'man <command>' for detailed information about a command.");
        return;
    }

    if (argc == 2) {
        for (uint64_t i = 0; i < 10; i++) { // Ajustado para 10 comandos
            if (strcmp(argv[1], command_names[i]) == 0) {
                puts(manualPages[i]);
                return;
            }
        }
        puts("No manual entry for ");
        puts(argv[1]);
        return;
    }
    puts("Usage: man [command]");
}

static uint8_t str_to_int(const uint8_t* str) {
    uint64_t num = 0;
    int sign = 1;
    uint64_t i = 0;

    if (str[0] == '\0') {
        return 0;
    }

    if (str[0] == '-') {
        sign = -1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }

    if (str[i] == '\0') {
        return 0;
    }

    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
        num = (num * 10) + (str[i] - '0');
        i++;
    }

    return num * sign;
}

void time_shell(uint64_t argc, uint8_t* argv[]) {

    if (argc != 2) {
        puts("Usage: time <timezone>");
        return;
    }

    uint64_t time = _syscall(SYS_TIME, 0, 0, str_to_int(argv[1]));
    uint8_t buffer[9];
    buffer[0] = (time / 3600) / 10 + '0';
    buffer[1] = (time / 3600) % 10 + '0';
    buffer[2] = ':';
    buffer[3] = (time / 60) % 60 / 10 + '0';
    buffer[4] = (time / 60) % 60 % 10 + '0';
    buffer[5] = ':';
    buffer[6] = time % 60 / 10 + '0';
    buffer[7] = time % 60 % 10 + '0';
    buffer[8] = '\0';
    puts(buffer);
}

static void int_to_hex_char(uint64_t value, uint8_t * buffer) {
    const uint8_t hexDigits[] = "0123456789ABCDEF";
    uint64_t idx = 0;

    buffer[idx++] = '0';
    buffer[idx++] = 'x';

    for(int i = 15; i >= 0; i--) {
        buffer[idx++] = hexDigits[(value >> (i * 4)) & 0xF];
    }
    
    buffer[idx] = '\0';    
}

void registers_shell() {
    uint64_t regs[17];
    _syscall(SYS_GET_REGISTERS, 0, (uint64_t)regs, 0);

    const uint8_t* regNames[] = {
        "RAX", "RBX", "RCX", "RDX", "RBP", "RSP",
        "RSI", "RDI", "R8 ", "R9 ", "R10", "R11",
        "R12", "R13", "R14", "R15", "RIP"
    };

    uint8_t hex_buffer[MAX_HEXA_LENGTH];

    puts("Registros actuales:");
    for (uint64_t i = 0; i < 17; i++) {
        int_to_hex_char(regs[i], hex_buffer);
        printf("%s: %s\n", regNames[i], hex_buffer);
    }
}


void font_shell(uint64_t argc, uint8_t* argv[]) {

    uint8_t
    c = argv[1][0];
    if (c < '0' || c > '1') {
        return;
    }
    _syscall(SYS_IOCTL, IOCTL_SET_FONT_SIZE, 0, c - '0');
}

void div_zero_shell() {
    int a = 1;
    int b = 0;
    volatile int c = a / b;
}

void invalid_opcode_shell() {
    __asm__ volatile ("ud2");
}

void clear_shell() {
    uint64_t black = 0x00000000;
    _syscall(100, 0, 0, black);
}
