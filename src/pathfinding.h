#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "particules.h"
#include "config.h"

//nombre maximum de node explorée.
#define OPEN_CLOSE_SET_MAX_SIZE 20000
//Taille maximum du chemin retracé une fois le chemin trouvé.
#define MAX_PATH_SIZE 25000

int GOTOPOS = 0;

//getTargetPos commence depuis coord 0,0 donc premier trouver est a partir de en haut a gauche, modifier fonction pour regarder depuis autour des coordonnées de l'explorer.
//Voir pour eviter que pathfinding sorte de la grille du world.


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


int g_start_x;
int g_start_y;
int g_target_x;
int g_target_y;


void updateExplorer(particule_t world[LIGNE][COLONNE], uint x, uint y){

    if(rand()%1 == 0){
        g_start_x = x;
        g_start_y = y;
        int target_x,target_y;
        int nextMove_x, nextMove_y;
        bool b = getTargetPos(world,&target_x, &target_y);
        
        if(b){
            g_target_x = target_x;
            g_target_y = target_y;
            b = getNextMoveTo(world, x, y, target_x,target_y, &nextMove_x, &nextMove_y);
            /*if (b){
                particule_t tmp = world[nextMove_y][nextMove_x];
                world[nextMove_y][nextMove_x] = world[y][x];
                world[y][x] = tmp;
            }else{
                //printf("couldn't move :(\n");
            }*/
        }else{
           // printf("Couldn't find a target for the explorer\n");
        }
    }
}

//cherche les coordonnées de la target, renvoie true si possible, false sinon. Les coordonnées sont appliqués aux pointeurs.
bool getTargetPos(particule_t world[LIGNE][COLONNE], int * target_x, int * target_y){
    for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            if(world[y][x].color.R == TARGET_R && world[y][x].color.G == TARGET_G && world[y][x].color.B == TARGET_B){
                *target_x = x;
                *target_y = y;
                return true;
            }
        }
    }
    /*if(SDL_GetMouseState(target_x,target_y) && GOTOPOS == 1){
        *target_x = *target_x/PIXEL_WIDTH;
        *target_y = *target_y/PIXEL_HEIGHT;
        return true;
    }*/
    return false;
}


//dans openSet et closedSet, la node de coordonées x = COLONNE+1 correspond à la fin du tableau
bool getNextMoveTo(particule_t world[LIGNE][COLONNE], uint start_x, uint start_y, uint target_x, uint target_y, int * nextMove_x, int * nextMove_y){
    bool searching = true;
    static node_t openSet[OPEN_CLOSE_SET_MAX_SIZE];
    static node_t closedSet[OPEN_CLOSE_SET_MAX_SIZE]; 
    node_t arrayEndNode = {COLONNE+1,COLONNE+1};
    openSet[0] = arrayEndNode;
    closedSet[0] = arrayEndNode;
    bool b ;
    node_t node_start = {start_x, start_y , 0, getDistance(start_x, start_y, target_x, target_y), getDistance(start_x, start_y, target_x, target_y)};
    b = setAdd(openSet, node_start);
    if (!b){
        printf("Can't add first node in getNextMoveTo\n");
        return false;
    }
    while(searching){
        node_t current = openSet[getIndexOfLowestF(openSet)];

        b = setRemove(openSet, current.x, current.y);
        if (!b){
            //printf("couldn't remove node in getNextMoveTo\n");
            return false;
        }
        b = setAdd(closedSet,current);
        if(!b){
            //printf("couldn't add node in getNextMoveTo\n");
            return false;
        }

        if (current.x == target_x && current.y == target_y){ //si chemin trouvé 
            retracePath(world, closedSet, current, nextMove_x, nextMove_y);
            return true;
        }

        //pour chaque voisins de current
        node_t Voisins[9];
        getVoisins(Voisins, current);
        for(int i = 0; Voisins[i].x != COLONNE+1; i++){
            if(world[Voisins[i].y][Voisins[i].x].id != mat_id_air || existInSet(closedSet, Voisins[i].x, Voisins[i].y)){ //si non traversable ou existe dans le set de node closed alors skip itération de la boucle.
                continue;
            }
            //highlightParticule(world, Voisins[i].x, Voisins[i].y, 255,0,0 );
            int newMoveCostToVoisin = current.G_cost + getDistance(current.x, current.y, Voisins[i].x, Voisins[i].y);
            if( newMoveCostToVoisin < Voisins[i].G_cost || existInSet(openSet, Voisins[i].x, Voisins[i].y) == false){
                Voisins[i].G_cost = newMoveCostToVoisin;
                coord_t parent = {current.x, current.y};
                Voisins[i].parent = parent;

                if(existInSet(openSet, Voisins[i].x, Voisins[i].y) == false){
                    setAdd(openSet,Voisins[i]);
                }
            }
        }

    }
    return false;
}

