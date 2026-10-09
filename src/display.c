#include <stdio.h>

#include "creation.h"

char symbole_labyrinth(int val){
    switch (val){
    case WALL:
        return '#';
        break;
    case PLAYER:
        return 'O';
        break;
    case EXIT:
        return '-';
        break;
    default:
        return ' ';
        break;
    }
}

int num_val_labyrinth(char sym){
    switch (sym)
    {
    case '#':
        return WALL;
        break;
    case 'O':
        return PLAYER;
        break;
    case '-':
        return EXIT;
        break;
    default:
        return WAY;
        break;
    }
}