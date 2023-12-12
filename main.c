#include <stdint.h>
#include <stdio.h>

#include "automata.h"
#include "graphics.h"
#include "input_handler.h"
#include "raylib.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    uint32_t height = 1080;
    uint32_t width = 1920;
    uint32_t scale = 4;
    automata* autom = automata_init(width/scale, height/scale);
    
    graphics_context* context = graphics_init(width, height, scale);

    //render every particule once
    TIME_BLOCK_EXEC("render", graphics_render(context, autom););


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) input_spawn_particules(autom, GetMousePosition(), scale);
        if (IsKeyPressed(KEY_N)) input_cycle_current_particule();
        
        TIME_BLOCK_EXEC("update", automata_update(autom););
        TIME_BLOCK_EXEC("render", graphics_render(context, autom););


    }
    
    automata_cleanup(autom);
    graphics_cleanup(context);


    return 0;
}