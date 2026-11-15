#include <stdio.h>
#include <stdlib.h>

#include "maze.h"
#include "monster.h"

static int find_monsters(Maze *maze, Position *monsters);
static Position find_object(Maze *maze, int object);
static int move_monster(Maze *maze, Position monster);
static int move_ghost(Maze *maze, Position ghost, Position player);
static int move_ghost_direction(Maze *maze, Position ghost,
                                   int row_direction, int column_direction);
static int move_ogre(Maze *maze, Position ogre, Position key);

int monster_penalty_points(int monster_type){
    if (monster_type == GHOST){
        return 10;
    }
    if (monster_type == OGRE){
        return 15;
    }
    return 0;
}

int monster_direction(Maze *maze){
    int cell_count = maze->height * maze->width;
    if (cell_count == 0){
        return 0;
    }

    Position *monsters = malloc(cell_count * sizeof(*monsters));
    if (monsters == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire pour les monstres.\n");
        return 0;
    }

    int monster_count = find_monsters(maze, monsters);
    int total_penalty = 0;
    for (int i = 0; i < monster_count; i++){
        total_penalty += move_monster(maze, monsters[i]);
    }

    free(monsters);
    return total_penalty;
}


/**
 * @brief Lister tous les monstres présents sur le labyrinth
 * @param maze pointeur du labyrinth
 * @param monsters liste de position des monstres
 *
 * @return le nombre de monstre sur le labyrinth
 */
static int find_monsters(Maze *maze, Position *monsters){
    int count = 0;
    for (int x = 0; x < maze->width; x++){
        for (int y = 0; y < maze->height; y++){
            int cell = maze->grid[x][y];
            if (cell == GHOST || cell == OGRE){
                monsters[count++] = (Position){x, y};
            }
        }
    }
    return count;
}

/**
 * @brief Renvoie la position d'une entité du labyrinth
 * @param maze labyrinth
 * @param object entité du labyrinth qui n'a pas de double
 *
 * @return la position de l'objet
 */
static Position find_object(Maze *maze, int object){
    for (int x = 0; x < maze->width; x++){
        for (int y = 0; y < maze->height; y++){
            if (maze->grid[x][y] == object){
                return (Position){x, y};
            }
        }
    }
    return (Position){-1, -1};
}
/**
 * @brief Determiner le mouvement du monstre en fonction de son entité (ogre ou fantome) et renvoyer la penalité
 * @param maze labyrinth
 * @param monster type de monstre
 *
 * @return la pénalité attribuée au joueur
 */
static int move_monster(Maze *maze, Position monster){
    int type = maze->grid[monster.x][monster.y];
    if (type == GHOST){
        Position player = find_object(maze, CELL_PLAYER);
        return move_ghost(maze, monster, player);
    }
    if (type == OGRE){
        Position key = find_object(maze, KEY);
        return move_ogre(maze, monster, key);
    }
    return 0;
}

/**
 * @brief Etablie le mouvement du fantome
 * @param maze labyrinth
 * @param ghost position du fantome
 * @param player position du joueur
 *
 * @return la penalité attribuée au joueur
 */
static int move_ghost(Maze *maze, Position ghost, Position player){
    if (player.x < 0 || player.y < 0){
        return 0;
    }

    int difference_x = ghost.x - player.x;
    int difference_y = ghost.y - player.y;
    int result;

    if (abs(difference_x) >= abs(difference_y)){
        int x_direction = difference_x > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, x_direction, 0);
        if (result != -1 || difference_y == 0){
            return result == -1 ? 0 : result;
        }
        int y_direction = difference_y > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, 0, y_direction);
    }
    else{
        int y_direction = difference_y > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, 0, y_direction);
        if (result != -1 || difference_x == 0){
            return result == -1 ? 0 : result;
        }
        int x_direction = difference_x > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, x_direction, 0);
    }

    return result == -1 ? 0 : result;
}

/**
 * @brief Etablissement des penalités en fonction du mouvement du fantomes
 * @param maze labyrinth
 * @param ghost position du fantome
 * @param x_direction direction x du joueur
 * @param y_direction direcdtion y du joueur
 *
 * @return la penalité 
 */
