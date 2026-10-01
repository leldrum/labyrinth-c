#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int randomNumber(int num){
    srand(time(NULL));
    int nb = rand() % num + 1;
    return nb;
}

void creationLabyrinth(int hauteur, int longueur){

    int numberLine = hauteur / 2 + longueur / 2;
    int **laby = (int)malloc(hauteur * longueur * sizeof(int));
    int verifEnd = 0;

    while (verifEnd != 1){
        int valTemp = randomNumber(numberLine);
        int direction = randomNumber(4);

        int nombreMur;

        for (int i = 0; i < hauteur; i++){
            for (int j = 0; j < longueur; j++){
                if(laby[i][j] != nombreMur && laby[i][j] != 0){
                    laby[i][j] = valTemp;
                }

            }

        }

}

int * allocate_line(int dimension, int val){
    int *tab = malloc(dimension*sizeof(int));
    for (int i = 0; i < dimension; i++){
        if(i % 2 == 0){
            *tab = val;
        }
        tab++;
    }
    return tab;
}

void initialisationTableau(int **laby, int hauteur, int longueur){
    int val = 1;
    for (int i = 0; i < hauteur; i++){
        if( i % 2 == 0){        
            *laby[i] = allocate_line(longueur, val);
        }
        val++;
    }
}


