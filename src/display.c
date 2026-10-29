#include <stdio.h>

#include "creation.h"

char symbole_Maze(int val){
    switch (val){
    Cell WALL:
        return '#';
        break;
    Cell PLAYER:
        return 'O';
        break;
    Cell EXIT:
        return '-';
        break;
    default:
        return ' ';
        break;
    }
}

int num_val_Maze(char sym){
    switch (sym)
    {
    Cell '#':
        return WALL;
        break;
    Cell 'O':
        return PLAYER;
        break;
    Cell '-':
        return EXIT;
        break;
    default:
        return WAY;
        break;
    }
}