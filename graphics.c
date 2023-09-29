#include "graphics.h"
#include "utils.h"
#include "raylib.h"

typedef struct graphics_context{
    uint32_t id;
} graphics_context;


graphics_context* graphics_init(void);

void graphics_render(graphics_context* context, const void *simulation){
       // Initialization
    const int screenHeight = 720;
    const int screenWidth = 1280;
    SetTraceLogLevel(LOG_WARNING); 

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(screenWidth/2 - 40, screenHeight/2 - 40, 80, 80, GREEN);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}

void graphics_cleanup(graphics_context* context);



