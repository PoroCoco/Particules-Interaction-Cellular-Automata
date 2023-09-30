#ifndef _GRAPHICS_H__
#define _GRAPHICS_H__

#include "automata.h"
#include "utils.h"

typedef struct graphics_context graphics_context;

graphics_context* graphics_init(uint32_t screen_width, uint32_t screen_height, uint32_t scale);

void graphics_render(graphics_context* context, const automata* autom);

void graphics_cleanup(graphics_context* context);


#endif // _GRAPHICS_H__
