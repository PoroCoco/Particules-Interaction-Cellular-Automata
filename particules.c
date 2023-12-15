#include <stdlib.h>
#include <stdio.h>

#include "automata_impl.h"
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

void update_heater(automata *autom, uint32_t index){
    return;
}

void update_cooler(automata *autom, uint32_t index){
    return;
}


particule particule_create(enum particule_type type){
    particule p = {.type = type, .temperature = rand()%300};
    switch (type)
    {
        case TYPE_AIR:
            p.update = update_air;
            break;
        case TYPE_SAND:
            p.update = update_sand;
            break;
        case TYPE_HEATER:
            p.update = update_heater;
            p.temperature = PART_HEATER_TEMP;
            break;
        case TYPE_COOLER:
            p.update = update_cooler;
            p.temperature = PART_COOLER_TEMP;
            break;
            
        default:
            break;
    }
    
    return p;
}
