#ifndef MANAGEMAZE_H
#define MANAGEMAZE_H

#include <stdbool.h>

#include "maze.h"

/**
 * @brief Sauvegarde le labyrinth créé dans un fichier
 * @param maze labyrinth
 */
void save_maze(Maze *maze);

/**
 * @brief Renvoie la liste des noms des labyrinths déjà existant
 * @param [in, out] count le nombre de labyrinth dans le projet 
 * @param is_difficult si le niveau est difficile ou non
 *
 * @return la liste des labyrinths
 */
char **array_maze_name(int *count, int *is_difficult);

/**
 * @brief Vider la liste des labyrinths
 * @param names la liste des noms
 * @param count le nombre de noms
 */
void free_maze_names(char **names, int count);

/**
 * @brief Charger un labyrinth
 * @param name le nom du labyrinth a chargé
 *
 * @return le labyrinth
 */
Maze *read_file_maze(char *name);

/**
 * @brief Savoir si le score du joueur est assez bon pour l'enregistrer
 * @param name nom du labyrinth
 * @param score_game score du joueur
 *
 * @return un boolean valisant ou non la sauvegarde du score
 */
bool is_better_score(char *name, int score_game);

/**
 * @brief Enregistrer le score du joueur dans le fichier
 * @param score score du joueur 
 * @param name nom du labyrinth
 * @param player_name nom du joueur
 */
void save_file_score(int score, char *name, const char *player_name);

/**
 * @brief afficher les scores du labyrinth chargé
 * @param name nom du labyrinth
 */
void display_file_score(char *name);


#endif