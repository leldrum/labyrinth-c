#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "maze.h"

void launcher(void);
Maze *generate_maze(void);
void same_maze_name(char *nom);
void choose_maze_name(char *nom);
void launch_maze(Maze *maze);

#endif 
