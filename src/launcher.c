#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "launcher.h"
#include "creation.h"
#include "display.h"
#include "managemaze.h"
#include "game.h"

typedef enum {
    create = 0, 
    play = 1, 
    load = 2,
    score = 3,
    exits = 4
}Menu;

static void display_score(char *name);
static void same_maze_name(char *nom);
static void choose_maze_name(char *nom);

/**
 * @brief Menu qui affiche les différents modes/choix
 */
static void menu(void){
    printf("\n");
    printf("  +--------------------------------------------+\n");
    printf("  |                 LABYRINTHE                 |\n");
    printf("  +--------------------------------------------+\n");
    printf("  |  0 - Creer un labyrinthe                   |\n");
    printf("  |  1 - Jouer                                 |\n");
    printf("  |  2 - Charger un labyrinthe                 |\n");
    printf("  |  3 - Consulter les meilleurs scores        |\n");
    printf("  |  4 - Quitter                               |\n");
    printf("  +--------------------------------------------+\n");
    printf("  Votre choix : ");
}

void launcher(void) {
    int choice;
    Maze *maze = NULL;

    printf("\n  ============================================\n");
    printf("           BIENVENUE DANS LE LABYRINTHE\n");
    printf("  ============================================\n");
    char name[50] = "";
    
    do {
        if (maze == NULL){
            printf("\n  Aucun labyrinthe charge pour le moment.\n");
        }
        else{
            printf("\n  Labyrinthe actif : %s (%d x %d)\n",
                   maze->name != NULL ? maze->name : "sans nom",
                   maze->height, maze->width);
        }

        menu();
        if (scanf("%d", &choice) != 1){
            int character;
            while ((character = getchar()) != '\n' && character != EOF){}
            system("clear");
            printf("  Choix invalide.\n");
            continue;
        }
        printf("\n");

        if (choice != exits){
            system("clear");
        }

        switch (choice){
        case create: {
            Maze *new_maze = generate_maze();
            if (new_maze != NULL){
                if (maze != NULL){
                    free_maze(maze->grid, maze->width);
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
                Maze *load_maze = read_file_maze(name);
                if (load_maze != NULL){
                    if (maze != NULL){
                        free_maze(maze->grid, maze->width);
                        free(maze->name);
                        free(maze);
                    }
                    maze = load_maze;
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
            printf("  Faute de frappes retapper votre choix\n\n");
            break;
        }


    } while (choice != exits);

    if (maze != NULL){
        free_maze(maze->grid, maze->width);
        free(maze->name);
        free(maze);
    }
}

Maze *generate_maze(void) {
    int height;
    int width;
    char name[50] = "";
    int difficulty;

    printf("  Veuillez insérer la hauteur de votre labyrinth (impaire): ");
    if (scanf("%d", &height) != 1){
        printf("Hauteur invalide.\n");
        return NULL;
    }
    printf("  Veuillez insérer la longueur de votre labyrinth (impaire): ");
    if (scanf("%d", &width) != 1){
        printf("Largeur invalide.\n");
        return NULL;
    }

    if (height < 3 || width < 3 || height % 2 == 0 || width % 2 == 0) {
        printf("  Les dimensions doivent être impaires et supérieures ou égales à 3.\n");
        return NULL;
    }

    //verfie qu'un même nom ne puisse pas être créé une nouvelle fois
    same_maze_name(name);
    if (name[0] == '\0'){
        return NULL;
    }

    printf("\n  Création du Maze en cours...\n\n");
    Maze *maze = create_maze(width, height);
    if (maze == NULL){
        return NULL;
    }

    maze->name = malloc(strlen(name) + 1);
    if (maze->name == NULL) {
        free_maze(maze->grid, maze->width);
        free(maze);
        printf("Erreur d'allocation mémoire pour le nom du labyrinth.\n");
        return NULL;
    }
    strcpy(maze->name, name);

    display(maze);

    printf("\n  Voulez-vous un labyrinth plus dur ?  -  (0) oui, (1) non");
    if(scanf("%d", &difficulty) != 1){
        printf("Choix invalide.\n");
        return NULL;
    }
    printf("\n");

    maze->is_difficult = difficulty;

    //si difficile on modifie la génération
    if(difficulty == 0){
        difficult_maze(maze);
    }

    display(maze);
    save_maze(maze);
    return maze;
}

void launch_maze(Maze *maze){
    if (maze == NULL){
        printf("  Veuillez charger ou créer un labyrinth d'abord\n");
        return;
    }

    start_game(maze);
}
    

/**
 * @brief Verifie qu'un nom existant ne puisse pas être pris
 * @param nom nom du labyrinth
 */
static void same_maze_name(char *nom){
    int maze_count = 0;
    char **maze_names = array_maze_name(&maze_count, NULL);
    bool name_exists;
    do {
        name_exists = false;
        printf("  Veuillez insérer le nom de votre labyrinth: ");
        if (scanf("%s", nom) != 1){
            free_maze_names(maze_names, maze_count);
            return;
        }
        printf("\n");

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

/**
 * @brief Permettre le choix du labyrinth a charger avec la liste de ceux déjà existant (difficile ou non)
 * @param nom nom du labyrinth
 */
static void choose_maze_name(char *nom){
    int maze_count = 0;
    int is_difficult = 1;

    printf("  Voulez-vous un niveau difficile ? (0) oui, (1) non ");
    if(scanf("%d", &is_difficult) != 1){
        printf("  Choix invalide.\n");
    }

    char **maze_names = array_maze_name(&maze_count, &is_difficult);
    bool is_present = false;

    if (maze_count == 0){
        printf("\n  Aucun labyrinthe enregistré\n\n");
        free_maze_names(maze_names, maze_count);
        nom[0] = '\0';
        return;
    }

    printf("\n  Listes des labyrinths déjà existants:\n\n");


    do {
        for (int i = 0; i < maze_count; i++){
            printf("    %s\n", maze_names[i]);
        }
        printf("\n\n");

        printf("  Veuillez insérer le nom du labyrinth choisi: ");
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
            printf("  Ce nom n'existe pas, veuillez en choisir à partir de la liste:\n\n");
        }
    } while (!is_present);
    free_maze_names(maze_names, maze_count);
}

/**
 * @brief Affiche le tableau des scores du labyrinth chargé 
 * @param name nom du labyrinth
 */
static void display_score(char *name){
    if(name[0] != '\0'){
        printf("Liste de tous les scores :\n\n");
        display_file_score(name);
    }
}
