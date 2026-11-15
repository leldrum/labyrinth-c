#ifndef CREATION_H
#define CREATION_H

#include <stdbool.h>

#include "maze.h"

/**
 * @struct MoveDirection
 * @brief gère les coordonnées y/x avec une direction
 */
typedef struct {
    int x;
    int y;
    Direction direction;
} MoveDirection;


/**
 * @brief Initialise la grille du labyrinth
 * @param width largeur du labyrinth
 * @param height hauteur du labyrinth
 *
 * @return la grille avec la situation initiale pour la générer ensuite 
 */
int **init_maze(int width, int height);

/**
 * @brief Retirer le labyrinth de la mémoire
 * @param grid grille du labyrinth
 * @param width largeur du labyrinth
 *
 */
void free_maze(int **grid, int width);

/**
 * @brief Essaye de faire fusionner deux régions d'un labyrinth
 * @param maze le pointeur du labyrinth
 * @param y coordonnée y de la cellule
 * @param x coordonnée x de la cellule
 * @param direction la direction de la cellule (4 possibilitées)
 *
 * @return retourne un boolean si fusion est OK ou KO
 */
bool fusion(Maze *maze, int y, int x, Direction direction);

/**
 * @brief Creation du labyrinth avec sa hauteur et sa largeur
 * @param width largeur du labyrinth
 * @param height hauteur du labyrinth
 *
 * @return le pointeur du labyrinth
 */
Maze *create_maze(int width, int height);

/**
 * @brief Enlever des murs pour créer un labyrinth qui sera plus difficile 
 * @param maze pointeur du labyrinth
 *
 */
void difficult_maze(Maze *maze);

#endif
