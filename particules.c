#include <stdlib.h>
#include <stdio.h>

#include "particules.h"



particule particule_create(enum particule_type type){
    particule p = {.type = type, .temperature = rand()%300};
    switch (type)
    {
        case TYPE_AIR:
            break;
        case TYPE_SAND:
            break;
        case TYPE_HEATER:
            p.temperature = PART_HEATER_TEMP;
            break;
        case TYPE_COOLER:
            p.temperature = PART_COOLER_TEMP;
            break;
            
        default:
            break;
    }
    
    return p;
}
