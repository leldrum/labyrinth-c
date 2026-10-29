#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "creation.h"


Maze *read_file(char *name){
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

    Maze *maze = malloc(sizeof(*maze));
    if (maze == NULL) {
        fclose(file);
        return NULL;
    }

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        maze->name = malloc(strlen(buffer) + 1);
        if (maze->name == NULL) {
            free(maze);
            fclose(file);
            return NULL;
        }

        strcpy(maze->name, buffer);
        printf("%s\n", maze->name);
    } else {
        free(maze);
        fclose(file);
        return NULL;
    }

    if (fgets(height, sizeof(height), file) != NULL) {
        maze->height = atoi(height);
        printf("%d\n", maze->height);
    } else {
        free(maze);
        fclose(file);
        return NULL;
    }


    if (fgets(width, sizeof(width), file) != NULL) {
        maze->width = atoi(width);
        printf("%d\n", maze->width);
    } else {
        free(maze);
        fclose(file);
        return NULL;
    }

    int cara;
    int h = 0;
    int w = 0;
    maze->grille = malloc(maze->height * maze->width * sizeof(int));
    while ((cara = fgetc(file)) != EOF) {

        if(cara != '\n'){
            maze->grille[h][w] = cara;
            w++;
        }
        w = 0;
        h++;

    }


    fclose(file);

    return maze;
}


int main(){

    Maze *maze = read_file("pop");
    if(maze != NULL){
        //printf("%s\n", maze->name);
        free(maze->name);
        free(maze);
    }

    return 1;
}
