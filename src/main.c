//mettre une particule d'air au millieu d'un banc de sable = remontée en diagonale gauche ? 
//update eau fausse ? eau droite gauche infini
//fixer bois qui se décale quand vapeur monte et sable/eau descend a coté.(Impression que cela arrive que sur le coté gauche pas le droit??) --> c'est bon :)

//revoir fonction ApplyVect pour faire y et x en même temps pas en boucle séparer car peut entrainer décalage de position donc donner mauvaise vélocité a mauvaise particule.

//chute libre particules pas bonne
//eau remonte descente sable et se retrouve dans endroits impossible....


#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SDL_RENDERER_SOFTWARE; SDL_RENDERER_ACCELERATED; SDL_RENDERER_PRESENTVSYNC


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define PIXEL_WIDTH 2
#define PIXEL_HEIGHT 2

#define COLONNE 500
#define LIGNE 400


#define water_speed 5

#define mat_id_air 0
#define mat_id_sable 1
#define mat_id_eau 2
#define mat_id_bois 3
#define mat_id_feu 4
#define mat_id_vapeur 5

#define bois_flammability 40

#define lifetime_wood 250
#define lifetime_feu 355
#define lifetime_vapeur 150

typedef unsigned int uint;

struct vector{
    float x;
    float y;
};
typedef struct vector vector_t;

struct color{
    uint R;
    uint G;
    uint B;
    uint ALPHA;
};
typedef struct color color_t;

color_t RED = {255,0,0,255};

// 0 = air; 1 = sable

struct particule{
    unsigned int id;
    float life_time;
    vector_t velocity;
    color_t color;
    bool has_been_updated;

};
typedef struct particule particule_t;


void SDL_ExitWithError(const char *message, SDL_Window *window, SDL_Renderer *renderer );

void fillArrayWithMat(particule_t world[LIGNE][COLONNE], uint mat_id);

void renderArray(SDL_Renderer *renderer, particule_t world[LIGNE][COLONNE], SDL_Window *window);

void setParticuleXY(particule_t world[LIGNE][COLONNE], uint mat_id, uint x, uint y);
void setParticuleXY_10by10(particule_t world[LIGNE][COLONNE], uint mat_id, uint x, uint y);

void updateArray(particule_t world[LIGNE][COLONNE]);

void updateSand(particule_t world[LIGNE][COLONNE], uint x, uint y);

bool isInWorldBoundaries(uint x, uint y);

void setArrayToUpdate(particule_t world[LIGNE][COLONNE]);

void updateWater(particule_t world[LIGNE][COLONNE], uint x, uint y);

void randomizeWorld(particule_t world[LIGNE][COLONNE]);

void updateAir(particule_t world[LIGNE][COLONNE], uint x, uint y);

void applyVect(int x, int y,particule_t world[LIGNE][COLONNE], int id);

void updateBois(particule_t world[LIGNE][COLONNE], uint x, uint y);

void updateFeu(particule_t world[LIGNE][COLONNE], uint x, uint y);

bool catchedOnFire(int flammability);

void burnAround(particule_t world[LIGNE][COLONNE],int y, int x);

void looseLife(particule_t world[LIGNE][COLONNE],int y, int x, int life_lost, int chance);

void updateVapeur(particule_t world[LIGNE][COLONNE], uint x, uint y);

void displayParticuleInfos(particule_t world[LIGNE][COLONNE], uint x, uint y);

void highlightParticule(particule_t world[LIGNE][COLONNE], uint x, uint y, int R, int G, int B);

bool isSurrounedAroundBy(particule_t world[LIGNE][COLONNE], uint x, uint y, int mat_id);

bool isSurrounedSideBy(particule_t world[LIGNE][COLONNE], uint x, uint y, int mat_id);

