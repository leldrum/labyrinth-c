#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define WAY 0
#define WALL 0
#define ENTER 2
#define EXIT 3

typedef struct {
    int height;
    int width;
    int **grille;
    char *name;
} Labyrinth;


typedef struct{
    bool is_verif;
    int value;
    int x;
    int y;
    int direction;
} Case;

typedef struct {

} Wall;


Case *at_Case(Labyrinth *laby, int y, int x){
    if(y >= 0 && y < laby->height && x >= 0 && x < laby->width){
        Case *currentCase;
        currentCase->is_verif;
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
            else{
                printf("%s ", " ");
            }
        }
        printf("\n");
    }
}


void testDisplay(Labyrinth *laby){
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; j < laby->width; j++){
            printf("%d ", laby->grille[i][j]);
        }
        printf("\n");
    }
}

void display_vector(int * vector, int dimension){
    for(int i = 0; i < dimension; i++){
        printf("%d ", *vector);
        vector++;
    }
    printf("\n");
}

void display_matrix(int ** matrix, int lines, int columns){
    for(int i = 0; i < columns; i++){
        display_vector(matrix[i], lines);
    }
}
void changeValues(Labyrinth *laby, int currentValue, int oldValue){
    Case *new;
    for (int i = 0; i < laby->height; i++){
        for (int j = 0; i < laby->width; j++){
            new = at_Case(laby, i,j);
            if(new->value == oldValue){
                new->value = currentValue;
            }
        }
    }
    
}


void fusion(Labyrinth *laby, Case *currentCase, Case *nextCase){

    Case *between_case;
    currentCase->is_verif = true;
    if(currentCase->direction == 0){
        between_case = at_Case(laby, currentCase->y, currentCase->x + 1);
    }
    else if (currentCase->direction == 1){
        between_case = at_Case(laby, currentCase->y+1, currentCase->x);
    }
    else{
    }

    between_case->is_verif = true;
    between_case->value = currentCase->value;

    changeValues(laby, currentCase->value, nextCase->value);
}

void entreeSortie(Labyrinth *laby, int h, int w){
    laby->grille[0][1] = ENTER;
    laby->grille[h-1][w-1] = EXIT;
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
    srand(time(NULL));
    int nb = rand() % num;
    return nb;
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


Labyrinth *creationLabyrinth(int height, int width){

    if(height <= 0 || width <= 0){
        printf("Les dimensions du labyrinthe doivent être positives.\n");
        return NULL;
    }

    int **grille = initialisationTableau(height, width);

    if(grille == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    Labyrinth *laby = malloc(sizeof(*laby));
    if(laby == NULL){
        free_laby(grille, height);
        free(grille);
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    laby->height = height;
    laby->width = width;
    laby->grille = grille;

    placementValue(laby);

    
    murInchangeable(laby);


    int nbMurEnleverMax = (height - 1) * (width - 1);
    int random_direction = randomNumber(2);
    int random_cord[2] = {randomNumber(height), randomNumber(width)}; 


   /* while (nbMurEnleverMax != 0){
        Case *currentCase = at_Case(laby, random_cord[0], random_cord[1]);
        Case *nextCase;

        if(!currentCase->is_verif){
            switch (random_direction){
            //si vers la droite
            case 0:
                nextCase = at_Case(laby, random_cord[0], random_cord[1]+2);
                nextCase->direction = 0;
                break;
            //si vers le bas
            case 1:
                nextCase = at_Case(laby, random_cord[0]+2, random_cord[1]);
                nextCase->direction = 1;
                break;
            default:
                break;
            }
        }
        else{
            break;
        }

        if(!nextCase->is_verif){
            fusion(laby, currentCase, nextCase);
            nbMurEnleverMax--;
        }
        else{
            continue;
        }
    }*/

    return laby;
}




int main(){
    Labyrinth *laby = creationLabyrinth(7,7);
 
    if(laby != NULL){
        testDisplay(laby);
        free_laby(laby->grille, laby->height);
        free(laby);
    }
    return 0;
}



