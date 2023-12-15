#ifndef _AUTOMATA_INTERNAL_H__
#define _AUTOMATA_INTERNAL_H__

#include <stdint.h>
#include <stdbool.h>
#include "particules_internal.h"

typedef struct automata{
    uint32_t width;
    uint32_t height;
    uint32_t current_frame;
    particule *world;
    uint32_t world_size;
    bool *updated;

} automata;

#endif // _AUTOMATA_INTERNAL_H__
