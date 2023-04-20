#include "raylib.h"

bool _is_borderless = false; // this better be global value (use 'extern')
int screen_width = 800;
int screen_height = 600;


void toggle_fullscreen(){ // not finished - work in progress

    //this is fullscreen - works fine entering fullscreen but cant exit fullscreen, ToggleFullscreen works like shit
    // becauce refresh rate is not specified. Need to mess with RayLib functions(or add new) to make this work.
    // BUT this fullscreen does not stretches the content of the window
    if (IsKeyPressed(KEY_ONE) && (IsKeyDown(KEY_LEFT_CONTROL))){
        SetWindowMonitor(0);
    }
    if (IsKeyPressed(KEY_TWO) && (IsKeyDown(KEY_LEFT_CONTROL))){
        SetWindowMonitor(1);
    }
    if (IsKeyPressed(KEY_THREE) && (IsKeyDown(KEY_LEFT_CONTROL))){
        SetWindowMonitor(2);
    }
}

void toggle_borderless(){
    static Vector2 window_position; //= GetWindowPosition(); //better be global value

    if (_is_borderless){ //Restore Windowed mode
        SetWindowSize(screen_width, screen_height);
        SetWindowPosition(int(window_position.x), int(window_position.y));
        //SetWindowPosition(100,100); delete this, this is for debugging
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
    }
    else{ //Set Borderless
        window_position = GetWindowPosition(); //a bit dangerous - vectors are float and position must be presize
        Vector2 monitor_position = GetMonitorPosition(GetCurrentMonitor()); //left-upper corner of current monitor
        screen_height = GetScreenHeight(); //or GetRenderHeight()???
        screen_width = GetScreenWidth();
        //^^^ Remember window state before turning on borderless ^^^

        SetWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowPosition(int(monitor_position.x),int(monitor_position.y));
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor())); // this MUST be positive parameters
    }
    _is_borderless = !_is_borderless;
}



int main(void){
    // Initialization-----------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screen_width, screen_height, "TEST WINDOW");

    //SetTargetFPS(120);

    // Load bunny texture
    Texture2D texture_bunny = LoadTexture("Game_Data/wabbit_alpha.png");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update -----------------------------------------------------------------------

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

