#include <stdint.h>

#ifndef PONGIS_H
#define PONGIS_H


#define P1_ID 0
#define P2_ID 1
#define BALL_ID 2
#define HOLE_ID 3
#define CANT_FIELDS 5
#define POINTS_TEXT 10
#define TOTAL_TEXT (POINTS_TEXT + 1 + 1) // como hay 9 niveles, los puntos son un solo char y otro para el \0
#define CANT_LEVELS 9
#define CANT_OBJECTS 4
#define EDGE_OFFSET 10
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 24
#define WELCOME_TEXT "Welcome to Pongis Golf!"
#define WELCOME_TEXT_LENGTH 23
#define P1_WIN_TEXT "Player 1 wins! Congratulations!"
#define P2_WIN_TEXT "Player 2 wins! Congratulations!"
#define WIN_TEXT_LENGTH 32
#define EXIT_TEXT "Press [SPACE] to exit"
#define EXIT_TEXT_LENGTH 21
#define CANT_NOTAS_MARIO 48
#define CANT_VICTORY_NOTAS 11

void pongis_main(uint64_t argc, uint8_t* argv[]);

#endif //PONGIS_H
