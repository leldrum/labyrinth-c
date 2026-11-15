#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "creation.h"
#include "display.h"

static bool is_wall_outline(Maze *maze, int y, int x);
static Position *every_possibilities_difficult_maze(Maze *maze, int *length);



/**
 * @brief Crée une ligne qui permet de faire création d'une futur grille
 * @param dimension dimension du labyrinth
 * @param val la valeur a l'initilisation de la ligne
 *
 * @return une ligne de notre futur grille
 */
static int *allocate_line(int dimension, int val){
    int *tab = malloc(dimension * sizeof(*tab));

    if(tab == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    for (int i = 0; i < dimension; i++){
        if(i % 2 == 1 && val != CELL_WALL){
            tab[i] = val;
            val++;
        }
        else{
            tab[i] = CELL_WALL;
        }
    }
   
    return tab;
}

int **init_maze(int width, int height){

    int **maze = malloc(width * sizeof(*maze));
    int valNum = 20;
    if(maze == NULL){
        printf("Erreur d'allocation mémoire");
        exit(1);
    }
    for (int x = 0; x < width; x++){
        //si ligne impaire, on lui met une valeur en quinconce
        if(x % 2 == 1){
            maze[x] = allocate_line(height, valNum);
            valNum += height/2;
        }
        else{
            maze[x] = allocate_line(height, CELL_WALL);
        }
    }
    return maze;
}

void free_maze(int **grid, int width){
    if (grid == NULL){
        return;
    }
    for (int i = 0; i < width; i++){
        free(grid[i]);
    }
    free(grid);
}

/**
 * @brief Changer la valeur dans la grille du labyrinth
 * @param maze le pointeur du labyrinth
 * @param currentValue la valeur actuelle 
 * @param nextValue la valeur qui va être changé
 *
 */
static void change_values(Maze *maze, int currentValue, int nextValue){
    for (int x = 0; x < maze->width; x++){
        for (int y = 0; y < maze->height; y++){
            if(maze->grid[x][y] == nextValue ){
                maze->grid[x][y] = currentValue;
            }
        }
    }
}



bool fusion(Maze *maze, int y, int x, Direction direction){

    //si pas les coordonnées en retourne false
    if (maze == NULL || maze->grid == NULL ||
        is_wall_outline(maze, y, x) ||
        y % 2 == 0 || x % 2 == 0){
        return false;
    }

    int nextY = y;
    int nextX = x;

    switch (direction){
    case DIR_LEFT:
        nextX -= 2;
        break;
    case DIR_RIGHT:
        nextX += 2;
        break;
    case DIR_UP:
        nextY -= 2;
        break;
    case DIR_DOWN:
        nextY += 2;
        break;
    default:
        return false;
    }

    if (is_wall_outline(maze, nextY, nextX)){
        return false;
    }

    int currentValue = maze->grid[x][y];
    int nextValue = maze->grid[nextX][nextY];
    if (currentValue <= CELL_WALL || nextValue <= CELL_WALL || currentValue == nextValue){
        return false;
    }

    int wallY = (y + nextY) / 2;
    int wallX = (x + nextX) / 2;
    // si pas de mur entre les 2, on retourne false
    if (maze->grid[wallX][wallY] != CELL_WALL){
        return false;
    }

    maze->grid[wallX][wallY] = CELL_WAY;
    change_values(maze, currentValue, nextValue);
    return true;
}



/**
 * @brief Permet de savoir si une cellule est à l'extérieur du labyrinth
 * @param maze pointeur du labyrinth
 * @param y coordonnée y de la cellule
 * @param x coordonnée x de la cellule
 *
 * @return Renvoie un boolean designant la sortie ou non de la cellule
 */
static bool is_wall_outline(Maze *maze, int y, int x){
    if (maze == NULL || maze->grid == NULL || y < 0 || y >= maze->height || x < 0 || x >= maze->width){
        return true;
    }

    return y == 0 || y == maze->height - 1 || x == 0 || x == maze->width - 1;
}


/**
 * @brief Enonce toutes possibilités de mouvement dans chaque cellule du labyrinth 
 * @param maze pointeur du labyrinth
 * @param [out, in] length pointeur qui defini la longueur de toutes les possibilités trouvées 
 *
 * @return la listte de toutes les possibilités pour générer le labyrinth
 */
static MoveDirection *every_possibilities(Maze *maze, int *length){
    
    int rows = (maze->height-1) / 2;
    int columns = (maze->width-1) / 2;
    int capacity = 2 * (rows * (columns-1) + columns * (rows-1)); //le nombre de tentative pour avoir le laby parfait 
    MoveDirection *possibilities = malloc(capacity * sizeof(*possibilities));
    if (possibilities == NULL){
        return NULL;
    }

    *length = 0;
    for (int x = 1; x < maze->width-1; x += 2){
        for (int y = 1; y < maze->height-1; y += 2){
            if (!is_wall_outline(maze, y, x - 2)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_LEFT};
            }
            if (!is_wall_outline(maze, y, x + 2)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_RIGHT};
            }
            if (!is_wall_outline(maze, y - 2, x)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_UP};
            }
            if (!is_wall_outline(maze, y + 2, x)){
                possibilities[(*length)++] = (MoveDirection){x, y, DIR_DOWN};
            }
        }
    }

    return possibilities;
}

