#include <stdlib.h>
#include <stdio.h>

#include "particules.h"



particule particule_create(enum particule_type type){
    particule p = {.type = type};
    switch (type)
    {
        case TYPE_AIR:
            break;
        case TYPE_SAND:
            break;
        default:
            break;
    }
    
    return p;
}
