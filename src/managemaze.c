#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/stat.h>

#include "display.h"
#include "managemaze.h"

#ifndef MAZE_DIR
#define MAZE_DIR "file"
#endif

#define MAX_BEST_SCORES 10
#define MAX_PLAYER_NAME_LENGTH 100

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

    //print la difficulté
    fprintf(file, "%d\n",maze->is_difficult);

    //print la hauteur et la largeur
    fprintf(file, "%d\n",maze->height);
    fprintf(file, "%d\n", maze->width);

    //print le Maze
    for (int y = 0; y < maze->height; y++){
        for (int x = 0; x < maze->width; x++){
            fprintf(file, "%c", maze_symbol(maze->grid[x][y]));
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\n  Enregistrement du fichier avec succès !\n");
}

/**
 * @brief nettoie les fuites de mémoire
 * @param file fichier manipulé
 * @param maze pointeur du labyrinth 
 *
 * @return
 */
static void clean_read_maze(FILE *file, Maze *maze){
    if (file != NULL){
        fclose(file);
    }
    if (maze->grid != NULL){
        for (int x = 0; x < maze->width; x++){
            free(maze->grid[x]);
        }
        free(maze->grid);
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
        clean_read_maze(file, maze);
        return NULL;
    }

    if (fscanf(file, "%d", &maze->is_difficult) != 1){
        printf("Le fichier du labyrinthe est invalide.\n");
        clean_read_maze(file, maze);
        return NULL;
    }

    maze_name[strcspn(maze_name, "\n")] = '\0';

    if (fscanf(file, "%d%d", &maze->height, &maze->width) != 2){
        printf("Le fichier du labyrinthe est invalide.\n");
        clean_read_maze(file, maze);
        return NULL;
    }

    //consommer le \n
    int newline = fgetc(file);
    if (newline != '\n'){
        printf("Le fichier du labyrinthe est invalide.\n");
        clean_read_maze(file, maze);
        return NULL;
    }

    maze->name = malloc(strlen(maze_name) + 1);
    maze->grid = calloc(maze->width, sizeof(*maze->grid));
    if (maze->name == NULL || maze->grid == NULL){
        printf("Erreur d'allocation mémoire.\n");
        clean_read_maze(file, maze);
        return NULL;
    }

    strcpy(maze->name, maze_name);

    for (int x = 0; x < maze->width; x++){
        maze->grid[x] = malloc(maze->height * sizeof(*maze->grid[x]));
        if (maze->grid[x] == NULL){
            printf("Erreur d'allocation mémoire.\n");
            clean_read_maze(file, maze);
            return NULL;
        }
    }

    for (int y = 0; y < maze->height; y++){
        for (int x = 0; x < maze->width; x++){
            int cell = fgetc(file);
            if (cell != '#' && cell != 'O' && cell != '-' && cell != ' '){
                printf("Le fichier du labyrinthe est invalide.\n");
                clean_read_maze(file, maze);
                return NULL;
            }
            maze->grid[x][y] = maze_cell_from_symbol(cell);
        }

        newline = fgetc(file);
        if (newline != '\n'){
            printf("Le fichier du labyrinthe est invalide.\n");
            clean_read_maze(file, maze);
            return NULL;
        }
    }

    fclose(file);
    return maze;
}

char **array_maze_name(int *count, int *is_difficult){
    if (count == NULL) {
        return NULL;
    }

    *count = 0;
    DIR *directory = opendir(MAZE_DIR);
    if (directory == NULL) {
        return NULL;
    }

    int capacity = 0;
    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        int filename_length = strlen(entry->d_name);
        if (filename_length <= 4 || strcmp(entry->d_name + filename_length - 4, ".cfg") != 0) {
            continue;
        }
        capacity++;
    }

    if (capacity == 0) {
        closedir(directory);
        return NULL;
    }

    char **names = calloc(capacity, sizeof(*names));
    if (names == NULL) {
        closedir(directory);
        return NULL;
    }

    rewinddir(directory);
    while ((entry = readdir(directory)) != NULL) {
        int filename_length = strlen(entry->d_name);
        if (filename_length <= 4 ||
            strcmp(entry->d_name + filename_length - 4, ".cfg") != 0) {
            continue;
        }

        if (is_difficult != NULL) {
            char filename[256];
            char maze_name[256];
            int maze_difficulty;
            snprintf(filename, sizeof(filename), "%s/%s", MAZE_DIR, entry->d_name);
            FILE *file = fopen(filename, "r");
            if (file == NULL) {
                continue;
            }

            bool matches_difficulty = fgets(maze_name, sizeof(maze_name), file) != NULL &&
                fscanf(file, "%d", &maze_difficulty) == 1 &&
                maze_difficulty == *is_difficult;
            fclose(file);
            if (!matches_difficulty) {
                continue;
            }
        }

        if (*count >= capacity) {
            continue;
        }

        int name_length = filename_length - 4;
        names[*count] = malloc(name_length + 1);
        if (names[*count] == NULL) {
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

/**
 * @brief Insère un score dans le classement conservé en mémoire
 * @param scores tableau des scores
 * @param player_names noms des joueurs
 * @param count nombre de scores 
 * @param score nouveau score
 * @param player_name nom du joueur
 */
static void insert_score(int scores[MAX_BEST_SCORES], char player_names[MAX_BEST_SCORES][MAX_PLAYER_NAME_LENGTH], int *count, int score, const char *player_name){
    int position = 0;
    while (position < *count && scores[position] >= score) {
        position++;
    }

    if (position >= MAX_BEST_SCORES) {
        return;
    }

    //si le nb de score est deja a 10 on laisse sinon on augmente de 1
    int new_count = *count < MAX_BEST_SCORES ? *count + 1 : *count;

    for (int i = new_count-1; i > position; i--) {
        scores[i] = scores[i-1];
        strcpy(player_names[i], player_names[i-1]);
    }

    scores[position] = score;
    snprintf(player_names[position], MAX_PLAYER_NAME_LENGTH, "%s", player_name);
    *count = new_count;
}

/**
 * @brief Construit le classement du score en mémoire
 * @param filename nom du fichier
 * @param scores scores de tous les joueurs
 * @param player_names le nom de tous les joueurs
 * @param [in, out] count le nombre de resultat dans le fichier
 *
 * @return validation ou non de l'insertion
 */
static bool read_top_scores(char *filename, int scores[MAX_BEST_SCORES], char player_names[MAX_BEST_SCORES][MAX_PLAYER_NAME_LENGTH], int *count){
    *count = 0;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier du score");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        int score;
        char player_name[MAX_PLAYER_NAME_LENGTH];
        if (sscanf(line, "%d %s", &score, player_name) == 2) {
            insert_score(scores, player_names, count, score, player_name);
        }
    }

    fclose(file);
    return true;
}

bool is_better_score(char *name, int score_game){
    char filename[256];
    int scores[MAX_BEST_SCORES];
    char player_names[MAX_BEST_SCORES][MAX_PLAYER_NAME_LENGTH];
    int count;
    if (name == NULL){
        printf("Le nom du labyrinthe est invalide.\n");
        return false;
    }

    snprintf(filename, sizeof(filename), "score/%s.score", name);

    if (!read_top_scores(filename, scores, player_names, &count)) {
        return false;
    }

    bool is_better = count < MAX_BEST_SCORES || score_game > scores[count-1];
    if (!is_better) {
        printf("  Interdiction d'enregistrer ce score : il n'est pas dans les 10 meilleurs.\n");
    }
    return is_better;
}

void save_file_score(int score, char *name, const char *player_name){
    char filename[256];
    int scores[MAX_BEST_SCORES];
    char player_names[MAX_BEST_SCORES][MAX_PLAYER_NAME_LENGTH];
    int count;
    if (name == NULL || player_name == NULL){
        printf("Le nom du labyrinthe est invalide.\n");
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
        char player_name[MAX_PLAYER_NAME_LENGTH];
        if (sscanf(line, "%d %99s", &score, player_name) == 2) {
            printf("  score = %d  -  joueur = %s\n", score, player_name);
        }
    }

    fclose(file);
}
