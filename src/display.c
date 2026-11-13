#include <stdio.h>

#include "display.h"

static void display_cell(int cell);


static void display_cell(int cell){
    const char *symbol = " ";

    switch (cell){
    case CELL_WALL:
        symbol = "#";
        break;
    case CELL_PLAYER:
        symbol = "@";
        break;
    case CELL_EXIT:
        symbol = "E";
        break;
    case KEY:
        symbol = "K";
        break;
    case BONUS:
        symbol = "+";
        break;
    case PENALTY:
        symbol = "!";
        break;
    case OGRE:
        symbol = "O";
        break;
    case GHOST:
        symbol = "F";
        break;
    default:
        break;
    }

    printf("%s ", symbol);
}


void display(Maze *maze){
    for (int i = 0; i < maze->height; i++){
        printf("  ");
        for (int j = 0; j < maze->width; j++){
           display_cell(maze->grille[i][j]);
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