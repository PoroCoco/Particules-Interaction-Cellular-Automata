#include <stdlib.h>
#include <stdio.h>

#include "automata_impl.h"
#include "particules_internal.h"
#include "particules.h"

void particule_switch(automata *autom, uint32_t index1, uint32_t index2){
    particule tmp = autom->world[index2];
    autom->world[index2] = autom->world[index1];
    autom->world[index1] = tmp;
    autom->updated[index1] = true;
    autom->updated[index2] = true;
}

void update_air(automata *autom, uint32_t index){
    return;
}

void update_glass(automata *autom, uint32_t index){
    return;
}

void update_water(automata *autom, uint32_t index){
    if (index + autom->width < autom->world_size && autom->world[index + autom->width].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width);
    }else if(index + autom->width + 1 < autom->world_size && autom->world[index + autom->width + 1].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width + 1);
    }else if(index + autom->width - 1 < autom->world_size && autom->world[index + autom->width - 1].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width - 1);
    }else{
        if (rand()%2 == 0){
            if (index + 1 < autom->world_size && autom->world[index + 1].type == TYPE_AIR){
                particule_switch(autom, index, index + 1);
            }else if (index - 1 < autom->world_size && autom->world[index - 1].type == TYPE_AIR){
                particule_switch(autom, index, index - 1);
            }
        }else{
            if (index - 1 < autom->world_size && autom->world[index - 1].type == TYPE_AIR){
                particule_switch(autom, index, index - 1);
            }else if (index + 1 < autom->world_size && autom->world[index + 1].type == TYPE_AIR){
                particule_switch(autom, index, index + 1);
            }
        }
    }
}


void update_sand(automata *autom, uint32_t index){
    if (autom->world[index].temperature >= 400){
        autom->world[index].type = TYPE_GLASS;
        autom->world[index].update = update_glass;
        return;
    }

    if (index + autom->width < autom->world_size && autom->world[index + autom->width].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width);
    }else if(index + autom->width + 1 < autom->world_size && autom->world[index + autom->width + 1].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width + 1);
    }else if(index + autom->width - 1 < autom->world_size && autom->world[index + autom->width - 1].type == TYPE_AIR){
        particule_switch(autom, index, index + autom->width - 1);
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
        case TYPE_GLASS:
            p.update = update_glass; 
            break;
        case TYPE_WATER:
            p.update = update_water; 
            break;
        default:
            break;
    }
    
    return p;
}
