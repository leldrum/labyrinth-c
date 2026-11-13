#include <stdlib.h>

#include "creation.h"
#include "minunit.h"

static const char *test_allocate_line_and_init_maze(void){
    int *line = allocate_line(5, 10);
    MU_ASSERT("allocate_line: allocation échouée", line != NULL);
    MU_ASSERT("allocate_line: murs ou valeurs incorrects",
              line[0] == CELL_WALL && line[1] == 10 &&
              line[2] == CELL_WALL && line[3] == 11 &&
              line[4] == CELL_WALL);
    free(line);

    int **grid = initMaze(3, 5);
    MU_ASSERT("initMaze: allocation échouée", grid != NULL);
    MU_ASSERT("initMaze: bordures incorrectes",
              grid[0][0] == CELL_WALL &&
              grid[1][0] == CELL_WALL &&
              grid[1][1] == 100 &&
              grid[1][3] == 101 &&
              grid[2][4] == CELL_WALL);
    free_maze(grid, 3);
    return NULL;
}

static const char *test_fusion_connects_adjacent_regions(void){
    int cells[5][5];
    int *rows[5];
    for (int row = 0; row < 5; row++){
        rows[row] = cells[row];
        for (int column = 0; column < 5; column++){
            cells[row][column] = CELL_WALL;
        }
    }

    cells[1][1] = 100;
    cells[1][3] = 101;
    Maze maze = {.height = 5, .width = 5, .grille = rows};

    MU_ASSERT("fusion: liaison valide refusée",
              fusion(&maze, 1, 1, DIR_RIGHT));
    MU_ASSERT("fusion: le mur entre les régions n'a pas été ouvert",
              cells[1][2] == CELL_WAY);
    MU_ASSERT("fusion: les régions n'ont pas été réunies",
              cells[1][1] == 100 && cells[1][3] == 100);
    MU_ASSERT("fusion: une position de bordure devrait être refusée",
              !fusion(&maze, 0, 1, DIR_RIGHT));
    return NULL;
}

static const char *test_maze_creation_dimensions(void){
    MU_ASSERT("creationMaze: une largeur paire devrait être refusée",
              creationMaze(5, 4) == NULL);

    srand(1);
    Maze *maze = creationMaze(5, 5);
    MU_ASSERT("creationMaze: dimensions valides refusées", maze != NULL);
    MU_ASSERT("creationMaze: dimensions incorrectes",
              maze->height == 5 && maze->width == 5);
    MU_ASSERT("creationMaze: entrée ou sortie absente",
              maze->grille[0][1] == CELL_PLAYER &&
              maze->grille[4][3] == CELL_EXIT);

    free_maze(maze->grille, maze->height);
    free(maze);
    return NULL;
}

const char *run_creation_tests(void){
    MU_RUN_TEST(test_allocate_line_and_init_maze);
    MU_RUN_TEST(test_fusion_connects_adjacent_regions);
    MU_RUN_TEST(test_maze_creation_dimensions);
    return NULL;
}
