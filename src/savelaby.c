#include <stdio.h>
#include <stdlib.h>

#include "creation.h"
#include "display.h"


void save_laby(Labyrinth *laby){
    char name[256];
    sprintf(name, "../listes/%s.cfg", laby->name);
    FILE *file = fopen(name, "w");
    if(file == NULL){
        printf("Le fichier n'a pas pu être ouvert !\n");
    }

    //print le nom
    fprintf(file, "%s\n", laby->name);

    //print la hauteur et la largeur
    fprintf(file, "%d\n",laby->height);
    fprintf(file, "%d\n", laby->width);




    //print le labyrinth
    for(int i = 0; i < laby->height; i++){
        for(int j = 0; j < laby->width; j++){
            fprintf(file, "%c", symbole_labyrinth(laby->grille[i][j]));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Enregistrement du fichier avec succès !\n");
}