#include <stdio.h>
#include <stdlib.h>

#include "creation.h"
#include "display.h"


void save_maze(Maze *maze){
    if (maze == NULL || maze->name == NULL) {
        printf("Le Maze est invalide pour l'enregistrement.\n");
        return;
    }

    char name[256];
    snprintf(name, sizeof(name), "listes/%s.cfg", maze->name);
    FILE *file = fopen(name, "w");
    if (file == NULL) {
        printf("Le fichier n'a pas pu être ouvert !\n");
        return;
    }

    //print le nom
    fprintf(file, "%s\n", maze->name);

    //print la hauteur et la largeur
    fprintf(file, "%d\n",maze->height);
    fprintf(file, "%d\n", maze->width);




    //print le Maze
    for(int i = 0; i < maze->height; i++){
        for(int j = 0; j < maze->width; j++){
            fprintf(file, "%c", symbole_Maze(maze->grille[i][j]));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Enregistrement du fichier avec succès !\n");
}