/**
 * @brief Enonce tous les murs du labyrinth, dans le but d'en supprimer pour le niveau difficile
 * @param maze pointeur du labyrinth
 * @param [out, in] length pointeur qui defini la longueur de toutes les possibilités trouvées 
 *
 * @return la liste des positions des murs
 */
static Position *every_possibilities_difficult_maze(Maze *maze, int *length) {
    if (maze == NULL || maze->grid == NULL || length == NULL) {
        return NULL;
    }

    //on prend toutes les longueurs du labyrinth pour avoir une taille assez grande
    int capacity = maze->height * maze->width;
    Position *positions = malloc(capacity * sizeof(*positions));
    if (positions == NULL) {
        return NULL;
    }

    *length = 0;
    for (int row = 1; row < maze->width-1; row++) {
        for (int column = 1; column < maze->height-1; column++) {
            //cellule full impaire ou full pair
            if (maze->grid[row][column] == CELL_WALL && ((row % 2 == 1 || column % 2 == 1 ) || (row % 2 == 0 || column % 2 == 0 ))) {
                positions[(*length)++] = (Position){row, column};
            }
        }
    }

    return positions;
}


void difficult_maze(Maze *maze){
    int length = 0;
    Position *every_position = every_possibilities_difficult_maze(maze, &length);
    if (every_position == NULL){
        return;
    }

    if (length == 0) {
        free(every_position);
        return;
    }

    int capacity = length / 8; //on prend 20% des positions

    for (int i = 0; i < capacity; i++){
        int random_object = rand() % capacity; //on prend au hasard dans la liste
        Position position = every_position[random_object];
        maze->grid[position.x][position.y] = CELL_WAY;
        every_position[random_object] = every_position[length-1]; // remplace la position actuelle par la dernière et on decremente de 1
        length--;
    }

    free(every_position);
}




Maze *create_maze(int width, int height){

    if(height <= 0 || width <= 0){
        printf("Les dimensions du Labyrinth doivent être positives.\n");
        return NULL;
    }

    if (height % 2 == 0 || width % 2 == 0) {
        printf("Les dimensions doivent être impaires.\n");
        return NULL;
    }

    Maze *maze = malloc(sizeof(*maze));
    if (maze == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    maze->height = height;
    maze->width = width;
    maze->name = NULL;
    maze->grid = init_maze(width, height);

    int cell_fusion_number = ((height-1) / 2) * ((width-1) / 2) - 1;
    int fusion_necessary = 0;

    int lenghtPossibilities = 0;    

    MoveDirection *possibilities = every_possibilities(maze, &lenghtPossibilities);
    if (possibilities == NULL){
        free_maze(maze->grid, maze->width);
        free(maze);
        return NULL;
    }

    // tant que la fusion necessare ne vaut pas le nombre de fusion requis, 
    // on fait les fusions avec la liste de toutes les coordonnées possible
    while(fusion_necessary != cell_fusion_number){

        int random_val = rand() % lenghtPossibilities;
        int x = possibilities[random_val].x;
        int y = possibilities[random_val].y;
        int direction = possibilities[random_val].direction;

        if(fusion(maze, y, x, direction)){
            fusion_necessary++;
        }

        // On remplace le mouvement possible recupéré par le dernier de la liste
        // pour faire la suppression 
        possibilities[random_val] = possibilities[lenghtPossibilities-1];
        lenghtPossibilities--;
    }

    //On attribut la valeur du chiffre inconnu par sa cell_way (-1)
    for (int x = 0; x < maze->width; x++) {
        for (int y = 0; y < maze->height; y++) {
            if (maze->grid[x][y] > CELL_WALL) {
                maze->grid[x][y] = CELL_WAY;
            }
        }
    }

    //On place le joueur et la sortie
    maze->grid[1][0] = CELL_PLAYER;
    maze->grid[width-2][height-1] = CELL_EXIT;

    free(possibilities);
    return maze;
}
