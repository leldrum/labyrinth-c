#ifndef MAZE_H
#define MAZE_H

/**
 * @enum Cell
 * @brief enumère les éléments initiales d'un labyrinth
 */
typedef enum {
    CELL_WAY = -1,
    CELL_WALL = 0,
    CELL_PLAYER = 2,
    CELL_EXIT = 3
} Cell;

/**
 * @enum Direction
 * @brief enumère les directions pouvant etre pris pour creer le labyrinth
 */
typedef enum {
    DIR_LEFT = 1,
    DIR_RIGHT = 2,
    DIR_UP = 3,
    DIR_DOWN = 4
} Direction;

/**
 * @enum KEY_DIRECTION
 * @brief enumère les directions clavier pour le joueur
 */
typedef enum{
    Q = 'q',
    D = 'd',
    Z = 'z',
    S = 's' 
}KEY_DIRECTION;

/**
 * @struct Maze
 * @brief les différentes élements d'un labyrinth
 */
typedef struct {
    int height;
    int width;
    int **grid;
    char *name;
    int is_difficult;
} Maze;

/**
 * @struct Position
 * @brief gère les coordonnées y/x avec une direction
 */
typedef struct{
    int x;
    int y;
}Position;

/**
 * @enum MazeObject
 * @brief enumère les objets qui améliore le labyrinth
 */
typedef enum {
    KEY = 5,
    BONUS = 6,
    PENALTY = 7,
    GHOST = 8,
    OGRE = 9
} MazeObject;


#endif
