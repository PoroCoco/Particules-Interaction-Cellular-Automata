#include <stdlib.h>
#include <stdio.h>

#include "input_handler.h"
#include "particules.h"


void input_spawn_particules(automata *autom, Vector2 mouse_pos, size_t scale){
    size_t radius = 20;
    uint32_t row = (mouse_pos.y / scale ) - radius;
    uint32_t col = (mouse_pos.x / scale) - radius;

    for (size_t i = 0; i < radius; i++)
    {
        for (size_t j = 0; j < radius; j++)
        {
            automata_set_particule(autom, automata_get_col_count(autom)*(row+j) + (col+i), particule_create(TYPE_SAND));
        }
        
    }
    


}