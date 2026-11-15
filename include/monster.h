#ifndef MONSTER_H
#define MONSTER_H

#include "maze.h"

/**
 * @brief Le type de penalité attribuée en fonction du monstre
 * @param monster_type type du monstre (ogre ou fantome)
 *
 * @return la penalité
 */
int monster_penalty_points(int monster_type);

/**
 * @brief Fait bouger les monstres et donne les penalités si joueur rencontré
 * @param maze labyrinth
 *
 * @return la penalité attribuée au joueur
 */
int monster_direction(Maze *maze);

#endif