struct particule particule_air = { .id = mat_id_air, .life_time = 0, .velocity = {.x = 0, .y = 0}, .color = {125,125,125,255}, .has_been_updated = false};
struct particule particule_sable = { .id = mat_id_sable, .life_time = -1, .velocity = {.x = 0, .y = 0}, .color = {255,255,0,255}, .has_been_updated = false};
struct particule particule_eau = { .id = mat_id_eau, .life_time = -1, .velocity = {.x = 0, .y = 0}, .color = {0,0,255,255}, .has_been_updated = false};
struct particule particule_bois = { .id = mat_id_bois, .life_time = lifetime_wood, .velocity = {.x = 0, .y = 0}, .color = {139,69,19,255}, .has_been_updated = false};
struct particule particule_feu = { .id = mat_id_feu, .life_time = lifetime_feu, .velocity = {.x = 0, .y = 0}, .color = {200,0,0,255}, .has_been_updated = false};
struct particule particule_vapeur = { .id = mat_id_vapeur, .life_time = lifetime_vapeur, .velocity = {.x = 0, .y = 0}, .color = {220,220,220,120}, .has_been_updated = false};

int main(int argc, char ** argv){
 

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    static particule_t world[LIGNE][COLONNE];

    srand(8575); //initialise le random avec une seed
    uint elementSelected = 0;


    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_ExitWithError("Initialisation SDL",window,renderer);
    }

    //Execution programme

    window = SDL_CreateWindow("Fenêtre particules", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        SDL_ExitWithError("Creation fenetre echouee",window,renderer);
    }


    renderer = SDL_CreateRenderer(window, SDL_RENDERER_SOFTWARE, 0);
    if (renderer == NULL){
        SDL_ExitWithError("Creation rendu echouee",window,renderer);
    }


    SDL_bool program_launched = SDL_TRUE;
    //fillArrayWithMat(world,0);

    while(program_launched)
    {
        SDL_Event event;
        bool button_mousedown;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    button_mousedown = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    button_mousedown = false;
                    break;


                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        printf("Touche echappe appuiee, fermeture du programme\n");
                        program_launched = SDL_FALSE;
                        continue;
                    
                    case SDLK_a:
                        printf("appui a\n");
                        fillArrayWithMat(world,0);
                        continue;

                    case SDLK_z:
                        printf("appui z\n");
                        fillArrayWithMat(world,1);
                        continue;

                    case SDLK_w:
                        printf("appui w : élément mis = air\n");
                        elementSelected = 0;
                        continue;
                    
                    case SDLK_x:
                        printf("appui x : élément mis = sable\n");
                        elementSelected = 1;
                        continue;
                    
                    case SDLK_c:
                        printf("appui c : élément mis = eau\n");
                        elementSelected = 2;
                        continue;

                    case SDLK_r:
                        printf("appui r : Randomization du monde\n");
                        randomizeWorld(world);
                        continue;

                    case SDLK_v:
                        printf("appui v : élément mis = bois\n");
                        elementSelected = 3;
                        continue;

                    case SDLK_b:
                        printf("appui b : élément mis = feu\n");
                        elementSelected = mat_id_feu;
                        continue;

                    case SDLK_n:
                        printf("appui n : élément mis = vapeur\n");
                        elementSelected = mat_id_vapeur;
                        continue;

                    case SDLK_l:
                        //updateArray(world);
                        continue;

                    case SDLK_i:
                        printf("info mod selectionné :\n");
                        elementSelected = -1;
                        continue;

                    case SDLK_p:
                        printf("Paint mod selectionné :\n");
                        elementSelected = -2;
                        continue;

                    default:
                        continue;
                    }

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_a:
                        printf("relachement de a\n");
                        continue;
        
                    default:
                        continue;
                    }

                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;

                default:
                    break;
            }
        }
        if(button_mousedown){
            printf("Clique en %d / %d\n",event.button.x, event.button.y);
            if(elementSelected == -1){
                displayParticuleInfos(world,event.button.x/PIXEL_WIDTH,event.button.y/PIXEL_HEIGHT);
            }else if(elementSelected==-2){
                highlightParticule(world, event.button.x/PIXEL_WIDTH, event.button.y/PIXEL_HEIGHT,0,0,0);
            }else{
            setParticuleXY_10by10(world, elementSelected , event.button.x/PIXEL_WIDTH, event.button.y/PIXEL_HEIGHT);
            }
        }

        updateArray(world);
        renderArray(renderer,world,window);
        SDL_RenderPresent(renderer);
        setArrayToUpdate(world);
    }






    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

