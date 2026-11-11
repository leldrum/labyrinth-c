#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "launcher.h"
#include "creation.h"
#include "display.h"
#include "savelaby.h"
#include "game.h"

typedef enum {
    create = 0, 
    play = 1, 
    load = 2,
    score = 3,
    exits = 4
}Menu;

static void display_score(char *name);


void menu(){
    printf("\nCréer votre labyrinth => 0\n");
    printf("Jouer au labyrinth => 1\n");
    printf("Charger un labyrinth => 2\n");
    printf("Charger le tableau des scores => 3\n");
    printf("Quitter le jeux => 4\n");
}

void launcher(void) {
    int choice;
    Maze *maze = NULL;

    printf("Bienvenue dans le Labyrinth !\n\n");
    char name[50] = "";
    
    do {
        if (maze == NULL){
            printf("Pas encore de labyrinth chargé actuellement\n\n");
        }
        else{
            printf("Labyrinth actuellement chargé: %s\n\n", maze->name);
        }

        menu();
        if (scanf("%d", &choice) != 1){
            int character;
            while ((character = getchar()) != '\n' && character != EOF){}
            printf("Choix invalide.\n");
            continue;
        }
        printf("\n");

        switch (choice){
        case create: {
            Maze *new_maze = generate_maze();
            if (new_maze != NULL){
                if (maze != NULL){
                    free_maze(maze->grille, maze->height);
                    free(maze->name);
                    free(maze);
                }
                maze = new_maze;
            }
            break;
        }
        case play:
            launch_maze(maze);
            break;
        case load: {
            choose_maze_name(name);
            if (name[0] != '\0'){
                Maze *loaded_maze = read_file_maze(name);
                if (loaded_maze != NULL){
                    if (maze != NULL){
                        free_maze(maze->grille, maze->height);
                        free(maze->name);
                        free(maze);
                    }
                    maze = loaded_maze;
                }
            }
            break;
        }
        case score:
            display_score(name);
            break;
        case exits:
            break;
        default:
            printf("Faute de frappes retapper votre choix\n\n");
            break;
        }


    } while (choice != exits);

    if (maze != NULL){
        free_maze(maze->grille, maze->height);
        free(maze->name);
        free(maze);
    }
}

Maze *generate_maze(void) {
    int height;
    int width;
    char name[50] = "";

    printf("Veuillez insérer la hauteur de votre labyrinth (impaire): ");
    if (scanf("%d", &height) != 1){
        printf("Hauteur invalide.\n");
        return NULL;
    }
    printf("Veuillez insérer la longueur de votre labyrinth (impaire): ");
    if (scanf("%d", &width) != 1){
        printf("Largeur invalide.\n");
        return NULL;
    }

    if (height < 3 || width < 3 || height % 2 == 0 || width % 2 == 0) {
        printf("Les dimensions doivent être impaires et supérieures ou égales à 3.\n");
        return NULL;
    }

    same_maze_name(name);
    if (name[0] == '\0'){
        return NULL;
    }

    printf("Création du Maze en cours...\n");
    Maze *maze = creationMaze(height, width);
    if (maze == NULL){
        return NULL;
    }

    maze->name = malloc(strlen(name) + 1);
    if (maze->name == NULL) {
        free_maze(maze->grille, maze->height);
        free(maze);
        printf("Erreur d'allocation mémoire pour le nom du labyrinth.\n");
        return NULL;
    }
    strcpy(maze->name, name);
    display(maze);
    save_maze(maze);
    return maze;
}

void launch_maze(Maze *maze){
    if (maze == NULL){
        printf("Veuillez charger ou créer un labyrinth d'abord\n");
        return;
    }

    start_game(maze);
}
    

void same_maze_name(char *nom){
    int maze_count = 0;
    char **maze_names = array_maze_name(&maze_count);
    bool name_exists;
    do {
        name_exists = false;
        printf("Veuillez insérer le nom de votre labyrinth: ");
        if (scanf("%49s", nom) != 1){
            free_maze_names(maze_names, maze_count);
            return;
        }

        for (int i = 0; i < maze_count; i++){
            if (strcmp(nom, maze_names[i]) == 0){
                name_exists = true;
                printf("Ce nom existe déjà, veuillez en choisir un autre.\n");
                break;
            }
        }
    } while (name_exists);
    free_maze_names(maze_names, maze_count);
}

void choose_maze_name(char *nom){
    int maze_count = 0;
    char **maze_names = array_maze_name(&maze_count);
    bool is_present = false;

    if (maze_count == 0){
        printf("Aucun labyrinthe enregistré.\n");
        free_maze_names(maze_names, maze_count);
        nom[0] = '\0';
        return;
    }

    printf("Lites des labyrinths déjà existants:\n\n");


    do {
        for (int i = 0; i < maze_count; i++){
            printf("%s\n", maze_names[i]);
        }
        printf("\n");

        printf("Veuillez insérer le nom du labyrinth choisi: ");
        if (scanf("%49s", nom) != 1){
            free_maze_names(maze_names, maze_count);
            return;
        }
        printf("\n");

        for (int i = 0; i < maze_count; i++){
            if (strcmp(nom, maze_names[i]) == 0){
                is_present = true;
                break;
            }
        }
        if(!is_present){
            printf("Ce nom n'existe pas, veuillez en choisir à partir de la liste:\n\n");
        }
    } while (!is_present);
    free_maze_names(maze_names, maze_count);
}

static void display_score(char *name){
    if(name[0] != '\0'){
        printf("Liste de tous les scores :\n\n");
        display_file_score(name);
    }
}
