#ifndef DISPLAY_H
#define DISPLAY_H

#include "maze.h"

/**
 * @brief Recupere les valeurs char issu du fichier labyrinth
 * @param cell integer compris par la grille du labyrinth
 *
 * @return le caractère du symbole
 */
char maze_symbol(int cell);

/**
 * @brief Recupere les valeurs integer compris par la grille du labyrinth
 * @param symbol caractère issu du fichier 
 *
 * @return valeur integer du symbole correspondant au char
 */
int maze_cell_from_symbol(char symbol);

/**
 * @brief affiche la grille du labyrinth
 * @param maze pointeur du labyrinth
 */
void display(Maze *maze);

#endif