#include "minunit.h"
#include "launcher.h"

/* Vérifie qu'une frappe invalide ne crée pas de labyrinthe. */
MU_TEST(test_generate_maze_rejects_invalid_input){
    clearerr(stdin);
    mu_assert(ungetc('x', stdin) == 'x', "impossible pour l'entrée invalide");

    Maze *maze = generate_maze();
    mu_assert(maze == NULL, "frappe invalide a créé un labyrinthe");
}

/* Vérifie que le lanceur gère l'absence de labyrinthe */
MU_TEST(test_launch_without_maze){
    launch_maze(NULL);
}

MU_TEST_SUITE(launcher_suite){
    MU_RUN_TEST(test_generate_maze_rejects_invalid_input);
    MU_RUN_TEST(test_launch_without_maze);
}
