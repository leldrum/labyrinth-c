#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "managemaze.h"
#include "creation.h"


/* Construit un nom de fichier temporaire unique pour les tests (demandé à l'IA de me créer cette fonction) */
static int make_unique_name(char *name, size_t size, const char *prefix){
    static unsigned int counter = 0;
    struct timespec timestamp;
    if (timespec_get(&timestamp, TIME_UTC) != TIME_UTC){
        return 0;
    }
    int written = snprintf(name, size, "%s_%lld_%ld_%u", prefix, (long long)timestamp.tv_sec, timestamp.tv_nsec, counter++);
    return written >= 0 && (size_t)written < size;
}


/* Vérifie le rejet d'un fichier de labyrinthe invalide */
MU_TEST(test_invalid_maze_file){
    char name[80];
    char filename[120];
    make_unique_name(name, sizeof(name), "minunit_invalid");
    snprintf(filename, sizeof(filename), "file/%s.cfg", name);

    FILE *file = fopen(filename, "w");
    mu_assert(file != NULL, "impossible de créer le fichier de test");
    fputs("contenu invalide\n", file);
    fclose(file);

    Maze *maze = read_file_maze(name);
    mu_assert(maze == NULL, "fichier mal formé a été accepté");
    mu_assert(!is_better_score(NULL, 100), "nom de labyrinthe NULL devrait être refusé");

    remove(filename);
}

/* Vérifie lecture et sauvegarde d'une grille */
MU_TEST(test_rectangular_maze_file_round_trip){
    char name[80];
    char filename[120];
    make_unique_name(name, sizeof(name), "minunit_rectangular");
    snprintf(filename, sizeof(filename), "file/%s.cfg", name);

    Maze *maze = create_maze(5, 3);
    maze->is_difficult = 0;
    maze->name = name;

    save_maze(maze);
    Maze *loaded = read_file_maze(name);
    mu_assert(loaded != NULL, "labyrinth non relu");
    mu_assert_int_eq(loaded->width, maze->width);
    mu_assert_int_eq(loaded->height, maze->height);
    mu_assert_int_eq(loaded->grid[1][0], CELL_PLAYER);
    mu_assert_int_eq(loaded->grid[3][2], CELL_EXIT);

    remove(filename);
    free_maze(maze->grid, maze->width);
    free_maze(loaded->grid, loaded->width);
    free(loaded->name);
    free(maze);
    free(loaded);
}

/* Vérifie que le nom du joueur n'est pas tronqué lors de la sauvegarde. */
MU_TEST(test_long_score_player_name){
    char maze_name[80];
    char filename[120];
    const char *player_name = "joueur_avec_un_nom_de_plus_de_vingt_caracteres";
    mu_assert(make_unique_name(maze_name, sizeof(maze_name), "minunit_score"),
              "impossible de générer un nom de fichier de test");
    snprintf(filename, sizeof(filename), "score/%s.score", maze_name);

    save_file_score(10, maze_name, player_name);

    FILE *file = fopen(filename, "r");
    char line[128];
    int saved = file != NULL && fgets(line, sizeof(line), file) != NULL &&
                strcmp(line, "10 joueur_avec_un_nom_de_plus_de_vingt_caracteres\n") == 0;
    if (file != NULL) {
        fclose(file);
    }
    remove(filename);

    mu_assert(saved, "le nom du joueur a été tronqué");
}

MU_TEST_SUITE(managemaze_suite){
    MU_RUN_TEST(test_invalid_maze_file);
    MU_RUN_TEST(test_rectangular_maze_file_round_trip);
    MU_RUN_TEST(test_long_score_player_name);
}
