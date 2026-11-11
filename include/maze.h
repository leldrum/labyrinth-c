#ifndef MAZE_H
#define MAZE_H

typedef enum {
    CELL_WAY = -1,
    CELL_WALL = 0,
    CELL_PLAYER = 2,
    CELL_EXIT = 3
} Cell;

typedef enum {
    DIR_LEFT = 1,
    DIR_RIGHT = 2,
    DIR_UP = 3,
    DIR_DOWN = 4
} Direction;

typedef enum{
    Q = 'q',
    D = 'd',
    Z = 'z',
    S = 's' 
}KEY_DIRECTION;

typedef struct {
    int height;
    int width;
    int **grille;
    char *name;
} Maze;

typedef enum{
    KEY = 5,
    BONUS = 6,
    PENALTY = 7
};


#endif
