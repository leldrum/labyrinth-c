#include <stdio.h>

#include "display.h"

/**
 * @brief Defini les symbole à afficher 
 * @param cell valeur intière issu de la grille du labyrinth
 */
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
    for (int y = 0; y < maze->height; y++){
        printf("  ");
        for (int x = 0; x < maze->width; x++){
           display_cell(maze->grid[x][y]);
        }
        printf("\n");
    }
}


char maze_symbol(int cell){
    switch (cell){
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

int maze_cell_from_symbol(char symbol){
    switch (symbol)
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