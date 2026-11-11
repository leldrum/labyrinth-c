#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "creation.h"
#include "display.h"

/* Création de la matrice */

int * allocate_line(int dimension, int val){
    int *tab = malloc(dimension * sizeof(*tab));

    if(tab == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    for (int i = 0; i < dimension; i++){
        if(i % 2 == 1 && val != CELL_WALL){
            tab[i] = val;
            val++;
        }
        else{
            tab[i] = CELL_WALL;
        }
    }
   
    return tab;
}

int **initMaze(int height, int width){

    int **maze = (int **)malloc(height * sizeof(*maze));
    int valNum = CELL_EXIT + 1;
    if(maze == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }
    for (int i = 0; i < height; i++){
        if(i % 2 == 1){
            maze[i] = allocate_line(width, valNum);
            valNum += width/2;
        }
        else{
            maze[i] = allocate_line(width, CELL_WALL);
        }
    }
    return maze;
}

void free_maze(int **grille, int height){
    if (grille == NULL){
        return;
    }
    for (int i = 0; i < height; i++){
        free(grille[i]);
    }
    free(grille);
}

void testdisplay(int **maze, int height, int width){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if(maze[i][j] == CELL_WALL){
                printf("%s ", "#");
            }
            else{
                printf("%d ", maze[i][j]);
            }
        }
        printf("\n");
    }
}


void changeValues(Maze *maze, int currentValue, int nextValue){
    for (int i = 0; i < maze->height; i++){
        for (int j = 0; j < maze->width; j++){
            if(maze->grille[i][j] == nextValue ){
                maze->grille[i][j] = currentValue;
            }
        }
    }
}



bool fusion(Maze *maze, int y, int x, Direction direction){
    if (maze == NULL || maze->grille == NULL || 
        is_wall_outline(maze, y, x) ||
        y % 2 == 0 || x % 2 == 0){
        return false;
    }

    int nextY = y;
    int nextX = x;

    switch (direction){
    case DIR_LEFT:
        nextX -= 2;
        break;
    case DIR_RIGHT:
        nextX += 2;
        break;
    case DIR_UP:
        nextY -= 2;
        break;
    case DIR_DOWN:
        nextY += 2;
        break;
    default:
        return false;
    }

    if (is_wall_outline(maze, nextY, nextX)){
        return false;
    }

    int currentValue = maze->grille[y][x];
    int nextValue = maze->grille[nextY][nextX];
    if (currentValue <= CELL_WALL || nextValue <= CELL_WALL ||
        currentValue == nextValue){
        return false;
    }

    int wallY = (y + nextY) / 2;
    int wallX = (x + nextX) / 2;
    // si pas de mur entre les 2, on retourne false
    if (maze->grille[wallY][wallX] != CELL_WALL){
        return false;
    }

    maze->grille[wallY][wallX] = CELL_WAY;
    changeValues(maze, currentValue, nextValue);
    return true;
}



bool is_wall_outline(Maze *maze, int y, int x){
    if (maze == NULL || maze->grille == NULL ||
        y < 0 || y >= maze->height || x < 0 || x >= maze->width){
        return true;
    }

    return y == 0 || y == maze->height - 1 ||
           x == 0 || x == maze->width - 1;
}


MoveDirection *EveryPossibilities(Maze *maze, int *length){
    
    int rows = (maze->height-1) / 2;
    int columns = (maze->width-1) / 2;
    int capacity = 2 * (rows * (columns-1) + columns * (rows-1));
    MoveDirection *possibilities = malloc(capacity * sizeof(*possibilities));
    if (possibilities == NULL){
        return NULL;
    }

    *length = 0;
    for (int x = 1; x < maze->width-1; x += 2){
        for (int y = 1; y < maze->height-1; y += 2){
            if (!is_wall_outline(maze, y, x - 2)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_LEFT};
            }
            if (!is_wall_outline(maze, y, x + 2)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_RIGHT};
            }
            if (!is_wall_outline(maze, y - 2, x)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_UP};
            }
            if (!is_wall_outline(maze, y + 2, x)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_DOWN};
            }
        }
    }

    return possibilities;
}



Maze *creationMaze(int height, int width){

    if(height <= 0 || width <= 0){
        printf("Les dimensions du Labyrinth doivent être positives.\n");
        return NULL;
    }

    if (height % 2 == 0 || width % 2 == 0) {
        printf("Les dimensions doivent être impaires.\n");
        return NULL;
    }

    Maze *maze = malloc(sizeof(*maze));
    if (maze == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    maze->height = height;
    maze->width = width;
    maze->name = NULL;
    maze->grille = initMaze(height, width);

    int cell_fusion_number = ((height-1) / 2) * ((width-1) / 2) - 1;
    int fusion_necessary = 0;

    int lenghtPossibilities = 0;    

    MoveDirection *possibilities = EveryPossibilities(maze, &lenghtPossibilities);
    if (possibilities == NULL){
        free_maze(maze->grille, maze->height);
        free(maze);
        return NULL;
    }

    while(fusion_necessary != cell_fusion_number){

        int random_val = rand() % lenghtPossibilities;
        int x = possibilities[random_val].x;
        int y = possibilities[random_val].y;
        int direction = possibilities[random_val].direction;

        if(fusion(maze, y, x, direction)){
            fusion_necessary++;
        }

        // On remplace le mouvement possible recupéré par le dernier de la liste
        // pour faire la suppression 
        possibilities[random_val] = possibilities[lenghtPossibilities-1];
        lenghtPossibilities--;
    }

    maze->grille[0][1] = CELL_PLAYER;
    maze->grille[height-1][width-2] = CELL_EXIT;

    free(possibilities);
    return maze;
}