//gcc -std=c99 -Wall src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

void SDL_ExitWithError(const char *message, SDL_Window *window, SDL_Renderer *renderer ){
    printf("Erreur\n");
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void fillArrayWithMat(particule_t world[LIGNE][COLONNE], uint mat_id){
    for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            if (mat_id == mat_id_sable){
                world[y][x] = particule_sable;
            }else if(mat_id == mat_id_eau){
                world[y][x] = particule_eau;
            }else{
                world[y][x] = particule_air;
            }
        }
    }
    return;
}

void renderArray(SDL_Renderer *renderer, particule_t world[LIGNE][COLONNE], SDL_Window *window){
    for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            if(SDL_SetRenderDrawColor(renderer, world[y][x].color.R, world[y][x].color.G, world[y][x].color.B, world[y][x].color.ALPHA) != 0){
                SDL_ExitWithError("Changement couleur impossible",window,renderer);
            }
            SDL_Rect rectangle = {PIXEL_WIDTH * x, PIXEL_HEIGHT * y,PIXEL_WIDTH,PIXEL_HEIGHT};
            if(SDL_RenderFillRect(renderer, &rectangle) != 0){
                SDL_ExitWithError("Impossible de dessiner un pixel(rectangle)",window,renderer);
            }
        }
    }
}

void setParticuleXY(particule_t world[LIGNE][COLONNE], uint mat_id, uint x, uint y){
    if(isInWorldBoundaries(y,x)){
        if(mat_id == mat_id_sable){
            world[y][x] = particule_sable;
        }else if(mat_id == mat_id_air){
            world[y][x] = particule_air;
        }else if(mat_id == mat_id_eau){
            world[y][x] = particule_eau;
        }
    }
}

void updateArray(particule_t world[LIGNE][COLONNE]){
    int randomInt = rand() % 2;
    if(randomInt ==0){
        for(int y = LIGNE; y > -1; y--){
            for(int x = 0; x < COLONNE; x++){
                if(world[y][x].has_been_updated == false){
                    if(world[y][x].id == mat_id_sable){
                        updateSand(world, x, y);
                    }else if(world[y][x].id == mat_id_eau){
                        updateWater(world,x,y);
                    }else if(world[y][x].id == mat_id_bois){
                        updateBois(world,x,y);
                    }else if(world[y][x].id == mat_id_feu){
                        updateFeu(world,x,y);
                    }else if(world[y][x].id == mat_id_vapeur){
                        updateVapeur(world,x,y);
                    }
                    //}
                }
            }
        }
    }else{
        for(int y = LIGNE; y > -1; y--){
            for(int x = COLONNE; x > -1 ; x = x-1){
                if(world[y][x].has_been_updated == false){
                    if(world[y][x].id == mat_id_sable){
                        updateSand(world, x, y);
                    }else if(world[y][x].id == mat_id_eau){
                        updateWater(world, x, y);
                    }else if(world[y][x].id == mat_id_bois){
                        updateBois(world,x,y);
                    }else if(world[y][x].id == mat_id_feu){
                        updateFeu(world,x,y);
                    }else if(world[y][x].id == mat_id_vapeur){
                        updateVapeur(world,x,y);
                    }
                    
                    //}
                }
            }
        }
    }

}


