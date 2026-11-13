#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "savelaby.h"
#include "creation.h"
#include "minunit.h"

static void make_unique_name(char *name, size_t size, const char *prefix){
    unsigned int suffix = (unsigned int)time(NULL);
    char filename[256];
    FILE *file;

    do {
        snprintf(name, size, "%s_%u", prefix, suffix++);
        snprintf(filename, sizeof(filename), "file/%s.cfg", name);
        file = fopen(filename, "r");
        if (file != NULL){
            fclose(file);
            continue;
        }

        snprintf(filename, sizeof(filename), "score/%s.score", name);
        file = fopen(filename, "r");
        if (file != NULL){
            fclose(file);
        }
    } while (file != NULL);
}

static const char *test_maze_file_round_trip_and_listing(void){
    char name[80];
    char filename[120];
    make_unique_name(name, sizeof(name), "minunit_maze");
    snprintf(filename, sizeof(filename), "file/%s.cfg", name);

    int cells[3][5] = {
        {CELL_WALL, CELL_PLAYER, CELL_WALL, CELL_WALL, CELL_WALL},
        {CELL_WALL, CELL_WAY, CELL_WAY, CELL_WAY, CELL_WALL},
        {CELL_WALL, CELL_WALL, CELL_WALL, CELL_EXIT, CELL_WALL}
    };
    int *rows[] = {cells[0], cells[1], cells[2]};
    Maze original = {
        .height = 3,
        .width = 5,
        .grille = rows,
        .name = name,
        .is_difficult = 1
    };

    save_maze(&original);
    Maze *loaded = read_file_maze(name);
    MU_ASSERT("sauvegarde/lecture: labyrinthe non relu", loaded != NULL);
    MU_ASSERT("sauvegarde/lecture: nom ou dimensions incorrects",
              strcmp(loaded->name, name) == 0 &&
              loaded->height == original.height &&
              loaded->width == original.width);
    MU_ASSERT("sauvegarde/lecture: difficulté incorrecte",
              loaded->is_difficult == original.is_difficult);
    MU_ASSERT("sauvegarde/lecture: cases non conservées",
              loaded->grille[0][0] == CELL_WALL &&
              loaded->grille[0][1] == CELL_PLAYER &&
              loaded->grille[1][1] == CELL_WAY &&
              loaded->grille[2][3] == CELL_EXIT);

    int count = 0;
    int difficulty = 1;
    char **names = array_maze_name(&count, &difficulty);
    int found = 0;
    for (int i = 0; i < count; i++){
        if (strcmp(names[i], name) == 0){
            found = 1;
        }
    }
    free_maze_names(names, count);
    free_maze(loaded->grille, loaded->height);
    free(loaded->name);
    free(loaded);
    remove(filename);
    MU_ASSERT("liste des labyrinthes: labyrinthe sauvegardé absent", found);
    return NULL;
}

static const char *test_invalid_maze_file_is_rejected(void){
    char name[80];
    char filename[120];
    make_unique_name(name, sizeof(name), "minunit_invalid");
    snprintf(filename, sizeof(filename), "file/%s.cfg", name);

    FILE *file = fopen(filename, "w");
    MU_ASSERT("fichier invalide: impossible de créer le fichier de test",
              file != NULL);
    fputs("contenu invalide\n", file);
    fclose(file);

    Maze *maze = read_file_maze(name);
    remove(filename);
    MU_ASSERT("fichier invalide: le fichier mal formé a été accepté",
              maze == NULL);
    MU_ASSERT("scores: un nom de labyrinthe NULL devrait être refusé",
              !is_better_score(NULL, 100));
    return NULL;
}

static const char *test_score_file_ranking(void){
    char name[80];
    char filename[120];
    make_unique_name(name, sizeof(name), "minunit_score");
    snprintf(filename, sizeof(filename), "score/%s.score", name);

    for (int i = 0; i < 10; i++){
        char player[20];
        snprintf(player, sizeof(player), "joueur%d", i);
        save_file_score(100 - i, name, player);
    }

    MU_ASSERT("scores: un score sous le top 10 devrait être refusé",
              !is_better_score(name, 0));
    MU_ASSERT("scores: un score supérieur au top 10 devrait être accepté",
              is_better_score(name, 101));
    save_file_score(101, name, "nouveau");

    FILE *file = fopen(filename, "r");
    MU_ASSERT("scores: fichier de score absent", file != NULL);
    int score;
    char player[20];
    int lines = 0;
    int first_score = 0;
    int last_score = 0;
    int previous_score = 101;
    while (fscanf(file, "%d %19s", &score, player) == 2){
        if (lines == 0){
            first_score = score;
        }
        if (score > previous_score){
            fclose(file);
            remove(filename);
            return "scores: classement non trié";
        }
        previous_score = score;
        last_score = score;
        lines++;
    }
    fclose(file);
    remove(filename);

    MU_ASSERT("scores: top 10 mal trié ou mal limité",
              lines == 10 && first_score == 101 && last_score == 92);
    return NULL;
}

const char *run_savelaby_tests(void){
    MU_RUN_TEST(test_maze_file_round_trip_and_listing);
    MU_RUN_TEST(test_invalid_maze_file_is_rejected);
    MU_RUN_TEST(test_score_file_ranking);
    return NULL;
}
