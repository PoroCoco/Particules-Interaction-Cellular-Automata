#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "particules.h"
#include "config.h"

//nombre maximum de node explorée.
#define OPEN_CLOSE_SET_MAX_SIZE 200000
//Taille maximum du chemin retracé une fois le chemin trouvé.
#define MAX_PATH_SIZE 100000


#define HIGHLIGHT_PATH 1
int GOTOPOS = 0;

//getTargetPos commence depuis coord 0,0 donc premier trouver est a partir de en haut a gauche, modifier fonction pour regarder depuis autour des coordonnées de l'explorer.
//Voir pour eviter que pathfinding sorte de la grille du world.
//si target x,y(0,0) ou x = COLLONE, y=0, impossible de path


struct coord{
    int x;
    int y;
};
typedef struct coord coord_t;


struct node{
    int x;
    int y;
    int G_cost;
    int H_cost;
    int F_cost;
    coord_t parent;
};
typedef struct node node_t;



void updateExplorer(particule_t world[LIGNE][COLONNE], uint x, uint y);
bool getTargetPos(particule_t world[LIGNE][COLONNE], int * target_x, int * target_y);
bool getNextMoveTo(particule_t world[LIGNE][COLONNE], uint start_x, uint start_y, uint target_x, uint target_y, int * nextMove_x, int * nextMove_y);
int getDistance(uint start_x, uint start_y, uint target_x, uint target_y);
bool existInSet(node_t tab[], uint x, uint y);
void getVoisins(node_t tab[], node_t current_node);
bool setRemove(node_t tab[], uint x, uint y);
bool getIndexOfLastElement(node_t tab[], int * index);
bool setAdd(node_t tab[], node_t nodeToAdd);
int getIndexOfLowestF(node_t tab[]);
int getIndexOfParentByCoord(node_t tab[], uint x, uint y);
int getIndexSetByCoord(node_t tab[], uint x, uint y);
void retracePath(particule_t world[LIGNE][COLONNE], node_t set[], node_t endNode, int * nextMove_x, int * nextMove_y);
void displaySet(node_t tab[]);
void displaySetSize(node_t set[]);

bool generateMaze(particule_t world[LIGNE][COLONNE], uint start_x, uint start_y, uint width, uint height, uint mat_id);
void randomizeDirection(int tab[]);
bool carve_passage(particule_t world[LIGNE][COLONNE], uint x, uint y);
bool isInMazeBoundaries(uint y, uint x);

int g_start_x;
int g_start_y;
int g_target_x;
int g_target_y;

int maze_start_y;
int maze_start_x;
int maze_end_y;
int maze_end_x;


