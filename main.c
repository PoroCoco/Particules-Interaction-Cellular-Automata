#include <stdint.h>
#include <stdio.h>

#include "automata.h"
#include "graphics.h"
#include "raylib.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    uint32_t height = 1080;
    uint32_t width = 1920;
    uint32_t scale = 1;
    automata* autom = automata_init(width/scale, height/scale);
    
    graphics_context* context = graphics_init(width, height, scale);


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        TIME_BLOCK_EXEC("render", graphics_render(context, autom););
        
        TIME_BLOCK_EXEC("update", automata_update(autom););
    }
    
    automata_cleanup(autom);
    graphics_cleanup(context);


    return 0;
}