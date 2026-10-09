#ifndef CREATION_H
#define CREATION_H

#include <stdbool.h>

#define WALL 0
#define PLAYER 2
#define EXIT 3
#define WAY -1

typedef struct {
    int height;
    int width;
    int **grille;
    char *name;
} Labyrinth;

typedef struct {
    bool is_verif;
    int value;
    int x;
    int y;
    int direction;
} Case;

Case *at_Case(Labyrinth *laby, int y, int x);
void display(Labyrinth *laby);
void changeValues(Labyrinth *laby, int currentValue, int oldValue);
bool hasMultipleNonZeroValues(Labyrinth *laby);
void fusion(Labyrinth *laby, Case *currentCase, Case *nextCase);
void entreeSortie(Labyrinth *laby, int h, int w);
void murInchangeable(Labyrinth *laby);
int randomNumber(int num);
int *allocate_line(int dimension, int val);
int **initialisationTableau(int height, int width);
void placementValue(Labyrinth *laby);
void free_laby(int **grille, int height);
Labyrinth *creationLabyrinth(int height, int width);
#endif
