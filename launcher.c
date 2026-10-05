#include <stdio.h>

#define PLAYER 0
#define DOOR 1
#define ENTER 2
#define EXIT 3

void diplay(){

}


void generate(){
    int largeur;
    int width;
    char *nom;

    printf("Veuillez insérer la largeur de votre labyrinthe (impaire): ");
    scanf("%d", &largeur);
    printf("Veuillez insérer la width de votre labyrinthe (impaire): ");
    scanf("%d", &width);

    if(largeur % 2 == 0 || width % 2 == 0){
        printf("Vous ne respectez pas la condition d'une width/largeur impaire.");
        return;
    }

    printf("Veuillez insérer votre pseudo");
    scanf("%s", &nom);
}

