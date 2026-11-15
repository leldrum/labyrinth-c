#include "minunit.h"
#include "test_creation.c"
#include "test_display.c"
#include "test_game.c"
#include "test_monster.c"
#include "test_managemaze.c"
#include "test_launcher.c"

MU_TEST_SUITE(statistics_suite){
    MU_RUN_SUITE(creation_suite);
    MU_RUN_SUITE(display_suite);
    MU_RUN_SUITE(game_suite);
    MU_RUN_SUITE(monster_suite);
    MU_RUN_SUITE(managemaze_suite);
    MU_RUN_SUITE(launcher_suite);
}

int main(void){
    MU_RUN_SUITE(statistics_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
