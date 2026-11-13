#include "monster.h"
#include "minunit.h"

static const char *test_ogre_moves_onto_bonus_and_leaves_it_behind(void){
    int cells[3][7] = {
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, BONUS, CELL_WAY, CELL_WAY, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2]};
    Maze maze = {.height = 3, .width = 7, .grille = rows};

    MU_ASSERT("ogre: pénalité inattendue en avançant sur le bonus",
              monster_direction(&maze) == 0);
    MU_ASSERT("ogre: le bonus n'a pas été laissé derrière",
              cells[1][1] == BONUS);
    MU_ASSERT("ogre: le déplacement attendu n'a pas eu lieu",
              cells[1][2] == OGRE);
    return NULL;
}

static const char *test_ogre_moves_onto_penalty_and_leaves_it_behind(void){
    int cells[3][7] = {
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, PENALTY, CELL_WAY, CELL_WAY, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2]};
    Maze maze = {.height = 3, .width = 7, .grille = rows};

    MU_ASSERT("ogre: pénalité inattendue en avançant sur la pénalité",
              monster_direction(&maze) == 0);
    MU_ASSERT("ogre: la pénalité n'a pas été laissée derrière",
              cells[1][1] == PENALTY);
    MU_ASSERT("ogre: le déplacement attendu n'a pas eu lieu",
              cells[1][2] == OGRE);
    return NULL;
}

static const char *test_ogre_penalty_when_reaching_player(void){
    int cells[3][5] = {
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, CELL_PLAYER, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2]};
    Maze maze = {.height = 3, .width = 5, .grille = rows};

    MU_ASSERT("ogre: pénalité incorrecte au contact du joueur",
              monster_direction(&maze) == monster_penalty_points(OGRE));
    MU_ASSERT("ogre: le joueur ou l'ogre a été écrasé",
              cells[1][1] == OGRE && cells[1][2] == CELL_PLAYER);
    return NULL;
}

const char *run_monster_tests(void){
    MU_RUN_TEST(test_ogre_moves_onto_bonus_and_leaves_it_behind);
    MU_RUN_TEST(test_ogre_moves_onto_penalty_and_leaves_it_behind);
    MU_RUN_TEST(test_ogre_penalty_when_reaching_player);
    return NULL;
}
