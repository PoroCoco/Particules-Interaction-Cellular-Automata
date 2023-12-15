#include <stdlib.h>
#include <stdio.h>

#include "automata_internal.h"
#include "particules_internal.h"
#include "particules.h"

void update_air(automata *autom, uint32_t index){
    return;
}


void update_sand(automata *autom, uint32_t index){
    if(autom->world[index].type == TYPE_SAND){
        if (index + autom->width < autom->world_size && autom->world[index + autom->width].type == TYPE_AIR){
            particule tmp = autom->world[index + autom->width];
            autom->world[index + autom->width] = autom->world[index];
            autom->world[index] = tmp;
            autom->updated[index] = true;
            autom->updated[index + autom->width] = true;

        }else if(index + autom->width + 1 < autom->world_size && autom->world[index + autom->width + 1].type == TYPE_AIR){
            particule tmp = autom->world[index + autom->width + 1];
            autom->world[index + autom->width + 1] = autom->world[index];
            autom->world[index] = tmp;
            autom->updated[index] = true;
            autom->updated[index + autom->width + 1] = true;

        }else if(index + autom->width - 1 < autom->world_size && autom->world[index + autom->width - 1].type == TYPE_AIR){
            particule tmp = autom->world[index + autom->width - 1];
            autom->world[index + autom->width - 1] = autom->world[index];
            autom->world[index] = tmp;
            autom->updated[index] = true;
            autom->updated[index + autom->width - 1] = true;

        }
    }
}

particule particule_create(enum particule_type type){
    particule p = {.type = type};
    switch (type)
    {
        case TYPE_AIR:
            p.update = update_air;
            break;
        case TYPE_SAND:
            p.update = update_sand;
            break;
        default:
            break;
    }
    
    return p;
}
