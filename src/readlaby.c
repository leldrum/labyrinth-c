#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "creation.h"


Labyrinth *read_file(char *name){
    char filename[256];
    char buffer[256];
    char height[5];
    char width[5];
    char **grille;


    snprintf(filename, sizeof(filename), "../listes/%s.cfg", name);

    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Erreur le fichier recherché n'est pas retrouvé\n");
        return NULL;
    }

    Labyrinth *laby = malloc(sizeof(*laby));
    if (laby == NULL) {
        fclose(file);
        return NULL;
    }

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        laby->name = malloc(strlen(buffer) + 1);
        if (laby->name == NULL) {
            free(laby);
            fclose(file);
            return NULL;
        }

        strcpy(laby->name, buffer);
        printf("%s\n", laby->name);
    } else {
        free(laby);
        fclose(file);
        return NULL;
    }

    if (fgets(height, sizeof(height), file) != NULL) {
        laby->height = atoi(height);
        printf("%d\n", laby->height);
    } else {
        free(laby);
        fclose(file);
        return NULL;
    }


    if (fgets(width, sizeof(width), file) != NULL) {
        laby->width = atoi(width);
        printf("%d\n", laby->width);
    } else {
        free(laby);
        fclose(file);
        return NULL;
    }

    int cara;
    int h = 0;
    int w = 0;
    laby->grille = malloc(laby->height * laby->width * sizeof(int));
    while ((cara = fgetc(file)) != EOF) {

        if(cara != '\n'){
            laby->grille[h][w] = cara;
            w++;
        }
        w = 0;
        h++;

    }


    fclose(file);

    return laby;
}


int main(){

    Labyrinth *laby = read_file("pop");
    if(laby != NULL){
        //printf("%s\n", laby->name);
        free(laby->name);
        free(laby);
    }

    return 1;
}
