
#include <stdint.h>

#ifndef STANDARLIB_H
#define STANDARLIB_H


#define SYS_READ 0
#define SYS_WRITE 4
#define STDOUT 1
#define STDERR 2
#define STDIN 0
#define BUFFER_SIZE 256
#define NUM_BUFFER_SIZE 20

extern uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);


uint64_t fd_printf(uint64_t fd, const uint8_t *fmt, ...);
uint64_t printf(const uint8_t *fmt, ...);
uint64_t scanf(const uint8_t *fmt, ...);
void  putc(uint8_t c);
void  puts(uint8_t * c);
uint8_t getc();
uint8_t * gets();
uint64_t strcmp(uint8_t* s1, uint8_t* s2 );


#endif //STANDARLIB_H
