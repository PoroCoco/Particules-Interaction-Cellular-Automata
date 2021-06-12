#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SDL_RENDERER_SOFTWARE; SDL_RENDERER_ACCELERATED; SDL_RENDERER_PRESENTVSYNC

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


void SDL_ExitWithError(const char *message, SDL_Window *window, SDL_Renderer *renderer );

int main(int argc, char ** argv){

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;


    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_ExitWithError("Initialisation SDL",window,renderer);
    }

    //Execution programme

    window = SDL_CreateWindow("Fenêtre particules", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        SDL_ExitWithError("Creation fenetre echouee",window,renderer);
    }


    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL){
        SDL_ExitWithError("Creation rendu echouee",window,renderer);
    }


    if(SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("Changement couleur impossible",window,renderer);
    }

    if(SDL_RenderDrawPoint(renderer,100,450) != 0){
        SDL_ExitWithError("Impossible de dessiner un point",window,renderer);
    }

    if(SDL_RenderDrawLine(renderer,10,200, 500,500) != 0){
        SDL_ExitWithError("Impossible de dessiner une ligne",window,renderer);
    }

    SDL_Rect rectangle;
    rectangle.x = 100;
    rectangle.y = 200;
    rectangle.w = 100;
    rectangle.h = 100;

    if(SDL_RenderDrawRect(renderer, &rectangle ) != 0){
        SDL_ExitWithError("Impossible de dessiner un rectangle",window,renderer);
    }

    






    SDL_RenderPresent(renderer);

    SDL_Delay(3000);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

//gcc -std=c99 -Wall src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2

void SDL_ExitWithError(const char *message, SDL_Window *window, SDL_Renderer *renderer ){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
}


        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        printf("Touche echappe appuiee, fermeture du programme\n");
                        program_launched = SDL_FALSE;
                        continue;
                    
                    case SDLK_a:
                        printf("appui a\n");
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
        }*




//working sand before Y velocity
void updateSand(particule_t world[LIGNE][COLONNE], uint x, uint y){
    world[y][x].has_been_updated = true;
    if(isInWorldBoundaries(y+1,x) && world[y+1][x].id == 0){
        world[y+1][x] = world[y][x];
        world[y][x] = particule_air;
    }else{
        int randomInt = rand() % 2;

        if(randomInt==0){
            if(isInWorldBoundaries(y+1,x-1) && world[y+1][x-1].id == 0){
                world[y+1][x-1] = world[y][x];
                world[y][x] = particule_air;        
            }else if(isInWorldBoundaries(y+1,x+1) && world[y+1][x+1].id == 0){
                world[y+1][x+1] = world[y][x];
                world[y][x] = particule_air;  
            }
        }else{
            if(isInWorldBoundaries(y+1,x+1) && world[y+1][x+1].id == 0){
                world[y+1][x+1] = world[y][x];
                world[y][x] = particule_air;
            }else if(isInWorldBoundaries(y+1,x-1) && world[y+1][x-1].id == 0){
                world[y+1][x-1] = world[y][x];
                world[y][x] = particule_air; 
            }

        }    
    }
}


        int randomInt = rand() % 2;
        world[y][x].velocity.y = 0;
        if(randomInt==0){
            if(isInWorldBoundaries(y+1,x-1) && (world[y+1][x-1].id == mat_id_air || world[y+1][x-1].id == mat_id_vapeur || world[y+1][x-1].id == mat_id_eau) && world[y][x-1].id != mat_id_sable && world[y][x-1].velocity.y == 0 ){
                particule_t tmp = world[y+1][x-1];
                world[y+1][x-1] = world[y][x];
                world[y][x] = tmp;        
            }else if(isInWorldBoundaries(y+1,x+1) && (world[y+1][x+1].id == mat_id_air || world[y+1][x+1].id == mat_id_vapeur|| world[y+1][x+1].id == mat_id_eau) && world[y][x+1].id != mat_id_sable && world[y][x+1].velocity.y == 0 ){
                particule_t tmp = world[y+1][x+1];
                world[y+1][x+1] = world[y][x];
                world[y][x] = tmp;  
            }
        }else{
            if(isInWorldBoundaries(y+1,x+1) && (world[y+1][x+1].id == mat_id_air || world[y+1][x+1].id == mat_id_vapeur || world[y+1][x+1].id == mat_id_eau) && world[y][x+1].id != mat_id_sable && world[y][x+1].velocity.y == 0  ){
                particule_t tmp = world[y+1][x+1];
                world[y+1][x+1] = world[y][x];
                world[y][x] = tmp;
            }else if(isInWorldBoundaries(y+1,x-1) && (world[y+1][x-1].id == mat_id_air || world[y+1][x-1].id == mat_id_vapeur || world[y+1][x-1].id == mat_id_eau) && world[y][x-1].id != mat_id_sable && world[y][x-1].velocity.y == 0 ){
                particule_t tmp = world[y+1][x-1];
                world[y+1][x-1] = world[y][x];
                world[y][x] = tmp; 
            }

        }