static int move_ghost_direction(Maze *maze, Position ghost, int x_direction, int y_direction){
    int x = ghost.x + x_direction;
    int y = ghost.y + y_direction;

    while (x >= 0 && x < maze->width &&
           y >= 0 && y < maze->height){
        int destination = maze->grid[x][y];
        if (destination == CELL_WALL){
            x += x_direction;
            y += y_direction;
            continue;
        }
        if (destination == CELL_PLAYER){
            return monster_penalty_points(GHOST);
        }
        if (destination != CELL_WAY){
            return -1;
        }

        maze->grid[ghost.x][ghost.y] = CELL_WAY;
        maze->grid[x][y] = GHOST;
        return 0;
    }

    return -1;
}


/**
 * @brief Retourne la position que l'ogre doit faire pour aller vers la clé parcours bfs jusqua la clé
 * @param maze labyrinth
 * @param ogre position de l'ogre
 * @param key postion de la clé
 * @param [in, out] path_length longueur de cellule vers la clé 
 *
 * @return la prochaine postion pour aller vers la clé 
 */
static Position ogre_bfs(Maze *maze, Position ogre, Position key, int *path_length){

    int capacity = maze->height * maze->width;
    int head = 0;
    int tail = 0;
    Position *queue = malloc(capacity * sizeof(*queue));
    Position *parent = malloc(capacity * sizeof(*parent));
    Position *visited = malloc(capacity * sizeof(*visited));

    if(queue == NULL || parent == NULL || visited == NULL){
        free(queue);
        free(parent);
        free(visited);
        *path_length = 0;
        return ogre;
    }

    queue[tail++] = ogre;
    visited[0] = ogre;
    parent[0] = (Position){-1, -1};

    Position positions[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int find_key = -1;

    while(head < tail && find_key == -1){

        Position current_position = queue[head];

        for (int i = 0; i < 4; i++){

            int next_x = current_position.x + positions[i].x;
            int next_y = current_position.y + positions[i].y;
            Position next_position = {next_x, next_y};

            if(next_x < 0 || next_x >= maze->width ||
               next_y < 0 || next_y >= maze->height){
                continue;
            }

            if(maze->grid[next_x][next_y] == CELL_WALL){
                continue;
            }

            int already_visited = 0;
            for (int j = 0; j < tail; j++){
                if(visited[j].x == next_x && visited[j].y == next_y){
                    already_visited = 1;
                    break;
                }
            }
            if(already_visited){
                continue;
            }

            visited[tail] = next_position;
            parent[tail] = current_position;
            queue[tail] = next_position;
            if(next_x == key.x && next_y == key.y){
                find_key = tail;
            }
            tail++;
        }
        head++;
    }

    //si pas de clé trouvée on laisse l'ogre à sa place
    if(find_key == -1){
        free(visited);
        free(queue);
        free(parent);
        *path_length = 0;
        return ogre;
    }

    int index = find_key;
    *path_length = 1;
    while(parent[index].x != ogre.x || parent[index].y != ogre.y){

        Position previous = parent[index];
        int previous_index = -1;
        for (int i = 0; i < tail; i++){
            if(queue[i].x == previous.x && queue[i].y == previous.y){
                previous_index = i;
                break;
            }
        }
        if(previous_index == -1){
            free(visited);
            free(queue);
            free(parent);
            *path_length = 0;
            return ogre;
        }
        index = previous_index;
        (*path_length)++;
    }

    Position path = queue[index];
    free(visited);
    free(parent);
    free(queue);

    return path; 
    
}

/**
 * @brief Renvoie la penalité et met a jour la postion de l'ogre vers la clé
 * @param maze labyrinth
 * @param ogre position de l'ogre
 * @param key position de la clé 
 *
 * @return la penalité
 */
static int move_ogre(Maze *maze, Position ogre, Position key){
    if (ogre.x < 0 || ogre.y < 0 ||
        key.x < 0 || key.y < 0){
        return 0;
    }

    int path_length = 0;
    Position path = ogre_bfs(maze, ogre, key, &path_length);
    if (path_length == 0){
        return 0;
    }

    int destination = maze->grid[path.x][path.y];

    if (destination == CELL_PLAYER){
        printf("  L'ogre vous a mis dans le mal : -15 points\n");
        return monster_penalty_points(OGRE);
    }

    if (destination == KEY){
        return 0;
    }

    if (destination == BONUS || destination == CELL_WAY || destination == PENALTY || destination == GHOST){
        maze->grid[ogre.x][ogre.y] = destination;
        maze->grid[path.x][path.y] = OGRE;
    }
    return 0;
}
