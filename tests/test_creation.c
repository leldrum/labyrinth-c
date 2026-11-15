#include "minunit.h"
#include <stdlib.h>
#include "creation.h"


/* Vérifie l'initialisation d'une grille correcte */
MU_TEST(test_init_maze){
    int **grid = init_maze(5, 3);
    mu_assert(grid != NULL, "allocation échouée");
    mu_assert(grid[0][0] == CELL_WALL, "mur incorrect");
    mu_assert(grid[4][2] == CELL_WALL, "mur incorrect");
    free_maze(grid, 5);
}

/* Vérifie la fusion de régions */
MU_TEST(test_fusion){
    int cells[5][5];
    int *rows[5];
    for (int row = 0; row < 5; row++){
        rows[row] = cells[row];
        for (int column = 0; column < 5; column++){
            cells[row][column] = CELL_WALL;
        }
    }

    cells[1][1] = 20;
    cells[3][1] = 21;
    Maze maze;
    maze.height = 5;
    maze.width = 5;
    maze.grid = rows;

    mu_assert(fusion(&maze, 1, 1, DIR_RIGHT), "liaison valide refusée");
    mu_assert(cells[2][1] == CELL_WAY, "le mur pas été ouvert");
    mu_assert_int_eq(20, cells[1][1]);
    mu_assert_int_eq(20, cells[3][1]);
    mu_assert(!fusion(&maze, 0, 1, DIR_RIGHT), "fusion refusée dans le mur");
}

/* Vérifie les dimensions acceptées et les entrées/sorties */
MU_TEST(test_maze_creation_dimensions){
    mu_assert(create_maze(4, 5) == NULL, "largeur paire refusée");

    Maze *maze = create_maze(7, 5);
    mu_assert(maze != NULL, "dimensions valides refusées");
    mu_assert(maze->height == 5 && maze->width == 7, "dimensions incorrectes");
    mu_assert_int_eq(CELL_PLAYER, maze->grid[1][0]);
    mu_assert_int_eq(CELL_EXIT, maze->grid[5][4]);

    free_maze(maze->grid, maze->width);
    free(maze);
}

MU_TEST_SUITE(creation_suite){
    MU_RUN_TEST(test_init_maze);
    MU_RUN_TEST(test_fusion);
    MU_RUN_TEST(test_maze_creation_dimensions);
}
