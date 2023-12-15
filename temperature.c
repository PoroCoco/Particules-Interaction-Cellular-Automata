#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#include "temperature.h"

void temperature_update(automata* autom, uint32_t index){
    assert(index < autom->world_size);
    if (index+1+autom->width >= autom->world_size || index-autom->width-1 >= autom->world_size){
        return;
    }
    particule *w = autom->world;
    uint32_t width = autom->width;
    w[index].temperature =  (4*(w[index+width].temperature + w[index-width].temperature + w[index+1].temperature + w[index-1].temperature)
                             + w[index-width-1].temperature + w[index-width+1].temperature + w[index+width-1].temperature + w[index+width+1].temperature
                            ) / 20;

    // If emitter then keep standard value
    if (w[index].type == TYPE_HEATER){
        w[index].temperature = PART_HEATER_TEMP;
    }else if (w[index].type == TYPE_COOLER){
        w[index].temperature = PART_COOLER_TEMP;
    }
}