//return la distance entre 2 coordonées. 10 = déplacement droite/gauche/haut/bas, 14 = déplacement diagonale.
int getDistance(uint start_x, uint start_y, uint target_x, uint target_y){
    int distance_X = abs(start_x - target_x);
    int distance_Y = abs(start_y - target_y);

    if (distance_X > distance_Y){
        return 14*distance_Y + 10* (distance_X - distance_Y);
    }
    return 14* distance_X + 10*(distance_Y - distance_X);
}

//retourn l'index de la node avec le F_cost le plus bas dans l'array. Si multiple F_cost égaux, return le premier.
int getIndexOfLowestF(node_t tab[]){
    int index = 0;
    for(int i = 1; tab[i].x != COLONNE+1; i++){
        if(tab[i].F_cost < tab[index].F_cost){
            index  = i;
        } 
    }
    return index;
}

//Ajoute une node à la fin de l'array.
bool setAdd(node_t tab[], node_t nodeToAdd){
    if(tab == NULL){
        return false;
    }
    int index;
    bool b = getIndexOfLastElement(tab,&index);
    if (b){
        tab[index+1] = tab[index];
        tab[index] = nodeToAdd;
        return true;
    }else{
        //printf("Function setAdd couldn't work\n");
        return false;
    }
}

//parcours array et cherche l'index de la première node au coordonnées x = LIGNES +1 qui signifie la fin du tableau. renvoie true si possible, false sinon. L'index est assigné depuis un pointeur.
bool getIndexOfLastElement(node_t tab[], int * index){
    if(tab == NULL || index == NULL){
        printf("tab/index is NULL in getIndexOfLastElement\n");
        return false;
    }
    *index = 0;
    for(int i = 0; i<OPEN_CLOSE_SET_MAX_SIZE; i++){
        if(tab[i].x == COLONNE+1){
            *index = i;
            return true;
        }
    }
    printf("Node set is biggger than OPEN_CLOSE_SET_MAX_SIZE-1, in function getIndexOfLastElement\n");
    return false;
}

//parcours l'array et supprime la node aux coordonnées x, y. Décale tout l'array pour remplir le trou. Renvoie booléen en fonction du succès.
bool setRemove(node_t tab[], uint x, uint y){
    if(tab == NULL){
        return false;
    }
    for(int i = 0; tab[i].x != COLONNE+1; i++){
        if(tab[i].x == x && tab[i].y == y){
            for(; tab[i].x != COLONNE+1; i++){
                tab[i] = tab[i+1];
            }
            return true;
        }
    }
    //printf("setRemove didn't work, no idea why :)\n");
    return false;
}

