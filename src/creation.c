#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "creation.h"
#include "display.h"


Case *at_Case(Labyrinth *laby, int y, int x){
    if(y >= 0 && y < laby->height && x >= 0 && x < laby->width){
        Case *currentCase = malloc(sizeof(*currentCase));
        currentCase->is_verif = false;
        currentCase->x = x;
        currentCase->y = y;
        currentCase->value = laby->grille[y][x];
        return currentCase;
    }
    return 0;
}

void display(Labyrinth *laby){
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            if(laby->grille[i][j] == WALL){
                printf("%s ", "#");
            }
            else if (laby->grille[i][j] == PLAYER)
            {
                printf("%s ", "0");
            }
            else if (laby->grille[i][j] == EXIT)
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


void changeValues(Labyrinth *laby, int currentValue, int oldValue){
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            if(laby->grille[i][j] == oldValue ){
                laby->grille[i][j] = currentValue;
            }
        }
    }
}

bool hasMultipleNonZeroValues(Labyrinth *laby){
    int firstValue = 0;

    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            int value = laby->grille[i][j];

            if(firstValue != value && (firstValue != 0 && value != 0)){
                return true;
            }

            if (value != 0){
                firstValue = value;
            }
            
        }
    }

    return false;
}


void fusion(Labyrinth *laby, Case *currentCase, Case *nextCase){

    Case *between_case;
    currentCase->is_verif = true;
    if (currentCase->direction == 0) {
        between_case = at_Case(laby, currentCase->y, currentCase->x + 1);
    }
    else if (currentCase->direction == 1) {
        between_case = at_Case(laby, currentCase->y + 1, currentCase->x);
    }
    else if (currentCase->direction == 2) {
        between_case = at_Case(laby, currentCase->y, currentCase->x - 1);
    }
    else if (currentCase->direction == 3) {
        between_case = at_Case(laby, currentCase->y - 1, currentCase->x);
    }
    else {
        return;
    }

    between_case->is_verif = true;
    between_case->value = currentCase->value;

    laby->grille[between_case->y][between_case->x] = between_case->value;

    changeValues(laby, currentCase->value, nextCase->value);
    //testDisplay(laby);
}

void entreeSortie(Labyrinth *laby, int h, int w){
    laby->grille[0][1] = PLAYER;
    laby->grille[h-1][w-2] = EXIT;
}



void murInchangeable(Labyrinth *laby){
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            if(i == 0 || i == laby->height - 1 || j == 0 || j == laby->width - 1){
                at_Case(laby, i, j)->is_verif = true;
            }
        }
    }
}


int randomNumber(int num){
    return rand() % num;
}

static int randomRange(int min, int max){
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }
    return min + (rand() % (max - min + 1));
}

int * allocate_line(int dimension, int val){
    int *tab = malloc(dimension * sizeof(*tab));
    int *line = tab;

    if(tab == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    for (int i = 0; i < dimension; i++){
        if(i % 2 == 1 && val != 0){
            line[i] = val;
            val++;
        }
        else{
            line[i] = WALL;
        }
    }
   
    return line;
}

int **initialisationTableau(int height, int width){

    int **laby = malloc(height * sizeof(*laby));
    if(laby == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }
    for (int i = 0; i < height; i++){
        laby[i] = allocate_line(width, WALL);
    }
    return laby;
}

void placementValue(Labyrinth *laby){
    int val = 1;
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            if(i % 2 == 1 && j % 2 == 1){
                laby->grille[i][j] = val;
                val++;
            }
        }
    }
}


void free_laby(int **grille, int height){
    for (int i = 0; i < height; i++){
        free(grille[i]);
    }
    
}


static void carveMaze(Labyrinth *laby, bool *visited, int y, int x) {
    visited[(size_t)y * (size_t)laby->width + (size_t)x] = true;
    laby->grille[y][x] = WAY;

    int directions[4][2] = {
        {0, 2},
        {2, 0},
        {0, -2},
        {-2, 0}
    };

    for (int i = 0; i < 4; i++) {
        int j = rand() % (4 - i);
        int tmp = directions[i][0];
        directions[i][0] = directions[j][0];
        directions[j][0] = tmp;

        tmp = directions[i][1];
        directions[i][1] = directions[j][1];
        directions[j][1] = tmp;
    }

    for (int i = 0; i < 4; i++) {
        int nextY = y + directions[i][0];
        int nextX = x + directions[i][1];

        if (nextY <= 0 || nextY >= laby->height - 1 || nextX <= 0 || nextX >= laby->width - 1) {
            continue;
        }

        if (!visited[(size_t)nextY * (size_t)laby->width + (size_t)nextX]) {
            laby->grille[y + directions[i][0] / 2][x + directions[i][1] / 2] = WAY;
            carveMaze(laby, visited, nextY, nextX);
        }
    }
}

Labyrinth *creationLabyrinth(int height, int width){

    if(height <= 0 || width <= 0){
        printf("Les dimensions du labyrinthe doivent être positives.\n");
        return NULL;
    }

    if (height % 2 == 0 || width % 2 == 0) {
        printf("Les dimensions doivent être impaires.\n");
        return NULL;
    }

    Labyrinth *laby = malloc(sizeof(*laby));
    if (laby == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    laby->height = height;
    laby->width = width;
    laby->name = NULL;
    laby->grille = malloc(height * sizeof(*laby->grille));
    if (laby->grille == NULL) {
        free(laby);
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        laby->grille[i] = malloc(width * sizeof(*laby->grille[i]));
        if (laby->grille[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(laby->grille[j]);
            }
            free(laby->grille);
            free(laby);
            printf("Erreur d'allocation mémoire\n");
            return NULL;
        }

        for (int j = 0; j < width; j++) {
            laby->grille[i][j] = WALL;
        }
    }

    bool *visited = calloc((size_t)height * (size_t)width, sizeof(*visited));
    if (visited == NULL) {
        free_laby(laby->grille, height);
        free(laby->grille);
        free(laby);
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    carveMaze(laby, visited, 1, 1);
    free(visited);

    laby->grille[0][1] = PLAYER;
    laby->grille[height - 1][width - 2] = EXIT;

    return laby;
}