void updateSand(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(isInWorldBoundaries(y+1,x) && (world[y+1][x].id == mat_id_air || world[y+1][x].id == mat_id_vapeur) ){
        world[y][x].velocity.y += 1;
        applyVect(x,y,world,1);
    }else if(isInWorldBoundaries(y+1,x) && world[y+1][x].id == mat_id_eau && world[y+1][x].has_been_updated == false){
        if(world[y][x].velocity.y > 0){
            world[y][x].velocity.y = -1;
        }
        particule_t tmp = world[y+1][x];
        world[y+1][x] = world[y][x];
        world[y][x] = tmp;
        world[y][x].has_been_updated = true;
    }else{
        int randomInt = rand() % 2;
        world[y][x].velocity.y = 0;
        if(randomInt==0){
            if(isInWorldBoundaries(y+1,x-1) && (world[y+1][x-1].id == mat_id_air || world[y+1][x-1].id == mat_id_vapeur || world[y+1][x-1].id == mat_id_eau) ){
                particule_t tmp = world[y+1][x-1];
                world[y+1][x-1] = world[y][x];
                world[y][x] = tmp;        
            }else if(isInWorldBoundaries(y+1,x+1) && (world[y+1][x+1].id == mat_id_air || world[y+1][x+1].id == mat_id_vapeur|| world[y+1][x+1].id == mat_id_eau)   ){
                particule_t tmp = world[y+1][x+1];
                world[y+1][x+1] = world[y][x];
                world[y][x] = tmp;  
            }
        }else{
            if(isInWorldBoundaries(y+1,x+1) && (world[y+1][x+1].id == mat_id_air || world[y+1][x+1].id == mat_id_vapeur || world[y+1][x+1].id == mat_id_eau) ){
                particule_t tmp = world[y+1][x+1];
                world[y+1][x+1] = world[y][x];
                world[y][x] = tmp;
            }else if(isInWorldBoundaries(y+1,x-1) && (world[y+1][x-1].id == mat_id_air || world[y+1][x-1].id == mat_id_vapeur || world[y+1][x-1].id == mat_id_eau)  ){
                particule_t tmp = world[y+1][x-1];
                world[y+1][x-1] = world[y][x];
                world[y][x] = tmp; 
            }

        }    
    }
}

bool isInWorldBoundaries(uint y, uint x){
    if(x>=0 && x<=COLONNE-1 && y>=0 && y<= LIGNE-1){
        return true;
    }
    return false;
}

void setArrayToUpdate(particule_t world[LIGNE][COLONNE]){
    for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            world[y][x].has_been_updated = false;
        }
    }
}

void updateWater(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(isInWorldBoundaries(y+1,x) && world[y+1][x].id == mat_id_bois && world[y+1][x].life_time != lifetime_wood){ //éteint le bois qui est en train de bruler;
        world[y][x] = particule_vapeur;
        world[y+1][x] = particule_bois;
        world[y+1][x].life_time = lifetime_wood;
    }
    if(isInWorldBoundaries(y+1,x) && (world[y+1][x].id == mat_id_air || world[y+1][x].id == mat_id_vapeur)){
        world[y][x].velocity.y += 1;
        world[y][x].velocity.x = world[y][x].velocity.x/2;
        applyVect(x,y,world,mat_id_eau);
    }else if(isSurrounedSideBy(world, x, y, mat_id_sable)){
        particule_t tmp = world[y-1][x];
        world[y-1][x] = world[y][x];
        world[y][x] = tmp;
    }else{
        
        if(isInWorldBoundaries(y+1,x-1) && (world[y+1][x-1].id == mat_id_air || world[y+1][x-1].id == mat_id_vapeur)){
            applyVect(x,y,world,mat_id_eau);
            world[y][x].velocity.x = 0;
            world[y+1][x-1] = world[y][x];
            world[y][x] = particule_air;  
        }else if(isInWorldBoundaries(y+1,x+1) && (world[y+1][x+1].id == mat_id_air || world[y+1][x+1].id == mat_id_vapeur)){
            applyVect(x,y,world,mat_id_eau);
            world[y][x].velocity.x = 0;
            world[y+1][x+1] = world[y][x];
            world[y][x] = particule_air;  
        }else if(isInWorldBoundaries(y,x-1) && (world[y][x-1].id == mat_id_air || world[y][x-1].id == mat_id_vapeur) && world[y][x].velocity.x <=0){
            world[y][x].velocity.x += -2;
            applyVect(x,y,world,mat_id_eau); 
        }else if(isInWorldBoundaries(y,x+1) && (world[y][x+1].id == mat_id_air  || world[y][x+1].id == mat_id_vapeur)&& world[y][x].velocity.x >=0 ){
            world[y][x].velocity.x += +2; 
            applyVect(x,y,world,mat_id_eau);
        }else{
            world[y][x].velocity.x = 0;
            world[y][x].has_been_updated = true;
        }
    }
}

