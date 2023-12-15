#ifndef _INPUT_HANDLER_H__
#define _INPUT_HANDLER_H__

#include "raylib.h"
#include "automata.h"


void input_spawn_particules(automata *autom, Vector2 mouse_pos, size_t scale);
void input_cycle_current_particule(void);

#endif // _INPUT_HANDLER_H__
