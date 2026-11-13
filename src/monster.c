#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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


static int find_monsters(Maze *maze, Position *monsters){
    int count = 0;
    for (int row = 0; row < maze->height; row++){
        for (int column = 0; column < maze->width; column++){
            int cell = maze->grille[row][column];
            if (cell == GHOST || cell == OGRE){
                monsters[count++] = (Position){row, column};
            }
        }
    }
    return count;
}

static Position find_object(Maze *maze, int object){
    for (int row = 0; row < maze->height; row++){
        for (int column = 0; column < maze->width; column++){
            if (maze->grille[row][column] == object){
                return (Position){row, column};
            }
        }
    }
    return (Position){-1, -1};
}
static int move_monster(Maze *maze, Position monster){
    int type = maze->grille[monster.row][monster.column];
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

static int move_ghost(Maze *maze, Position ghost, Position player){
    if (player.row < 0 || player.column < 0){
        return 0;
    }

    int difference_row = ghost.row - player.row;
    int difference_column = ghost.column - player.column;
    int result;

    if (abs(difference_column) >= abs(difference_row)){
        int column_direction = difference_column > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, 0, column_direction);
        if (result != -1 || difference_row == 0){
            return result == -1 ? 0 : result;
        }
        int row_direction = difference_row > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, row_direction, 0);
    }
    else{
        int row_direction = difference_row > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, row_direction, 0);
        if (result != -1 || difference_column == 0){
            return result == -1 ? 0 : result;
        }
        int column_direction = difference_column > 0 ? -1 : 1;
        result = move_ghost_direction(maze, ghost, 0, column_direction);
    }

    return result == -1 ? 0 : result;
}

static int move_ghost_direction(Maze *maze, Position ghost,
                                   int row_direction, int column_direction){
    int row = ghost.row + row_direction;
    int column = ghost.column + column_direction;

    while (row >= 0 && row < maze->height &&
           column >= 0 && column < maze->width){
        int destination = maze->grille[row][column];
        if (destination == CELL_WALL){
            row += row_direction;
            column += column_direction;
            continue;
        }
        if (destination == CELL_PLAYER){
            return monster_penalty_points(GHOST);
        }
        if (destination != CELL_WAY){
            return -1;
        }

        maze->grille[ghost.row][ghost.column] = CELL_WAY;
        maze->grille[row][column] = GHOST;
        return 0;
    }

    return -1;
}


Position ogre_bfs(Maze *maze, Position ogre, Position key, int *path_length){

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

            int next_y = current_position.column + positions[i].column;
            int next_x = current_position.row + positions[i].row; 
            Position next_position = {next_x, next_y};

            if(next_x < 0 || next_x >= maze->height ||
               next_y < 0 || next_y >= maze->width){
                continue;
            }

            if(maze->grille[next_x][next_y] == CELL_WALL){
                continue;
            }

            int already_visited = 0;
            for (int j = 0; j < tail; j++){
                if(visited[j].column == next_y && visited[j].row == next_x){
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
            if(next_y == key.column && next_x == key.row){
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
    while(parent[index].row != ogre.row || parent[index].column != ogre.column){

        Position previous = parent[index];
        int previous_index = -1;
        for (int i = 0; i < tail; i++){
            if(queue[i].row == previous.row && queue[i].column == previous.column){
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

static int move_ogre(Maze *maze, Position ogre, Position key){
    if (ogre.row < 0 || ogre.column < 0 ||
        key.row < 0 || key.column < 0){
        return 0;
    }

    int path_length = 0;
    Position path = ogre_bfs(maze, ogre, key, &path_length);
    if (path_length == 0){
        return 0;
    }

    int destination = maze->grille[path.row][path.column];

    if (destination == CELL_PLAYER){
        printf("  L'ogre vous a mis dans le mal : -15 points\n");
        return monster_penalty_points(OGRE);
    }

    if (destination == KEY){
        return 0;
    }

    if (destination == BONUS || destination == CELL_WAY || destination == PENALTY || destination == GHOST){
        maze->grille[ogre.row][ogre.column] = destination;
        maze->grille[path.row][path.column] = OGRE;
    }
    return 0;
}
