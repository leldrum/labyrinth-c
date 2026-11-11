#ifndef SAVEmaze_H
#define SAVEmaze_H

#include <stdbool.h>

#include "maze.h"

void save_maze(Maze *maze);
char **array_maze_name(int *count);
void free_maze_names(char **names, int count);
Maze *read_file_maze(char *name);
bool is_better_score(char *name, int score_game);
void save_file_score(int score, char *name, char *player_name);
void display_file_score(char *name);

#endif