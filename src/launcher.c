#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "launcher.h"
#include "creation.h"
#include "savelaby.h"

//enum {create, loading,play, exit};

void launcher(void) {
    printf("Bienvenue dans le labyrinthe !\n");

    generate();
}





void keep_labyrinth(Labyrinth **laby, char **name){
    //TODO
}

void generate(void) {
    int largeur;
    int width;
    char nom[50];
    int wantCreate;
    int wantSave;

    printf("Veuillez insérer la largeur de votre labyrinthe (impaire): ");
    scanf("%d", &largeur);
    printf("Veuillez insérer la width de votre labyrinthe (impaire): ");
    scanf("%d", &width);

    if (largeur % 2 == 0 || width % 2 == 0) {
        printf("Vous ne respectez pas la condition d'une width/largeur impaire.\n");
        return;
    }

    printf("Veuillez insérer le nom de votre labyrinth: ");
    scanf("%49s", nom);

    //TODO si le nom existe redemander un nouveau en metta,t la liste des labyrinths déjà existants 

    printf("Voulez vous creer votre labyrinth ? 0 (oui), 1 (non) ");
    scanf("%d", &wantCreate);

    if (wantCreate == 0) {
        printf("Création du labyrinthe en cours...\n");
        Labyrinth *laby = creationLabyrinth(largeur, width);
        if (laby != NULL) {
            laby->name = nom;
            display(laby);

            printf("Voulez vous enregistrer ce labyrinth ? 0 (oui), 1 (non) ");
            scanf("%d", &wantSave);
            if(wantSave == 0){
                save_laby(laby);
            }


            free_laby(laby->grille, laby->height);
            free(laby);
        }
    } else {
        //keep_labyrinth()
    }
}



