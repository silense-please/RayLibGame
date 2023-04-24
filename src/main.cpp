#include "raylib.h"
#include "declaration.cpp"
#include "functions.cpp"


int main(void){
    // Initialization-----------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_RESIZABLE );// | FLAG_VSYNC_HINT);
    InitWindow(screen_width, screen_height, "TEST WINDOW");

    SetTargetFPS(120);

    // Load bunny texture
    Texture2D texture_bunny = LoadTexture("Game_Data/wabbit_alpha.png");


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update -----------------------------------------------------------------------

        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();
        //SetWindowSize(screen_width, screen_height);

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

        }
        //static int a = GetScreenWidth();
        //static int b = GetScreenHeight();

        if (IsKeyPressed(KEY_F)){

            toggle_borderless();
        }
        
        if (IsKeyDown(KEY_Q)){
            //ToggleFullscreen();
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            //IsWindowState();
        }
        if (IsKeyDown(KEY_W)){
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
        }


        toggle_fullscreen(); // not finished


        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);


        DrawTexture(texture_bunny, screen_width / 2 - texture_bunny.width / 2,
                    screen_height / 2 - texture_bunny.height / 2, WHITE);

        if(IsWindowFullscreen()){
            DrawText("WINDOW IS FULLSCREEN", 200, 40, 40, RED);
        }
        if(IsWindowMaximized()){
            DrawText("WINDOW IS ''MAXIMIZED''", 200, 80, 40, RED);
        }
        DrawText(TextFormat( "CURRENT MONITOR: %i", GetCurrentMonitor() +1), 200, 0, 40, RED);
        DrawText(TextFormat( "X: %i \nY: %i", int(GetWindowPosition().x),int(GetWindowPosition().y)), 200, 200, 20, DARKGRAY);


        DrawFPS(10,10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization -------------------------------------------------------------------
    UnloadTexture(texture_bunny);
    CloseWindow();        // Close window and OpenGL context

    return 0;
}