void setParticuleXY_10by10(particule_t world[LIGNE][COLONNE], uint mat_id, uint x, uint y){ //5 par 5 pour l'instant
    if(isInWorldBoundaries(y,x)){
        if(mat_id == mat_id_sable){
            for(int i = -5; i<6;i = i +1){
                for(int j = -5; j<6; j = j+1){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_sable;
                    }
                }
            }
        }else if(mat_id == mat_id_air){
            for(int i = -3; i<4;i++){
                for(int j = -3; j<4;j++){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_air;
                    }
                }
            }
        }else if(mat_id == mat_id_eau){
            for(int i = -15; i<16;i++){
                for(int j = -15; j<16;j++){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_eau;
                    }
                }
            }
        }else if(mat_id == mat_id_bois){
            for(int i = -4; i<5;i++){
                for(int j = -4; j<5;j++){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_bois;
                    }
                }
            }
        }else if(mat_id == mat_id_feu){
            for(int i = -2; i<3;i++){
                for(int j = -2; j<3;j++){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_feu;
                    }
                }
            }
        }else if(mat_id == mat_id_vapeur){
            for(int i = -7; i<8;i++){
                for(int j = -7; j<8;j++){
                    if(isInWorldBoundaries(y+i,x+j)){
                        world[y+i][x+j] = particule_vapeur;
                    }
                }
            }
        }
    }
}



void randomizeWorld(particule_t world[LIGNE][COLONNE]){
    for(uint y = 0; y < LIGNE; y++){
        for(uint x = 0; x < COLONNE; x++){
            int random = rand()%8;
            if (random == mat_id_sable){
                world[y][x] = particule_sable;
            }else if(random == mat_id_eau){
                world[y][x] = particule_eau;
            }else if(random == mat_id_bois){
                world[y][x] = particule_bois;
            }else if(random == mat_id_feu){
                world[y][x] = particule_feu;
            }else if(random == mat_id_vapeur){
                world[y][x] = particule_vapeur;
            }else{
                world[y][x] = particule_air;
            }
        }
    }
}
void updateBois(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(world[y][x].life_time != lifetime_wood){
        looseLife(world, y, x, 3, 4);
        world[y][x].color.R = 255;
        world[y][x].color.G = 140;
        world[y][x].color.B = 0;
        //burningColor(world, y, x, mat_id_bois);
        burnAround(world, y, x);
    }
    if(world[y][x].life_time <=0){
        world[y][x] = particule_feu;
        world[y][x].life_time = particule_feu.life_time - lifetime_wood;
    }

}

void updateAir(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(isInWorldBoundaries(y-1, x) && world[y-1][x].id != mat_id_air){
        world[y][x] = world[y-1][x];
        world[y-1][x] = particule_air;
        world[y][x].has_been_updated = true;
    }
}

