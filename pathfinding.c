#include "pathfinding.h"

void updateExplorer(particule_t world[LIGNE][COLONNE], uint x, uint y){
    if(rand()%1 == 0){
        world[y][x].has_been_updated = true;
        g_start_x = x;
        g_start_y = y;
        int target_x,target_y;
        int nextMove_x, nextMove_y;
        bool b = getTargetPos(world,&target_x, &target_y);
        
        if(b){
            g_target_x = target_x;
            g_target_y = target_y;
            b = getNextMoveTo(world, x, y, target_x,target_y, &nextMove_x, &nextMove_y);
            if (b && (nextMove_x != target_x || nextMove_y != target_y)){
                //printf("moving to x,y : %d,%d\n",nextMove_x,nextMove_y);
                particule_t tmp = world[nextMove_y][nextMove_x];
                world[nextMove_y][nextMove_x] = world[y][x];
                world[y][x] = tmp;
            }else{
                //printf("couldn't move :(\n");
            }
        }else{
           // printf("Couldn't find a target for the explorer\n");
        }
    }
}

//cherche les coordonnées de la target, renvoie true si possible, false sinon. Les coordonnées sont appliqués aux pointeurs.
bool getTargetPos(particule_t world[LIGNE][COLONNE], int * target_x, int * target_y){
    /*for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            if(world[y][x].color.R == TARGET_R && world[y][x].color.G == TARGET_G && world[y][x].color.B == TARGET_B){
                *target_x = x;
                *target_y = y;
                return true;
            }
        }
    }*/
    if(SDL_GetMouseState(target_x,target_y) && GOTOPOS == 1 && world[*target_y/PIXEL_WIDTH][*target_x/PIXEL_WIDTH].id == mat_id_air){
        *target_x = *target_x/PIXEL_WIDTH;
        *target_y = *target_y/PIXEL_HEIGHT;
        return true;
    }
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
        node_t Voisins[5];
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
    if(isInWorldBoundaries(current_node.y-1,current_node.x)){
        G_cost = getDistance(current_node.x, current_node.y-1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x, current_node.y-1, g_target_x, g_target_y);
        node_t node = {current_node.x, current_node.y-1 , G_cost, H_cost, G_cost + H_cost};
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
    }if(isInWorldBoundaries(current_node.y+1,current_node.x)){
        G_cost = getDistance(current_node.x, current_node.y+1, g_start_x, g_start_y);
        H_cost = getDistance(current_node.x, current_node.y+1, g_target_x, g_target_y);
        node_t node = {current_node.x, current_node.y+1, G_cost, H_cost, G_cost + H_cost};
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
    static node_t path[MAX_PATH_SIZE];
    node_t endNodeArray = {COLONNE+1,COLONNE+1,COLONNE+1,COLONNE+1,COLONNE+1,{COLONNE+1,COLONNE+1}};
    path[0] = endNodeArray;
    while(current.parent.x != 0 && current.parent.y != 0){
        setAdd(path,current);
        if (HIGHLIGHT_PATH == 1){
            highlightParticule(world, current.x,current.y,255,0,0);
        }
        current = set[getIndexOfParentByCoord(set, current.parent.x, current.parent.y)];
        if(current.x == g_start_x && current.y == g_start_y ){
            break;
            
        }
    }
    displaySetSize(path);
    int index;
    bool b;
    b= getIndexOfLastElement(path,&index);
    if(b && index != 0){
        *nextMove_x = path[index-1].x;
        *nextMove_y = path[index-1].y; 
    }else{
        *nextMove_x = g_start_x;
        *nextMove_y = g_start_y;
        printf("last index element impossible retracePath\n");
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

void displaySetSize(node_t set[]){
    int cpt = 0;
    for(int i = 0 ; set[i].x != COLONNE+1 ;i++){
        cpt ++;
    }
    printf("path size : \"%d\" \n",cpt);
}


//Permet de generer un labyrinthe surface -> commençant aux coordonnées x,y et se terminant en x+width, y+height | return true si maze peut etre generer, false sinon
bool generateMaze(particule_t world[LIGNE][COLONNE], uint start_x, uint start_y, uint width, uint height, uint mat_id){
    if(!(isInWorldBoundaries(start_y+height,start_x+width))){
        printf("maze is too big for this world\n");
        return false;
    }
    if(start_y%2 == 1){
        start_y -= 1; 
    }if(start_x%2 == 1){
        start_x -= 1; 
    }
    if(isInWorldBoundaries(start_y,start_x) && isInWorldBoundaries(start_y+height,start_x+width)){
        maze_start_x = start_x;
        maze_start_y = start_y;
        maze_end_x = start_x+width;
        maze_end_y = start_y+height;
        if(maze_end_y%2 == 1){
            maze_end_y -= 1; 
        }if(maze_end_x%2 == 1){
            maze_end_x -= 1; 
        }
    }else{
        return false;
    }
    if(!(isInWorldBoundaries(maze_end_y,maze_end_x))){
        printf("maze is too big for this world\n");
        return false;
    }

    for(int i = start_x; i<=maze_end_x ;i++){
        if(isInWorldBoundaries(maze_start_y,i) && isInWorldBoundaries(maze_end_y,i) ){
            world[maze_start_y][i] = particule_bois;
            world[maze_end_y][i] = particule_bois;
        }else{
            return false;
        }
    }
    for(int i = start_y; i<= maze_end_y; i++){
        if(isInWorldBoundaries(i,maze_start_x) && isInWorldBoundaries(i,maze_end_x) ){
            world[i][maze_start_x] = particule_bois;
            world[i][maze_end_x] = particule_bois;
        }else{
            return false;
        }
    }
    for(int y = start_y+1; y<maze_end_y ; y++){
        for(int x = start_x +1; x<maze_end_x ; x++){
            world[y][x] = particule_bois;
        }
    }
    world[start_y+1][start_x+1] = particule_air;
    if(!(carve_passage(world,start_x+1,start_y+1))){
        return false;
    }
    return true;
}

bool carve_passage(particule_t world[LIGNE][COLONNE], uint x, uint y){
    int direction[] = {1,2,3,4};
    randomizeDirection(direction);
    for(int i = 0; i<4; i++){
        if(direction[i] == 1 && isInMazeBoundaries(y-2,x) && world[y-2][x].id == mat_id_bois){
            world[y-1][x] = particule_air;
            world[y-2][x] = particule_air;
            carve_passage(world,x,y-2);
        }if(direction[i] == 2 && isInMazeBoundaries(y,x+2) && world[y][x+2].id == mat_id_bois){
            world[y][x+1] = particule_air;
            world[y][x+2] = particule_air;
            carve_passage(world,x+2,y);
        }if(direction[i] == 3 && isInMazeBoundaries(y+2,x) && world[y+2][x].id == mat_id_bois){
            world[y+1][x] = particule_air;
            world[y+2][x] = particule_air;
            carve_passage(world,x,y+2);
        }if(direction[i] == 4 && isInMazeBoundaries(y,x-2) && world[y][x-2].id == mat_id_bois){
            world[y][x-1] = particule_air;
            world[y][x-2] = particule_air;
            carve_passage(world,x-2,y);
        }
    }
    return true;
}

void randomizeDirection(int tab[]){
    int tmp;
    for(int i = 4-1; i>0; i--){
        int j = rand()%(i+1);
        tmp = tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
    }
}

bool isInMazeBoundaries(uint y, uint x){    
    if(x>=maze_start_x && x<=maze_end_x-1 && y>=maze_start_y && y<= maze_end_y-1){
        return true;
    }
    return false;
}