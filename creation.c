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
            else if (laby->grille[i][j] == ENTER)
            {
                /* code */
            }
            
            else{
                printf("%s ", " ");
            }
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

    laby->grille[between_case->y][between_case->x] = between_case->value;


    changeValues(laby, currentCase->value, nextCase->value);
    //testDisplay(laby);
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
    return rand() % num + 1;
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

    int processedCoordinates = 0;
    int attemptsWithoutFusion = 0;
    int cellCount = ((height - 1) / 2) * ((width - 1) / 2);
    bool *usedCoordinates = calloc(height *width, sizeof(*usedCoordinates));

    if (usedCoordinates == NULL){
        free_laby(grille, height);
        free(grille);
        free(laby);
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    while (hasMultipleNonZeroValues(laby)
           && attemptsWithoutFusion < cellCount * cellCount){

        int random_direction = randomNumber(2) - 1;
        int random_cord[2] = {randomNumber(height-1), randomNumber(width-1)};
        
        if(random_cord[0] % 2 == 0 || random_cord[1] % 2 == 0){
            continue;
        }

        size_t coordinateIndex = (size_t)random_cord[0] * (size_t)width
                               + (size_t)random_cord[1];
        if (usedCoordinates[coordinateIndex]){
            continue;
        }



        Case *currentCase = at_Case(laby, random_cord[0], random_cord[1]);
        Case *nextCase = NULL;

        if(!currentCase->is_verif){
            currentCase->direction = random_direction;
            switch (random_direction){
            //si vers la droite
            case 0:
                if(currentCase->x + 2 >= width){
                    break;
                }
                nextCase = at_Case(laby, random_cord[0], random_cord[1]+2);
                break;
            //si vers le bas
            case 1:
                if(currentCase->y + 2 >= height){
                    break;
                }
                nextCase = at_Case(laby, random_cord[0]+2, random_cord[1]);
                break;
            default:
                break;
            }
        }else{
            continue;
        }

        if(nextCase != NULL && !nextCase->is_verif){
            fusion(laby, currentCase, nextCase);
            usedCoordinates[coordinateIndex] = true;
            processedCoordinates++;
            attemptsWithoutFusion = 0;
        }
        else{
            attemptsWithoutFusion++;
            continue;
        }
    }

    free(usedCoordinates);
    entreeSortie(laby, laby->height, laby->width);

    return laby;
}




int main(){
    srand((int)time(NULL));

    Labyrinth *laby = creationLabyrinth(11,11);
 
    if(laby != NULL){
        display(laby);
        free_laby(laby->grille, laby->height);
        free(laby);
    }
    return 0;
}