void updateFeu(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    looseLife(world, y, x, 3, 4);
    if(world[y][x].life_time<=0){  
        world[y][x] = particule_air;
    }
    if(isInWorldBoundaries(y-1,x) && world[y-1][x].id == mat_id_eau){
        world[y-1][x] = particule_vapeur; 
        world[y][x] = particule_vapeur;
        return;
    }else if(isInWorldBoundaries(y+1,x) && world[y+1][x].id == mat_id_eau){
        world[y+1][x] = particule_vapeur; 
        world[y][x] = particule_vapeur;
        return;
    }else if(isInWorldBoundaries(y,x+1) && world[y][x+1].id == mat_id_eau){
        world[y][x+1] = particule_vapeur; 
        world[y][x] = particule_vapeur;
        return;
    }else if(isInWorldBoundaries(y,x-1) && world[y][x-1].id == mat_id_eau){
        world[y][x-1] = particule_vapeur; 
        world[y][x] = particule_vapeur;
        return;
    }

    if(isInWorldBoundaries(y+1,x) && (world[y+1][x].id == mat_id_air || world[y+1][x].id == mat_id_vapeur) ){ //si rien en dessous prend de la vitesse verticale.
        if(world[y][x].velocity.y == 0){
            world[y][x].velocity.y = 1;
        }else{
            world[y][x].velocity.y += 0.2;
        }
        applyVect(x,y,world,mat_id_feu);
    }else{
    int randomInt = rand() % 2;
    world[y][x].velocity.y = 0;
        if(randomInt==0){
            if(isInWorldBoundaries(y+1,x-1) && world[y+1][x-1].id == mat_id_air){
                world[y+1][x-1] = world[y][x];
                world[y][x] = particule_air;        
            }else if(isInWorldBoundaries(y+1,x+1) && world[y+1][x+1].id == mat_id_air){
                world[y+1][x+1] = world[y][x];
                world[y][x] = particule_air;  
            }
        }else{
            if(isInWorldBoundaries(y+1,x+1) && world[y+1][x+1].id == mat_id_air){
                world[y+1][x+1] = world[y][x];
                world[y][x] = particule_air;
            }else if(isInWorldBoundaries(y+1,x-1) && world[y+1][x-1].id == mat_id_air){
                world[y+1][x-1] = world[y][x];
                world[y][x] = particule_air; 
            }

        } 
    }
    if(world[y][x].velocity.y == 0){
    burnAround(world, y, x);
    }
}

