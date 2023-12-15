#include "automata.h"
#include "particules.h"
#include "graphics.h"
#include "raylib.h"

typedef struct graphics_context{
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t scale;
    Texture2D texture;
    char * texture_bytes;

} graphics_context;


graphics_context* graphics_init(uint32_t screen_width, uint32_t screen_height, uint32_t scale){
    assert(screen_width%scale == 0);
    assert(screen_height%scale == 0);
    SetTraceLogLevel(LOG_WARNING); 

    InitWindow(screen_width, screen_height, "raylib [core] example - basic window");

    SetTargetFPS(144);               // Set our game to run at 60 frames-per-second

    graphics_context * context = malloc(sizeof(*context));
    check_malloc(context);
    context->screen_width = screen_width;
    context->screen_height = screen_height;
    context->scale = scale;

    Image img = GenImageColor(context->screen_width / scale, context->screen_height / scale, WHITE);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    context->texture = LoadTextureFromImage(img);
    UnloadImage(img);


    context->texture_bytes = malloc(sizeof(*context->texture_bytes) * screen_width * screen_height * 4 / scale);
    check_malloc(context->texture_bytes);

    return context;
}

uint8_t temperature_to_red(float temperature){
    if (temperature < 0.0f) {
        temperature = 0.0f;
    } else if (temperature > 500.0f) {
        temperature = 500.0f;
    }

    float normalizedValue = temperature / 500.0f;

    uint8_t result = (uint8_t)(normalizedValue * 255.0f);

    return result;
}

void graphics_render(graphics_context* context, const automata* autom){
    BeginDrawing();

    // Color red = {.a = 255, .r = 255, .g = 0, .b = 0};


    ClearBackground(RAYWHITE);
    uint64_t world_size = automata_get_size(autom);
    for (uint64_t i = 0; i < world_size; i++)
    {
        // if (!automata_index_updated(autom, i)) continue;
        uint32_t index = i * 4;

        enum particule_type t = automata_get_particule_type(autom, i);
        switch (t)
        {
            case TYPE_AIR:
                context->texture_bytes[index] = temperature_to_red(automata_get_particule(autom, i).temperature);
                context->texture_bytes[index+1] = 0;
                context->texture_bytes[index+2] = 0;
                context->texture_bytes[index+3] = 255;
                break;
            case TYPE_SAND:
                context->texture_bytes[index] = 253;
                context->texture_bytes[index+1] = 249;
                context->texture_bytes[index+2] = 0;
                context->texture_bytes[index+3] = 255;
                break;
            case TYPE_HEATER:
                context->texture_bytes[index] = temperature_to_red(automata_get_particule(autom, i).temperature);
                context->texture_bytes[index+1] = 64;
                context->texture_bytes[index+2] = 125;
                context->texture_bytes[index+3] = 255;
                break;
            case TYPE_COOLER:
                context->texture_bytes[index] = temperature_to_red(automata_get_particule(autom, i).temperature);
                context->texture_bytes[index+1] = 0;
                context->texture_bytes[index+2] = 0;
                context->texture_bytes[index+3] = 255;
                break;
        }
    }


    UpdateTexture(context->texture, context->texture_bytes);
    DrawTextureEx(context->texture, (Vector2){ 0, 0 }, 0.0f, context->scale, WHITE);

    DrawFPS(10, 10);

    EndDrawing();
}

void graphics_cleanup(graphics_context* context){
    CloseWindow();        // Close window and OpenGL context

    UnloadTexture(context->texture);
    free(context->texture_bytes);
    free(context);
}



