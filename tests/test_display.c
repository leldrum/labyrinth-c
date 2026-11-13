#include "display.h"
#include "minunit.h"

static const char *test_maze_symbol_conversions(void){
    MU_ASSERT("symbole_Maze: mur incorrect", symbole_Maze(CELL_WALL) == '#');
    MU_ASSERT("symbole_Maze: joueur incorrect", symbole_Maze(CELL_PLAYER) == 'O');
    MU_ASSERT("symbole_Maze: sortie incorrecte", symbole_Maze(CELL_EXIT) == '-');
    MU_ASSERT("symbole_Maze: case libre incorrecte", symbole_Maze(CELL_WAY) == ' ');

    MU_ASSERT("num_val_Maze: mur incorrect", num_val_Maze('#') == CELL_WALL);
    MU_ASSERT("num_val_Maze: joueur incorrect", num_val_Maze('O') == CELL_PLAYER);
    MU_ASSERT("num_val_Maze: sortie incorrecte", num_val_Maze('-') == CELL_EXIT);
    MU_ASSERT("num_val_Maze: case libre incorrecte", num_val_Maze(' ') == CELL_WAY);
    return NULL;
}

const char *run_display_tests(void){
    MU_RUN_TEST(test_maze_symbol_conversions);
    return NULL;
}
