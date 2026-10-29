#ifndef CREATION_H
#define CREATION_H

#include <stdbool.h>

#define WALL 0
#define PLAYER 2
#define EXIT 3
#define WAY -1

typedef enum {
    left = 1, right = 2, up = 3, down = 4
} Direction;

typedef struct {
    int height;
    int width;
    int **grille;
    char *name;
} Maze;

typedef struct {
    int x;
    int y;
    Direction direction;
} MoveDirection;

void display(Maze *maze);
void changeValues(Maze *maze, int currentValue, int oldValue);
bool hasMultipleNonZeroValues(Maze *maze);
bool fusion(Maze *maze, int y, int x, Direction direction);
void entreeSortie(Maze *maze, int h, int w);
bool estSurContour(const Maze *maze, int y, int x);
int randomNumber(int num);
int *allocate_line(int dimension, int val);
int **initialisationTableau(int height, int width);
void placementValue(Maze *maze);
void free_maze(int **grille, int height);
Maze *creationMaze(int height, int width);
#endif
