#include "../include/exceptions.h"
#include "../include/videoDriver.h"

typedef enum {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    RBP = 4,
    RSP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
    RIP = 16
} RegisterID;

void exceptionDispatcher(uint64_t exception) {
    switch (exception) {
        case ZERO_EXCEPTION_ID:
            zero_division();
            break;
        case INVALID_OPCODE_ID:
            invalid_opcode();
            break;
    }
}

void zero_division() {
    print_string_color("Error: Division by zero\n", 0x00FF0000);
    print_string("Registers at time of exception:\n");
    printRegisters();
}

void invalid_opcode() {
    print_string_color("Error: Invalid opcode\n",0x00FF0000);
    print_string("Registers at time of exception:\n");
    printRegisters();
}

static void int_to_hex_char(uint64_t value, uint8_t * buffer) {
    const uint8_t hex_digits[] = "0123456789ABCDEF";
    int idx = 0;

    buffer[idx++] = '0';
    buffer[idx++] = 'x';

    for(int i = 15; i >= 0; i--) {
        buffer[idx++] = hex_digits[(value >> (i * 4)) & 0xF];
    }
    buffer[idx++] = '\n';
    buffer[idx] = '\0';    
}

void printRegisters() {
    uint64_t registers[CANT_REGISTERS];
    _get_registers(registers);
    uint8_t buffer[MAX_HEXA_LENGTH];

    const uint8_t* reg_names[] = {
        "RAX", "RBX", "RCX", "RDX", "RBP", "RSP", 
        "RSI", "RDI", "R8 ", "R9 ", "R10", "R11", 
        "R12", "R13", "R14", "R15", "RIP"
    };

    for(int i = 0; i < CANT_REGISTERS; i++) {
        print_string(reg_names[i]);
        print_string(": ");
        int_to_hex_char(registers[i], buffer);
        print_string(buffer);
    }
}