#include "automata.h"

#define aut_idx(autom, row, col) ((((autom)->width)*(row)) + (col))


typedef struct automata{
    uint32_t width;
    uint32_t height;
    uint32_t current_frame;
    particule *world;
    uint32_t world_size;


} automata;

automata* automata_init(uint32_t width, uint32_t height){
    automata *autom = malloc(sizeof(*autom));

    autom->width = width;
    autom->height = height;
    autom->current_frame = 0;
    autom->world = malloc(sizeof(*autom->world) * width * height);
    check_malloc(autom->world);
    autom->world_size = width * height;

    for (uint32_t i = 0; i < autom->world_size; i++)
        autom->world[i] = particule_create(TYPE_AIR);
    

    for (size_t i = 0; i < width; i++)
        autom->world[aut_idx(autom, 10, i)] = particule_create(TYPE_SAND);
    
    for (size_t i = 0; i < height; i++)
        autom->world[aut_idx(autom, i, 500)] = particule_create(TYPE_SAND);


    return autom;
}

void automata_update(automata* autom){
    for (uint32_t i = autom->world_size -1 ; i != 0; i--)
    {
    
        if(autom->world[i].type == TYPE_SAND){
            if (i + autom->width < autom->world_size && autom->world[i + autom->width].type == TYPE_AIR){
                particule tmp = autom->world[i + autom->width];
                autom->world[i + autom->width] = autom->world[i];
                autom->world[i] = tmp;
            }else if(i + autom->width + 1 < autom->world_size && autom->world[i + autom->width + 1].type == TYPE_AIR){
                particule tmp = autom->world[i + autom->width + 1];
                autom->world[i + autom->width + 1] = autom->world[i];
                autom->world[i] = tmp;
            }else if(i + autom->width - 1 < autom->world_size && autom->world[i + autom->width - 1].type == TYPE_AIR){
                particule tmp = autom->world[i + autom->width - 1];
                autom->world[i + autom->width - 1] = autom->world[i];
                autom->world[i] = tmp;
            }
        }
    }
    return;
}

void automata_cleanup(automata* autom){
    if (autom) free(autom->world);
    free(autom);
}


uint32_t automata_get_size(const automata* autom){
    return autom->world_size;
}

enum particule_type automata_get_particule_type(const automata* autom, uint32_t index){
    return autom->world[index].type;
}