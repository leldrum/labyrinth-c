#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "minunit.h"
#include "creation.h"
#include "game.h"

/* Vérifie que la partie n'est pas initialisé */
MU_TEST(test_rejects_start_game){
    int cells[2][2] = 
        {{CELL_WALL, CELL_WALL},
        {CELL_WALL, CELL_WALL}};

    int *grid[2] = {cells[0], cells[1]};

    Maze maze;
    maze.height = 2;
    maze.width = 2;
    maze.grid = grid;

    start_game(&maze);

    mu_assert_int_eq(CELL_WALL, maze.grid[1][0]); //doit correspondre à la coordonnée du joueur si start game fonctionne
    mu_assert_int_eq(CELL_WALL, maze.grid[1][1]);
}

MU_TEST_SUITE(game_suite){
    MU_RUN_TEST(test_rejects_start_game);
}
