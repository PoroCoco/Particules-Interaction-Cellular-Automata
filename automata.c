#include "automata.h"
#include "particules.h"
#include "utils.h"

#define autom_idx(autom, row, col) ((((autom)->width)*(row)) + (col))


typedef struct automata{
    uint32_t width;
    uint32_t height;
    uint64_t current_frame;
    particule *world;


} automata;

automata* automata_init(uint32_t width, uint32_t height){
    automata *autom = malloc(sizeof(*autom));

    autom->width = width;
    autom->height = height;
    autom->current_frame = 0;
    autom->world = malloc(sizeof(*autom->world) * width * height);
    check_malloc(autom->world);


    return autom;
}

void automata_update(automata* autom);

void automata_cleanup(automata* autom){
    free(autom);
}