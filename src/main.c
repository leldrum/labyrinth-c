#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "launcher.h"

/**
 * @brief Lance le jeu du labyrinth
 */
int main(void) {
    srand((int)time(NULL));
    launcher();
    return 0;
}