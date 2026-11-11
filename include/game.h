#ifndef GAME_H
#define GAME_H

#include "maze.h"

typedef struct{
    int row;
    int column;
}Position;



void start_game(Maze *maze);


#endif 
