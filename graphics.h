#ifndef _GRAPHICS_H__
#define _GRAPHICS_H__


typedef struct graphics_context graphics_context;

graphics_context* graphics_init(void);

void graphics_render(graphics_context* context, const void *simulation);

void graphics_cleanup(graphics_context* context);


#endif // _GRAPHICS_H__
