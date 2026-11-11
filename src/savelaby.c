#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>

#include "display.h"
#include "savelaby.h"

#ifndef MAZE_DIR
#define MAZE_DIR "file"
#endif

#define MAX_BEST_SCORES 10
#define PLAYER_NAME_LENGTH 20

void save_maze(Maze *maze){
    if (maze == NULL || maze->name == NULL) {
        printf("Le Maze est invalide pour l'enregistrement.\n");
        return;
    }

    char name[256];
    snprintf(name, sizeof(name), "file/%s.cfg", maze->name);
    FILE *file = fopen(name, "w");
    if (file == NULL) {
        printf("Le nom du labyrinthe est invalide.\n");
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

static void cleanup_read_maze(FILE *file, Maze *maze){
    if (file != NULL){
        fclose(file);
    }
    if (maze->grille != NULL){
        for (int i = 0; i < maze->height; i++){
            free(maze->grille[i]);
        }
        free(maze->grille);
    }
    free(maze->name);
    free(maze);
}

Maze *read_file_maze(char *name){
    char filename[256];
    char maze_name[256];
    if (name == NULL){
        printf("Le nom du labyrinthe est invalide.\n");
        return NULL;
    }

    snprintf(filename, sizeof(filename), "file/%s.cfg", name);

    FILE *file = fopen(filename, "r");
    if (file == NULL){
        perror("Impossible d'ouvrir le fichier du labyrinthe");
        return NULL;
    }

    Maze *maze = calloc(1, sizeof(*maze));
    if (maze == NULL){
        fclose(file);
        printf("Erreur d'allocation mémoire.\n");
        return NULL;
    }

    if (fgets(maze_name, sizeof(maze_name), file) == NULL){
        printf("Le fichier du labyrinthe est invalide.\n");
        cleanup_read_maze(file, maze);
        return NULL;
    }

    //enlver le \n du nom 
    maze_name[strcspn(maze_name, "\r\n")] = '\0';

    if (fscanf(file, "%d%d", &maze->height, &maze->width) != 2){
        printf("Le fichier du labyrinthe est invalide.\n");
        cleanup_read_maze(file, maze);
        return NULL;
    }

    int newline = fgetc(file);
    if (newline == '\r'){
        newline = fgetc(file);
    }
    

    maze->name = malloc(strlen(maze_name) + 1);
    maze->grille = calloc(maze->height, sizeof(*maze->grille));
    if (maze->name == NULL || maze->grille == NULL){
        printf("Erreur d'allocation mémoire.\n");
        cleanup_read_maze(file, maze);
        return NULL;
    }

    strcpy(maze->name, maze_name);

    for (int i = 0; i < maze->height; i++){
        maze->grille[i] = malloc(maze->width * sizeof(*maze->grille[i]));
        if (maze->grille[i] == NULL){
            printf("Erreur d'allocation mémoire.\n");
            cleanup_read_maze(file, maze);
            return NULL;
        }

        for (int j = 0; j < maze->width; j++){
            int cell = fgetc(file);
            if (cell != '#' && cell != 'O' && cell != '-' && cell != ' '){
                printf("Le fichier du labyrinthe est invalide.\n");
                cleanup_read_maze(file, maze);
                return NULL;
            }
            maze->grille[i][j] = num_val_Maze((char)cell);
        }

        newline = fgetc(file);
        if (newline == '\r'){
            newline = fgetc(file);
        }
        if (newline != '\n' && newline == EOF){
            printf("Le fichier du labyrinthe est invalide.\n");
            cleanup_read_maze(file, maze);
            return NULL;
        }
    }

    fclose(file);
    return maze;
}

char **array_maze_name(int *count){
    if (count == NULL){
        return NULL;
    }

    *count = 0;
    DIR *directory = opendir(MAZE_DIR);
    if (directory == NULL){
        return NULL;
    }

    char **names = NULL;
    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL){
        size_t filename_length = strlen(entry->d_name);
        if (filename_length <= 4 || strcmp(entry->d_name + filename_length - 4, ".cfg") != 0){
            continue;
        }

        char **new_names = realloc(names, (size_t)(*count + 1) * sizeof(*names));
        if (new_names == NULL){
            free_maze_names(names, *count);
            closedir(directory);
            return NULL;
        }
        names = new_names;

        size_t name_length = filename_length - 4;
        names[*count] = malloc(name_length + 1);
        if (names[*count] == NULL){
            free_maze_names(names, *count);
            closedir(directory);
            *count = 0;
            return NULL;
        }
        memcpy(names[*count], entry->d_name, name_length);
        names[*count][name_length] = '\0';
        (*count)++;
    }

    closedir(directory);
    return names;
}

void free_maze_names(char **names, int count){
    for (int i = 0; i < count; i++){
        free(names[i]);
    }
    free(names);
}


static void insert_score(int scores[MAX_BEST_SCORES],
                         char player_names[MAX_BEST_SCORES][PLAYER_NAME_LENGTH],
                         int *count, int score, const char *player_name){
    int position = 0;
    while (position < *count && scores[position] >= score) {
        position++;
    }

    if (position >= MAX_BEST_SCORES) {
        return;
    }

    int new_count = *count < MAX_BEST_SCORES ? *count + 1 : *count;
    for (int i = new_count-1; i > position; i--) {
        scores[i] = scores[i-1];
        memcpy(player_names[i], player_names[i-1], PLAYER_NAME_LENGTH);
    }

    scores[position] = score;
    snprintf(player_names[position], PLAYER_NAME_LENGTH, "%s", player_name);
    *count = new_count;
}

static bool read_top_scores(char *filename,
                            int scores[MAX_BEST_SCORES],
                            char player_names[MAX_BEST_SCORES][PLAYER_NAME_LENGTH],
                            int *count){
    *count = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        if (errno == ENOENT) {
            return true;
        }
        perror("Impossible d'ouvrir le fichier du score");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        int score;
        char player_name[PLAYER_NAME_LENGTH];
        if (sscanf(line, "%d %19s", &score, player_name) == 2) {
            insert_score(scores, player_names, count, score, player_name);
        }
    }

    fclose(file);
    return true;
}

