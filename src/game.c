#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "maze.h"
#include "display.h"
#include "savelaby.h"
#include "monster.h"

static bool maze_shift(KEY_DIRECTION direction, Maze *maze, int *current_position,
                       bool *find_key, int *entered_cell);
static bool is_end(int *current_position, Maze *maze);
static void generate_random_object(Maze *maze);
static void score_game(int *score, int entered_cell);
static void save_score(int score, char *maze_name);


void start_game(Maze *maze){
    if (maze == NULL || maze->grille == NULL || maze->height < 3 || maze->width < 3){
        printf("  Le labyrinthe est invalide.\n");
        return;
    }

    char direction_input;
    int current_position[2] = {1, 0};
    int score = 0;
    bool find_key = false;
    maze->grille[0][1] = CELL_PLAYER;
    maze->grille[maze->height-1][maze->width-2] = CELL_EXIT;
    generate_random_object(maze);

    do {
        system("clear");
        if(!find_key){
            printf("  Vous devez trouver la clé pour ouvrir la sortie du labyrinth !\n");
        }
        else{
            printf("  Vous avez trouvé la clé: sortez au plus vite !\n");
        }

        printf("  Votre score : %d\n", score);

        display(maze);
        printf("  Entrez une direction : q, d, z, s\n\n");
        if (scanf(" %c", &direction_input) != 1){
            return;
        }

        switch (direction_input) {
            case Q:
            case D:
            case Z:
            case S: {
                int entered_cell;
                if (maze_shift((KEY_DIRECTION)direction_input, maze, current_position,
                               &find_key, &entered_cell)) {
                    score_game(&score, entered_cell);
                    score -= monster_direction(maze);
                }
                break;
            }
            default:
                break;
        }
    } while (!is_end(current_position, maze));

    printf("  Vous avez gagné la partie !\n");
    printf("  Votre score final : %d\n", score);
    save_score(score, maze->name);
}

static bool maze_shift(KEY_DIRECTION direction, Maze *maze, int *current_position,
                       bool *find_key, int *entered_cell){
    int next_x = current_position[0];
    int next_y = current_position[1];

    switch (direction) {
        case Q:
            next_x--;
            break;
        case D:
            next_x++;
            break;
        case Z:
            next_y--;
            break;
        case S:
            next_y++;
            break;
        default:
            return false;
    }

    if (next_x < 0 || next_x >= maze->width ||
        next_y < 0 || next_y >= maze->height){
        return false;
    }

    int destination = maze->grille[next_y][next_x];
    if (destination == CELL_EXIT && !*find_key) {
        return false;
    }

    if (destination != CELL_WAY && destination != CELL_EXIT &&
        destination != PENALTY && destination != KEY && destination != BONUS &&
        destination != GHOST && destination != OGRE) {
        return false;
    }

    *entered_cell = destination;
    if (destination == KEY) {
        *find_key = true;
    }

    maze->grille[current_position[1]][current_position[0]] = CELL_WAY;
    current_position[0] = next_x;
    current_position[1] = next_y;
    maze->grille[next_y][next_x] = CELL_PLAYER;
    return true;
}

static bool is_end(int *current_position, Maze *maze){
    return current_position[0] == maze->width - 2 && current_position[1] == maze->height - 1;
}


static Position *every_possibilities_object(Maze *maze, int *length) {
    if (maze == NULL || maze->grille == NULL || length == NULL) {
        return NULL;
    }

    int capacity = maze->height * maze->width;
    Position *positions = malloc(capacity * sizeof(*positions));
    if (positions == NULL) {
        return NULL;
    }

    *length = 0;
    for (int row = 1; row < maze->height - 1; row++) {
        for (int column = 1; column < maze->width - 1; column++) {
            if (maze->grille[row][column] == CELL_WAY) {
                positions[(*length)++] = (Position){row, column};
            }
        }
    }

    return positions;
}


static void generate_random_object(Maze *maze){

    int length = 0;
    Position *every_position = every_possibilities_object(maze, &length);
    if (every_position == NULL){
        return;
    }

    if (length == 0) {
        free(every_position);
        return;
    }

    int random_object = rand() % length;
    Position position = every_position[random_object];
    maze->grille[position.row][position.column] = KEY;
    every_position[random_object] = every_position[length-1];
    length--;

    int desired_per_type = maze->width/2 + maze->height/2;
    int nb_bonus = desired_per_type;
    int nb_penalty = desired_per_type;

    for (int i = 0; i < nb_bonus; i++) {
        random_object = rand() % length;
        position = every_position[random_object];
        maze->grille[position.row][position.column] = BONUS;
        every_position[random_object] = every_position[length - 1];
        length--;
    }

    for (int i = 0; i < nb_penalty; i++) {
        random_object = rand() % length;
        position = every_position[random_object];
        maze->grille[position.row][position.column] = PENALTY;
        every_position[random_object] = every_position[length - 1];
        length--;
    }

    if (length != 0 && maze->is_difficult == 0){

        int nb_ogre = desired_per_type/4;
        int nb_ghost = desired_per_type/4;

        for (int i = 0; i < nb_ghost; i++) {
            random_object = rand() % length;
            position = every_position[random_object];
            maze->grille[position.row][position.column] = GHOST;
            every_position[random_object] = every_position[length - 1];
            length--;
        }

        for (int i = 0; i < nb_ogre; i++) {
            random_object = rand() % length;
            position = every_position[random_object];
            maze->grille[position.row][position.column] = OGRE;
            every_position[random_object] = every_position[length - 1];
            length--;
        }
    }
    

    free(every_position);
}

static void score_game(int *score, int entered_cell){
    (*score)--;
    if (entered_cell == BONUS) {
        *score += 15;
    }
    else if (entered_cell == PENALTY) {
        *score -= 10;
    }
    else if (entered_cell == GHOST || entered_cell == OGRE){
        *score -= monster_penalty_points(entered_cell);
    }
}

static void save_score(int score, char *maze_name){
    char player_name[20];
    if(is_better_score(maze_name, score)){
        printf("\n  Nouveau Top 10 meilleur score !\n");
        printf("  Entrez votre nom pour enregistrer le score : ");
        if(scanf("%19s", player_name) != 1){
            printf("\n  Erreur de lecture du nom.\n");
            return;
        }
        save_file_score(score, maze_name, player_name);
    }
}
