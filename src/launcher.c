#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "launcher.h"
#include "creation.h"
#include "savemaze.h"

//enum {create, loading,play, exit};

void launcher(void) {
    printf("Bienvenue dans le Mazee !\n");

    generate();
}





void keep_Maze(Maze **maze, char **name){
    //TODO
}

void generate(void) {
    int largeur;
    int width;
    char nom[50];
    int wantCreate;
    int wantSave;

    printf("Veuillez insérer la largeur de votre Mazee (impaire): ");
    scanf("%d", &largeur);
    printf("Veuillez insérer la width de votre Mazee (impaire): ");
    scanf("%d", &width);

    if (largeur % 2 == 0 || width % 2 == 0) {
        printf("Vous ne respectez pas la condition d'une width/largeur impaire.\n");
        return;
    }

    printf("Veuillez insérer le nom de votre Maze: ");
    scanf("%49s", nom);

    //TODO si le nom existe redemander un nouveau en metta,t la liste des Mazes déjà existants 

    printf("Voulez vous creer votre Maze ? 0 (oui), 1 (non) ");
    scanf("%d", &wantCreate);

    if (wantCreate == 0) {
        printf("Création du Mazee en cours...\n");
        Maze *maze = creationMaze(largeur, width);
        if (maze != NULL) {
            maze->name = malloc(strlen(nom) + 1);
            if (maze->name == NULL) {
                free_maze(maze->grille, maze->height);
                free(maze);
                printf("Erreur d'allocation mémoire pour le nom du Mazee.\n");
                return;
            }
            strcpy(maze->name, nom);
            display(maze);

            printf("Voulez vous enregistrer ce Maze ? 0 (oui), 1 (non) ");
            scanf("%d", &wantSave);
            if (wantSave == 0) {
                save_maze(maze);
            }

            free(maze->name);
            free_maze(maze->grille, maze->height);
            free(maze);
        }
    } else {
        //keep_Maze()
    }
}



