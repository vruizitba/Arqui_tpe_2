#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#define CANT_REGISTERS 17
#define MAX_HEXA_LENGTH 20


void exceptionDispatcher(uint64_t exception);

void zero_division();

void invalid_opcode();

void printRegisters();

extern void _get_registers(uint64_t * buffer);

#endif //EXCEPTIONS_H