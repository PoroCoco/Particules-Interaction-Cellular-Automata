#ifndef _AUTOMATA_H__
#define _AUTOMATA_H__

#include <stdbool.h>
#include "particules.h"
#include "utils.h"

typedef struct automata automata;

automata* automata_init(uint32_t width, uint32_t height);

void automata_update(automata* autom);

void automata_cleanup(automata* autom);

uint32_t automata_get_size(const automata* autom);

enum particule_type automata_get_particule_type(const automata* autom, uint32_t index);

bool automata_index_updated(const automata* autom, uint32_t index);

#endif // _AUTOMATA_H__
