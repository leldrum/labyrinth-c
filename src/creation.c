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
        if(i % 2 == 1 && val != WALL){
            tab[i] = val;
            val++;
        }
        else{
            tab[i] = WALL;
        }
    }
   
    return tab;
}

int **initMaze(int height, int width){

    int **maze = (int **)malloc(height * sizeof(*maze));
    int valNum = 1;
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
            maze[i] = allocate_line(width, WALL);
        }
    }
    return maze;
}

void free_maze(int **grille, int height){
    for (int i = 0; i < height; i++){
        free(grille[i]);
    }
}

void display(Maze *maze){
    for (int i = 0; i < maze->height; i++){
        for (int j = 0; j < maze->width; j++){
            if(maze->grille[i][j] == WALL){
                printf("%s ", "#");
            }
            else if (maze->grille[i][j] == PLAYER)
            {
                printf("%s ", "0");
            }
            else if (maze->grille[i][j] == EXIT)
            {
                printf("%s ", "-");
            }
            
            else{
                printf("%s ", " ");
            }
        }
        printf("\n");
    }
}


void testdisplay(int **maze, int height, int width){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if(maze[i][j] == WALL){
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
        y <= 0 || y >= maze->height-1 || 
        x <= 0 || x >= maze->width-1 ||
        y % 2 == 0 || x % 2 == 0){
        return false;
    }

    int nextY = y;
    int nextX = x;

    switch (direction){
    case left:
        nextX -= 2;
        break;
    case right:
        nextX += 2;
        break;
    case up:
        nextY -= 2;
        break;
    case down:
        nextY += 2;
        break;
    default:
        return false;
    }

    if (nextY <= 0 || nextY >= maze->height - 1 ||
        nextX <= 0 || nextX >= maze->width - 1){
        return false;
    }

    int currentValue = maze->grille[y][x];
    int nextValue = maze->grille[nextY][nextX];
    /*if (currentValue <= WALL || nextValue <= WALL ||
        currentValue == nextValue){
        return false;
    }*/

    int wallY = (y + nextY) / 2;
    int wallX = (x + nextX) / 2;
    // si pas de mur entre les 2, on retourne false
    if (maze->grille[wallY][wallX] != WALL){
        return false;
    }

    maze->grille[wallY][wallX] = WAY;
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


MoveDirection *EveryPossibilities(Maze *maze, int length){
    Direction direction[] = {left, right, up, down};
    int count;
    MoveDirection *possibilties;
    
    for (int i = 1; i < maze->width; i+=2){
        for (int j = 0; j < maze->height; j+=2){
            for (int d = 0; d < 4; d++){
                int nextI = i;
                int nextJ = j;
                switch (direction[d]){
                    case left:
                        nextI -= 2;
                        break;
                    case right:
                        nextI += 2;
                        break;
                    case up:
                        nextJ += 2;
                        break;
                    case down:
                        nextJ -= 2;
                        break;
                    default:
                        break;
                }

                if(!is_wall_outline(maze, nextJ, nextI)){
                    possibilties[count++] = (MoveDirection){i, j , direction[d]};
                    length++;
                }
            }
        }
    }

    return possibilties;
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

    MoveDirection *possibilities = malloc(cell_fusion_number * 2 * sizeof(*possibilities));
    int lenghtPossibilities = 0;    

    possibilities = EveryPossibilities(maze, &lenghtPossibilities);

    while(fusion_necessary != cell_fusion_number){

        int random_val = rand() % lenghtPossibilities - 1;
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

    return maze;
}


int main(void){

    int **tab = initMaze(7,7);
    testdisplay(tab, 7,7);

    return 0;
}