//regarde chaque voisins de la node donné et retourne un tableau de tout ces voisins (s'ils existent(pas hors world)) dont les G,H,F cost sont calculés. 
void getVoisins(node_t tab[], node_t current_node){
    int H_cost;
    int G_cost;
    int index = 0;
    if(isInWorldBoundaries(current_node.y-1,current_node.x-1)){
        G_cost = getDistance(current_node.x-1, current_node.y-1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x-1, current_node.y-1, g_target_x, g_target_y);
        node_t node = {current_node.x-1, current_node.y-1 , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y-1,current_node.x)){
        G_cost = getDistance(current_node.x, current_node.y-1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x, current_node.y-1, g_target_x, g_target_y);
        node_t node = {current_node.x, current_node.y-1 , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y-1,current_node.x+1)){
        G_cost = getDistance(current_node.x+1, current_node.y-1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x+1, current_node.y-1, g_target_x, g_target_y);
        node_t node = {current_node.x+1, current_node.y-1 , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y,current_node.x-1)){
        G_cost = getDistance(current_node.x-1, current_node.y, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x-1, current_node.y, g_target_x, g_target_y);
        node_t node = {current_node.x-1, current_node.y, G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y,current_node.x+1)){
        G_cost = getDistance(current_node.x+1, current_node.y, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x+1, current_node.y, g_target_x, g_target_y);
        node_t node = {current_node.x+1, current_node.y , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y+1,current_node.x-1)){
        G_cost = getDistance(current_node.x-1, current_node.y+1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x-1, current_node.y+1, g_target_x, g_target_y);
        node_t node = {current_node.x-1, current_node.y+1 , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y+1,current_node.x)){
        G_cost = getDistance(current_node.x, current_node.y+1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x, current_node.y+1, g_target_x, g_target_y);
        node_t node = {current_node.x, current_node.y+1, G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }if(isInWorldBoundaries(current_node.y+1,current_node.x+1)){
        G_cost = getDistance(current_node.x+1, current_node.y+1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x+1, current_node.y+1, g_target_x, g_target_y);
        node_t node = {current_node.x+1, current_node.y+1 , G_cost, H_cost, G_cost + H_cost};
        tab[index] = node;
        index += 1;
    }
    node_t endNode = {COLONNE+1, COLONNE +1,LIGNE,LIGNE,LIGNE};
    tab[index] = endNode;
}

//return true si la node au coordonnées x,y existe dans l'array de type node_t donné. False sinon
bool existInSet(node_t tab[], uint x, uint y){
    for(int i = 0; tab[i].x != COLONNE+1; i++){
        if(tab[i].x == x && tab[i].y == y){
            return true;
        }
    }
    return false;
}


void retracePath(particule_t world[LIGNE][COLONNE], node_t set[], node_t endNode, int * nextMove_x, int * nextMove_y){
    node_t current = endNode;
    node_t path[MAX_PATH_SIZE];
    node_t endNodeArray = {COLONNE+1,COLONNE+1,COLONNE+1,COLONNE+1,COLONNE+1,{COLONNE+1,COLONNE+1}};
    path[0] = endNodeArray;
    while(current.parent.x != 0 && current.parent.y != 0){
        setAdd(path,current);
        highlightParticule(world, current.x,current.y,255,0,0);
        current = set[getIndexOfParentByCoord(set, current.parent.x, current.parent.y)];
        if(current.x == g_start_x && current.y == g_start_y ){
            break;
            
        }
        
        /**nextMove_x = current.x;
        *nextMove_y = current.y; */
    }
}

//return l'index de la node aux coordonnées x,y dans l'array, -1 sinon 
int getIndexSetByCoord(node_t tab[], uint x, uint y){
    for(int i =0; tab[i].x != COLONNE+1; i++){
        if(tab[i].x == x && tab[i].y == y){
            return i;
        }
    }
    printf("Couldn't find coord in GetIndexSetByCoord\n");
    return -1;
}

//return l'index de la node dont les coordonnées de la node parent sont égales a x,y
int getIndexOfParentByCoord(node_t tab[], uint x, uint y){
    for(int i =0; tab[i].x != COLONNE+1; i++){
        if(i == MAX_PATH_SIZE){
            printf("Path is too long, MAX_PATH_SIZE set is %d, change it in the pathfinding.h file !",MAX_PATH_SIZE);
            exit(EXIT_FAILURE);
        }
        if(tab[i].x == x && tab[i].y == y){
            return i;
        }
    }   
    printf("Couldn't find parent coord in GetIndexOfParentByCoord\n");
    exit(EXIT_FAILURE);
}

//print des infos pour chaque éléments de l'array
void displaySet(node_t tab[]){
    for(int i =0; tab[i].x != COLONNE+1; i++){
        printf("%d : x = %d, y = %d\n",i,tab[i].x,tab[i].y );
        printf("%d : parent :  x = %d, y = %d\n",i,tab[i].parent.x,tab[i].parent.y );
    }
}