void applyVect(int x, int y,particule_t world[LIGNE][COLONNE], int id){
    int move_x = world[y][x].velocity.x;
    int move_y = world[y][x].velocity.y;
    int tmpPosOfY = y;
    if(world[y][x].velocity.x == 0 && world[y][x].velocity.y == 0 ){
        return;
    }
    int direction;
    if(move_y>0){
        direction = 1;
    }else{
        direction = -1;
    }
    move_y = move_y*direction; 
    if(id == mat_id_sable &&  move_y> 5){
        world[y][x].velocity.y = 5*direction;
    }
    else if(id == mat_id_feu &&  move_y> 2){
        world[y][x].velocity.y = 2*direction;
    }
    else if(id == mat_id_vapeur && move_y> 3){
        world[y][x].velocity.y = 3*direction;
    }
    else if(id == mat_id_eau && move_y > 6){
        world[y][x].velocity.y = 6*direction;
    }
    int newParticule_y = y;
    bool splashed = false;
    for(int i = 0; i<move_y;i++){
        if(isInWorldBoundaries(y+(1*direction)+(i*direction),x)){
            if( (world[y+(1*direction)+(i*direction)][x].id == mat_id_air || world[y+(1*direction)+(i*direction)][x].id == mat_id_vapeur)){
                newParticule_y = y+(1*direction)+(i*direction);
                tmpPosOfY = y+(1*direction)+(i*direction);
            }else if(id == 1 && world[y+(1*direction)+(i*direction)][x].id == mat_id_eau && i < 4){
                newParticule_y = y+(1*direction)+(i*direction);
                tmpPosOfY = y+(1*direction)+(i*direction);
                if(isInWorldBoundaries(y+(2*direction)+(i*direction),x) && world[y+(2*direction)+(i*direction)][x].id == mat_id_eau && isInWorldBoundaries(y+(3*direction)+(i*direction),x) && world[y+(3*direction)+(i*direction)][x].id == mat_id_eau ){
                    splashed = true;
                    break;
                }
            }else{
                break;
            }
        }
    }
    particule_t tmp = world[y][x];
    world[y][x] = world[newParticule_y][x];
    world[newParticule_y][x] = tmp;
    if(splashed == true){
        world[y][x].velocity.y = (move_y) *-1;
        if(rand()%2 == 0){
            if(isInWorldBoundaries(y,x-1) && world[y][x-1].id == mat_id_air){
                world[y][x].velocity.x -= 8;
            }else{
                world[y][x].velocity.x += 8;
            }
        }else{
            if(isInWorldBoundaries(y,x+1) && world[y][x+1].id == mat_id_air){
                world[y][x].velocity.x += 8;
            }else{
                world[y][x].velocity.x -= 8;
            }   
        }
    }


    if(move_x>0){
        direction = 1;
    }else{
        direction = -1;
    }
    move_x = move_x*direction; //transforme en valeur absolue (positif : x/1=x, négatif : -x/-1 =x)
    for(int i =0; i<move_x;i++){
        if(isInWorldBoundaries(tmpPosOfY,x+(1*direction)+(i*direction))){
            if(world[tmpPosOfY][x+(1*direction)+(i*direction)].id == mat_id_air || world[tmpPosOfY][x+(1*direction)+(i*direction)].id == mat_id_vapeur){
                particule_t tmp = world[tmpPosOfY][x+(1*direction)+(i*direction)];
                world[tmpPosOfY][x+(1*direction)+(i*direction)] = world[tmpPosOfY][x+(i*direction)];
                world[tmpPosOfY][x+(i*direction)] = tmp;
            }else{
                break;
            }
        }
        if(id == mat_id_vapeur && i== 2){
            world[tmpPosOfY][x+(1*direction)+(i*direction)].velocity.x = 2*direction;
            break;
        }
        if(i == 15){
            break;
        }
    } 
}

bool catchedOnFire(int mat_id){
    int flammability = 1;
    if(mat_id == mat_id_bois){
        flammability = bois_flammability;
    }
    return (rand()%flammability == 2);
}

void burnAround(particule_t world[LIGNE][COLONNE],int y, int x){
    if(isInWorldBoundaries(y+1,x) && catchedOnFire(world[y+1][x].id)){
        world[y+1][x].life_time --;
    }if(isInWorldBoundaries(y+1,x-1) && catchedOnFire(world[y+1][x-1].id)){
        world[y+1][x-1].life_time --;
    }if(isInWorldBoundaries(y+1,x+1) && catchedOnFire(world[y+1][x+1].id)){
        world[y+1][x+1].life_time --;
    }if(isInWorldBoundaries(y,x-1) && catchedOnFire(world[y][x-1].id)){
        world[y][x-1].life_time --;
    }if(isInWorldBoundaries(y,x+1) && catchedOnFire(world[y][x+1].id)){
        world[y+1][x+1].life_time --;
    }if(isInWorldBoundaries(y-1,x-1) && catchedOnFire(world[y-1][x-1].id)){
        world[y-1][x-1].life_time --;
    }if(isInWorldBoundaries(y-1,x) && catchedOnFire(world[y-1][x].id)){
        world[y-1][x].life_time --;
    }if(isInWorldBoundaries(y-1,x+1) && catchedOnFire(world[y-1][x+1].id)){
        world[y-1][x+1].life_time --;
    }
}

