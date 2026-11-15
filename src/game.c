#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "maze.h"
#include "display.h"
#include "managemaze.h"
#include "monster.h"

static bool maze_shift(KEY_DIRECTION direction, Maze *maze, int *current_position,
                       bool *find_key, int *entered_cell);
static bool is_end(int *current_position, Maze *maze);
static void generate_random_object(Maze *maze);
static void score_game(int *score, int entered_cell);
static void save_score(int score, char *maze_name);


void start_game(Maze *maze){
    if (maze == NULL || maze->grid == NULL || maze->height < 3 || maze->width < 3){
        printf("  Le labyrinthe est invalide.\n");
        return;
    }

    char direction_input;
    int current_position[2] = {1, 0};
    int score = 0;
    bool find_key = false;
    maze->grid[1][0] = CELL_PLAYER;
    maze->grid[maze->width-2][maze->height-1] = CELL_EXIT;
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
                int cell;
                if (maze_shift((KEY_DIRECTION)direction_input, maze, current_position,
                               &find_key, &cell)) {
                    score_game(&score, cell);
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

/**
 * @brief Mouvement du joueur grâce une direction en entrée
 * @param direction direction du joueur
 * @param maze pointeur du labyrinth
 * @param current_position postion actuelle du joueur
 * @param find_key boolean permettant de savoir si le joueur à réussi a capturer la clé
 * @param cell cellule de destination
 *
 * @return
 */
static bool maze_shift(KEY_DIRECTION direction, Maze *maze, int *current_position, bool *find_key, int *cell){
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

    if (next_x < 0 || next_x >= maze->width || next_y < 0 || next_y >= maze->height){
        return false;
    }

    int destination = maze->grid[next_x][next_y];
    //si il est a la sortie mais qu'il n'a pas la clé, le mouvement n'est pas valide
    if (destination == CELL_EXIT && !*find_key) {
        return false;
    }

    if (destination != CELL_WAY && destination != CELL_EXIT &&
        destination != PENALTY && destination != KEY && destination != BONUS &&
        destination != GHOST && destination != OGRE) {
        return false;
    }

    *cell = destination;
    if (destination == KEY) {
        *find_key = true;
    }

    //la position du joueur devient un chemain
    maze->grid[current_position[0]][current_position[1]] = CELL_WAY;
    current_position[0] = next_x;
    current_position[1] = next_y;
    maze->grid[next_x][next_y] = CELL_PLAYER;
    return true;
}

/**
 * @brief Etablie si la position actuelle est à la sortie
 * @param current_position position actuelle du joueur
 * @param maze pointeur du labyrinth
 *
 * @return boolean retournant la fin du labyrinth ou non
 */
static bool is_end(int *current_position, Maze *maze){
    return current_position[0] == maze->width - 2 && current_position[1] == maze->height - 1;
}


/**
 * @brief Liste de toutes les coordonées pour une cellule chemain
 * @param maze pointeur du labyrinth
 * @param length longueur de la liste des coordonnées
 *
 * @return la lisste des postions
 */
static Position *every_possibilities_object(Maze *maze, int *length) {
    if (maze == NULL || maze->grid == NULL || length == NULL) {
        return NULL;
    }

    int capacity = maze->height * maze->width;
    Position *positions = malloc(capacity * sizeof(*positions));
    if (positions == NULL) {
        return NULL;
    }

    *length = 0;
    for (int x = 1; x < maze->width - 1; x++) {
        for (int y = 1; y < maze->height - 1; y++) {
            if (maze->grid[x][y] == CELL_WAY) {
                positions[(*length)++] = (Position){x, y};
            }
        }
    }

    return positions;
}


/**
 * @brief Générer les bonus et malus sur le labyrinth à partir de la liste des postions des cellules chemain
 * @param maze pointeur du labyrinth
 */
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
    maze->grid[position.x][position.y] = KEY;
    every_position[random_object] = every_position[length-1];
    length--;

    int desired_per_type = maze->width/2 + maze->height/2; //1/4 du labyrinth est rempli de bonus et malus
    int nb_bonus = desired_per_type/2;
    int nb_penalty = desired_per_type/2;

    for (int i = 0; i < nb_bonus; i++) {
        random_object = rand() % length;
        position = every_position[random_object];
        maze->grid[position.x][position.y] = BONUS;
        every_position[random_object] = every_position[length - 1];
        length--;
    }

    for (int i = 0; i < nb_penalty; i++) {
        random_object = rand() % length;
        position = every_position[random_object];
        maze->grid[position.x][position.y] = PENALTY;
        every_position[random_object] = every_position[length - 1];
        length--;
    }

    if (length != 0 && maze->is_difficult == 0){

        int nb_ogre = desired_per_type/4;
        int nb_ghost = desired_per_type/4;

        for (int i = 0; i < nb_ghost; i++) {
            random_object = rand() % length;
            position = every_position[random_object];
            maze->grid[position.x][position.y] = GHOST;
            every_position[random_object] = every_position[length - 1];
            length--;
        }

        for (int i = 0; i < nb_ogre; i++) {
            random_object = rand() % length;
            position = every_position[random_object];
            maze->grid[position.x][position.y] = OGRE;
            every_position[random_object] = every_position[length - 1];
            length--;
        }
    }
    

    free(every_position);
}

/**
 * @brief Ajoute les bonus ou malus au joueur, a chaque deplacement, il pert 1 points
 * @param score score du joueur
 * @param cell cellule suivante
 */
static void score_game(int *score, int cell){
    (*score)--;
    if (cell == BONUS) {
        *score += 15;
    }
    else if (cell == PENALTY) {
        *score -= 10;
    }
    else if (cell == GHOST || cell == OGRE){
        *score -= monster_penalty_points(cell);
    }
}

/**
 * @brief enregistre le score du joueur dans un fichier si parmi les 10 meilleurs
 * @param score score du joueur
 * @param maze_name nom du labyrinth
 */
static void save_score(int score, char *maze_name){
    if(is_better_score(maze_name, score)){
        char player_name[100];
        printf("\n  Nouveau Top 10 des meilleurs scores !\n");
        printf("  Entrez votre nom pour enregistrer le score : ");
        if (scanf("%99s", player_name) != 1){
            printf("\n  Erreur de lecture du nom.\n");
            return;
        }

        save_file_score(score, maze_name, player_name);
    }
}
