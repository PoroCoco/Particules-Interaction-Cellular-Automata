#ifndef _AUTOMATA_IMPL_H__
#define _AUTOMATA_IMPL_H__

#include "automata.h"

#define aut_idx(autom, row, col) ((((autom)->width)*(row)) + (col))

typedef struct automata{
    uint32_t width;
    uint32_t height;
    uint32_t current_frame;
    particule *world;
    uint32_t world_size;
    bool *updated;

} automata;

#endif // _AUTOMATA_IMPL_H__
