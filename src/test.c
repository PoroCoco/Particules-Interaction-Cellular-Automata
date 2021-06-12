#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

 
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

struct particule{
    unsigned int id;
    float life_time;
    bool has_been_updated;
};
typedef struct particule particule_t;


int main(int argc, char **argv){
    printf("entered main\n");
    struct particule particule_sable = { .id = 1, .life_time = 0, .has_been_updated = false};
    struct particule particule_air = { .id = 0, .life_time = 0, .has_been_updated = false};
    printf("inited struct particules\n");
    static particule_t world[WINDOW_WIDTH][WINDOW_HEIGHT];
    printf("inited world\n");
    world[2][0].id = particule_sable.id;
    world[4][1].id = particule_air.id;
    printf("inited world indexes\n");
    printf("4,1 : %d  2,0 : %d",world[4][1].id, world[2][0].id);
}