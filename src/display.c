#include <stdio.h>

#include "display.h"

void display(Maze *maze){
    for (int i = 0; i < maze->height; i++){
        for (int j = 0; j < maze->width; j++){
            if (maze->grille[i][j] == CELL_WALL){
                printf("%s ", "#");
            }
            else if (maze->grille[i][j] == CELL_PLAYER){
                printf("%s ", "0");
            }
            else if (maze->grille[i][j] == CELL_EXIT){
                printf("%s ", "-");
            }
            else if(maze->grille[i][j] == KEY){
                printf("%s ", "K");
            }
            else if(maze->grille[i][j] == BONUS){
                printf("%s ", "B");
            }
            else if(maze->grille[i][j] == PENALTY){
                printf("%s ", "P");
            }
            else{
                printf("%s ", " ");
            }
        }
        printf("\n");
    }
}

char symbole_Maze(int val){
    switch (val){
    case CELL_WALL:
        return '#';
    case CELL_PLAYER:
        return 'O';
    case CELL_EXIT:
        return '-';
    default:
        return ' ';
    }
}

int num_val_Maze(char sym){
    switch (sym)
    {
    case '#':
        return CELL_WALL;
    case 'O':
        return CELL_PLAYER;
    case '-':
        return CELL_EXIT;
    default:
        return CELL_WAY;
    }
}