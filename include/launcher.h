#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "maze.h"

/**
 * @brief Lance le jeu du labyrinth 
 */
void launcher(void);

/**
 * @brief fait la génération du labyrinth
 *
 * @return le labyrinth chargé
 */
Maze *generate_maze(void);

/**
 * @brief Lancer la partie du labyrinth si un nom est chargé
 * @param maze labyrinth
 */
void launch_maze(Maze *maze);

#endif 
