#include "minunit.h"
#include "monster.h"

/* Vérifie que l'ogre traverse un bonus sans l'écraser */
MU_TEST(test_ogre_moves_bonus){
    int cells[7][3] = {
        {CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, CELL_WALL},
        {CELL_WALL, BONUS, CELL_WALL},
        {CELL_WALL, CELL_WAY, CELL_WALL},
        {CELL_WALL, CELL_WAY, CELL_WALL},
        {CELL_WALL, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2], cells[3], cells[4], cells[5], cells[6]};

    Maze maze;
    maze.height = 3;
    maze.width = 7;
    maze.grid = rows;

    mu_assert(monster_direction(&maze) == 0, "problème en avançant sur le bonus");
    mu_assert_int_eq(cells[1][1], BONUS);
    mu_assert_int_eq(cells[2][1], OGRE);
}

/* Vérifie que l'ogre traverse une pénalité sans l'écraser */
MU_TEST(test_ogre_moves_penalty){
    int cells[7][3] = {
        {CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, CELL_WALL},
        {CELL_WALL, PENALTY, CELL_WALL},
        {CELL_WALL, CELL_WAY, CELL_WALL},
        {CELL_WALL, CELL_WAY, CELL_WALL},
        {CELL_WALL, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL}
    };
    
    int *rows[] = {cells[0], cells[1], cells[2], cells[3], cells[4], cells[5], cells[6]};

    Maze maze;
    maze.height = 3;
    maze.width = 7;
    maze.grid = rows;

    mu_assert(monster_direction(&maze) == 0, "problème en avançant sur le bonus");
    mu_assert_int_eq(cells[1][1], PENALTY);
    mu_assert_int_eq(cells[2][1], OGRE);
}

/* Vérifie la pénalité de l'ogre lorsqu'il atteint le joueur */
MU_TEST(test_ogre_penalty_player){
    int cells[5][3] = {
        {CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, OGRE, CELL_WALL},
        {CELL_WALL, CELL_PLAYER, CELL_WALL},
        {CELL_WALL, KEY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2], cells[3], cells[4]};

    Maze maze;
    maze.height = 3;
    maze.width = 5;
    maze.grid = rows;

    mu_assert(monster_direction(&maze) == monster_penalty_points(OGRE), "pénalité incorrecte sur le joueur");
    mu_assert_int_eq(cells[1][1], OGRE);
    mu_assert_int_eq(cells[2][1], CELL_PLAYER);
}

MU_TEST_SUITE(monster_suite){
    MU_RUN_TEST(test_ogre_moves_bonus);
    MU_RUN_TEST(test_ogre_moves_penalty);
    MU_RUN_TEST(test_ogre_penalty_player);
}
