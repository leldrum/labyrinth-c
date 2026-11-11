#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "launcher.h"

int main(void) {
    srand((unsigned int)time(NULL));
    launcher();
    return 0;
}