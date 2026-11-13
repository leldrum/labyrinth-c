#include <stdio.h>

#include "minunit.h"

int tests_run = 0;

const char *run_creation_tests(void);
const char *run_display_tests(void);
const char *run_monster_tests(void);
const char *run_savelaby_tests(void);

int main(void){
    const char *failure;

    failure = run_creation_tests();
    if (failure != NULL) return 1;
    failure = run_display_tests();
    if (failure != NULL) return 1;
    failure = run_monster_tests();
    if (failure != NULL) return 1;
    failure = run_savelaby_tests();
    if (failure != NULL) return 1;

    printf("OK: %d tests MinUnit réussis.\n", tests_run);
    return 0;
}
