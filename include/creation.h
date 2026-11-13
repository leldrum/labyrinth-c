#ifndef CREATION_H
#define CREATION_H

#include <stdbool.h>

#include "maze.h"

typedef struct {
    int x;
    int y;
    Direction direction;
} MoveDirection;

int *allocate_line(int dimension, int val);
int **initMaze(int height, int width);
void free_maze(int **grille, int height);
void testdisplay(int **maze, int height, int width);
void changeValues(Maze *maze, int currentValue, int nextValue);
bool fusion(Maze *maze, int y, int x, Direction direction);
bool is_wall_outline(Maze *maze, int y, int x);
MoveDirection *EveryPossibilities(Maze *maze, int *length);
Maze *creationMaze(int height, int width);
void difficult_maze(Maze *maze);

#endif
