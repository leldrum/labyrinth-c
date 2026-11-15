#include "minunit.h"
#include "display.h"


/* Vérifie la conversion des types de cases vers les symboles */
MU_TEST(test_maze_symbol){
    mu_assert(maze_symbol(CELL_WALL) == '#', "symbole du mur");
    mu_assert(maze_symbol(CELL_PLAYER) == 'O', "symbole du joueur");
    mu_assert(maze_symbol(CELL_EXIT) == '-', "symbole de la sortie");
    mu_assert(maze_symbol(CELL_WAY) == ' ', "symbole du chemain");
}

/* Vérifie la conversion des symboles vers les types de cases */
MU_TEST(test_val_maze){
    mu_assert(maze_cell_from_symbol('#') == CELL_WALL, "symbole du mur");
    mu_assert(maze_cell_from_symbol('O') == CELL_PLAYER, "symbole du joueur");
    mu_assert(maze_cell_from_symbol('-') == CELL_EXIT, "symbole de la sortie");
    mu_assert(maze_cell_from_symbol(' ') == CELL_WAY, "symbole du chemain");
}

MU_TEST_SUITE(display_suite){
    MU_RUN_TEST(test_maze_symbol);
    MU_RUN_TEST(test_val_maze);
}