void looseLife(particule_t world[LIGNE][COLONNE],int y, int x, int life_lost, int chance){
    if(rand()%chance == 1){
        world[y][x].life_time = world[y][x].life_time - life_lost; 
    }
}

void updateVapeur(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(isInWorldBoundaries(y+1,x)&&isInWorldBoundaries(y-1,x) && world[y+1][x].id == mat_id_air && world[y-1][x].id == mat_id_vapeur){
        looseLife(world, y, x, 30, 5);
    }
    looseLife(world, y, x, 12, 20);
    if(world[y][x].life_time <=0){
        world[y][x] = particule_air;
        return;
    }
    if(isInWorldBoundaries(y-1, x) && world[y-1][x].id == mat_id_air){
        particule_t tmp = world[y-1][x];
        world[y-1][x] = world[y][x];
        world[y][x] = tmp;
    }else if(rand()%3==1){
        
        if(isInWorldBoundaries(y-1,x-1) && world[y-1][x-1].id == mat_id_air){
            world[y][x].velocity.x = 0;
            applyVect(x,y,world,mat_id_vapeur);
            particule_t tmp = world[y-1][x-1];
            world[y-1][x-1] = world[y][x];
            world[y][x] = tmp;  
        }else if(isInWorldBoundaries(y-1,x+1) && world[y-1][x+1].id == mat_id_air){
            world[y][x].velocity.x = 0;
            applyVect(x,y,world,mat_id_vapeur);
            particule_t tmp = world[y-1][x+1];
            world[y-1][x+1] = world[y][x];
            world[y][x] = tmp;  
        }else{
            if(isInWorldBoundaries(y,x-1) && world[y][x-1].id == mat_id_air && world[y][x].velocity.x <=0){
                world[y][x].velocity.x += -1;
                applyVect(x,y,world,mat_id_vapeur); 
            }else if(isInWorldBoundaries(y,x+1) && world[y][x+1].id == mat_id_air && world[y][x].velocity.x >=0 ){
                world[y][x].velocity.x += +1; 
                applyVect(x,y,world,mat_id_vapeur);
            }else{
                world[y][x].velocity.x = 0;
            }
        }
    }
}

void displayParticuleInfos(particule_t world[LIGNE][COLONNE], uint x, uint y){
    if(isInWorldBoundaries(y,x)){
    printf("id = %d\n",world[y][x].id);
    printf("lifetime = %f\n",world[y][x].life_time);
    printf("velocity : x = %f, y = %f\n",world[y][x].velocity.x,world[y][x].velocity.y);
    printf("has been updated = %d\n",world[y][x].has_been_updated);
    }
}

void highlightParticule(particule_t world[LIGNE][COLONNE], uint x, uint y, int R, int G, int B){
    if(isInWorldBoundaries(y,x)){
        world[y][x].color.R = R;
        world[y][x].color.G = G;
        world[y][x].color.B = B;
    }
}

bool isSurrounedAroundBy(particule_t world[LIGNE][COLONNE], uint x, uint y, int mat_id){
    bool surronded = true;
    if(isInWorldBoundaries(y-1,x-1) && world[y-1][x-1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y-1,x) && world[y-1][x].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y-1,x+1) && world[y-1][x+1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y,x-1) && world[y][x-1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y,x+1) && world[y][x+1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y+1,x-1) && world[y+1][x-1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y+1,x) && world[y+1][x].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y+1,x+1) && world[y+1][x+1].id != mat_id){
        surronded = false;
    }
    return surronded;
}

bool isSurrounedSideBy(particule_t world[LIGNE][COLONNE], uint x, uint y, int mat_id){
    bool surronded = true;
    if(isInWorldBoundaries(y-1,x) && world[y-1][x].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y,x-1) && world[y][x-1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y,x+1) && world[y][x+1].id != mat_id){
        surronded = false;
    }if(isInWorldBoundaries(y+1,x) && world[y+1][x].id != mat_id){
        surronded = false;
    }
    return surronded;
}