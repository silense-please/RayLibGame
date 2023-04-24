/*******************************************************************************************
*   raylib [textures] example - Bunnymark
*   Example originally created with raylib 1.6, last time updated with raylib 2.5
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>                 // Required for: malloc(), free()

#define MAX_BUNNIES        100000    // 50K bunnies limit

// This is the maximum amount of elements (quads) per batch
// NOTE: This value is defined in [rlgl] module and can be changed there
#define MAX_BATCH_ELEMENTS  8192

typedef struct Bunny {
    Vector2 position;
    Vector2 speed;
    Color color;
} Bunny;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [textures] example - bunnymark");

    // Load bunny texture
    Texture2D texBunny = LoadTexture("Game_Data/wabbit_alpha.png");

    Bunny *bunnies = (Bunny *)malloc(MAX_BUNNIES*sizeof(Bunny));    // Bunnies array

    int bunniesCount = 0;           // Bunnies counter

    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            // Create more bunnies
            for (int i = 0; i < 100; i++)
            {
                if (bunniesCount < MAX_BUNNIES)
                {
                    bunnies[bunniesCount].position = GetMousePosition();
                    bunnies[bunniesCount].speed.x = (float)GetRandomValue(-250, 250)/60.0f;
                    bunnies[bunniesCount].speed.y = (float)GetRandomValue(-250, 250)/60.0f;
                    bunnies[bunniesCount].color = (Color){ GetRandomValue(50, 240),
                                                           GetRandomValue(80, 240),
                                                           GetRandomValue(100, 240), 255 };
                    bunniesCount++;
                }
            }
        }

        static bool fps_locked = true;
        if (IsKeyPressed(KEY_T)){
            if (fps_locked){
                SetTargetFPS(0);
                fps_locked = false;
            }
            else{
                SetTargetFPS(120);
                fps_locked = true;
            }

            static int a = GetScreenWidth();
            static int b = GetScreenHeight();
            bool Windows_is_shit = true;
        }
        static bool vsync = false;
        if (IsKeyPressed(KEY_F)){
            //SetWindowSize(GetMonitorWidth(0),GetMonitorHeight(0));
            //SetWindowPosition(0,0);
            //SetWindowMinSize(100, 100);
            //ToggleFullscreen();
            if (vsync){
                //ToggleFullscreen();
                RestoreWindow();
                ClearWindowState(FLAG_WINDOW_UNDECORATED);
                vsync = false;
            }
            else{
                SetWindowState(FLAG_WINDOW_UNDECORATED);
                MaximizeWindow();
                //ToggleFullscreen();
                vsync = true;
            }

        }
        if (IsKeyPressed(KEY_Q)){
            SetWindowSize(800,600);
        }

        if (IsKeyPressed(KEY_ONE) && (IsKeyDown(KEY_LEFT_CONTROL))){ //this is fullscreen
            SetWindowMonitor(0);
        }
        if (IsKeyPressed(KEY_TWO) && (IsKeyDown(KEY_LEFT_CONTROL))){
            SetWindowMonitor(1);
        }
        if (IsKeyPressed(KEY_THREE) && (IsKeyDown(KEY_LEFT_CONTROL))){
            SetWindowMonitor(2);
        }



        // Update bunnies
        for (int i = 0; i < bunniesCount; i++)
        {
            bunnies[i].position.x += bunnies[i].speed.x;
            bunnies[i].position.y += bunnies[i].speed.y;

            if (((bunnies[i].position.x + texBunny.width/2) > GetScreenWidth()) ||
                ((bunnies[i].position.x + texBunny.width/2) < 0)) bunnies[i].speed.x *= -1;
            if (((bunnies[i].position.y + texBunny.height/2) > GetScreenHeight()) ||
                ((bunnies[i].position.y + texBunny.height/2 - 40) < 0)) bunnies[i].speed.y *= -1;
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < bunniesCount; i++)
        {
            // NOTE: When internal batch buffer limit is reached (MAX_BATCH_ELEMENTS),
            // a draw call is launched and buffer starts being filled again;
            // before issuing a draw call, updated vertex data from internal CPU buffer is send to GPU...
            // Process of sending data is costly and it could happen that GPU data has not been completely
            // processed for drawing while new data is tried to be sent (updating current in-use buffers)
            // it could generates a stall and consequently a frame drop, limiting the number of drawn bunnies
            DrawTexture(texBunny, (int)bunnies[i].position.x, (int)bunnies[i].position.y, bunnies[i].color);
        }

        DrawRectangle(0, 0, screenWidth, 50, BLACK);
        DrawText(TextFormat("bunnies: %i", bunniesCount), 120, 10, 20, GREEN);
        DrawText(TextFormat("batched draw calls: %i", 1 + bunniesCount/MAX_BATCH_ELEMENTS), 320, 10, 20, MAROON);

        DrawFPS(10, 10);

        EndDrawing();
    }
    // De-Initialization
    free(bunnies);              // Unload bunnies data array
    UnloadTexture(texBunny);    // Unload bunny texture
    CloseWindow();              // Close window and OpenGL context
    return 0;
}