bool is_better_score(char *name, int score_game){
    char filename[256];
    int scores[MAX_BEST_SCORES];
    char player_names[MAX_BEST_SCORES][PLAYER_NAME_LENGTH];
    int count;
    if (name == NULL){
        printf("Le nom du labyrinthe est invalide.\n");
        return false;
    }

    snprintf(filename, sizeof(filename), "score/%s.score", name);

    if (!read_top_scores(filename, scores, player_names, &count)) {
        return false;
    }

    if (count < MAX_BEST_SCORES || score_game > scores[count - 1]) {
        return true;
    }

    printf("Interdiction d'enregistrer ce score : il n'est pas dans les 10 meilleurs.\n");
    return false;
}

void save_file_score(int score, char *name, char *player_name){
    char filename[256];
    int scores[MAX_BEST_SCORES];
    char player_names[MAX_BEST_SCORES][PLAYER_NAME_LENGTH];
    int count;
    if (name == NULL || player_name == NULL){
        printf("Le nom du labyrinthe est invalide.\n");
        return;
    }

    if (mkdir("score", 0777) != 0 && errno != EEXIST) {
        perror("Impossible de créer le dossier des scores");
        return;
    }

    snprintf(filename, sizeof(filename), "score/%s.score", name);

    if (!read_top_scores(filename, scores, player_names, &count)) {
        return;
    }

    insert_score(scores, player_names, &count, score, player_name);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier du score");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(file, "%d %s\n", scores[i], player_names[i]) < 0) {
            perror("Impossible d'enregistrer le score");
            break;
        }
    }

    fclose(file);
    printf("Enregistrement de votre score avec succès !\n");
}

void display_file_score(char *name){
    if (name == NULL || name[0] == '\0') {
        printf("Aucun labyrinthe sélectionné.\n");
        return;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "score/%s.score", name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier du score, il n'existe surement pas\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        int score;
        char player_name[PLAYER_NAME_LENGTH];
        if (sscanf(line, "%d %19s", &score, player_name) == 2) {
            printf("score = %d  -  joueur = %s\n", score, player_name);
        }
    }

    fclose(file);
}
