#include <stdbool.h>
#include <string.h>

#include "automata.h"
#include "automata_impl.h"
#include "temperature.h"


automata* automata_init(uint32_t width, uint32_t height){
    automata *autom = malloc(sizeof(*autom));

    autom->width = width;
    autom->height = height;
    autom->current_frame = 0;
    autom->world = malloc(sizeof(*autom->world) * width * height);
    check_malloc(autom->world);
    autom->world_size = width * height;
    autom->updated = malloc(sizeof(*autom->updated) * width * height);
    check_malloc(autom->updated);
    memset(autom->updated, true, autom->world_size);

    for (uint32_t i = 0; i < autom->world_size; i++)
        autom->world[i] = particule_create(TYPE_AIR);
    

    for (size_t i = 0; i < width; i++)
        autom->world[aut_idx(autom, autom->height/4, i)] = particule_create(TYPE_SAND);
    
    for (size_t i = 0; i < height; i++)
        autom->world[aut_idx(autom, i, autom->width/2)] = particule_create(TYPE_SAND);


    return autom;
}

void automata_update(automata* autom){
    memset(autom->updated, false, autom->world_size);

    for (uint32_t i = autom->world_size -1 ; i != 0; i--){
        autom->world[i].update(autom, i);
        temperature_update(autom, i);
    }
    
    return;
}

void automata_cleanup(automata* autom){
    if (autom) free(autom->world);
    if (autom) free(autom->updated);
    free(autom);
}


uint32_t automata_get_size(const automata* autom){
    return autom->world_size;
}

enum particule_type automata_get_particule_type(const automata* autom, uint32_t index){
    assert(index < autom->world_size);
    return autom->world[index].type;
}

void automata_set_particule(const automata* autom, uint32_t index, particule p){
    assert(index < autom->world_size);
    autom->world[index] = p;
}

bool automata_index_updated(const automata* autom, uint32_t index){
    assert(index < autom->world_size);
    return autom->updated[index];
}

uint32_t automata_get_row_count(const automata* autom){
    return autom->height;
}

uint32_t automata_get_col_count(const automata* autom){
    return autom->width;
}

particule automata_get_particule(const automata* autom, uint32_t index){
    assert(index < autom->world_size);
    return autom->world[index];
}
