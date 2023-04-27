#include "raylib.h"
#include "declaration.cpp"
#include "functions.cpp"


int main(void){
    // Initialization-----------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "TEST WINDOW");

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(window_width, window_height);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use


    SetTargetFPS(120);

    // Load bunny texture
    //Texture2D texture_bunny = LoadTexture("Game_Data/wabbit_alpha.png");
    Image image_bunny = LoadImage("Game_Data/wabbit_alpha.png");
    Texture2D texture_bunny = LoadTextureFromImage(image_bunny);


    int player_x = 400;
    int player_y = 200;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Process Input -----------------------------------------------------------------------
        int scaled_mouse_x = GetMouseX() / scale_x;
        int scaled_mouse_y = GetMouseY() / scale_y;

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
        if (IsKeyPressed(KEY_F)){

            toggle_borderless();
        }

        if (IsKeyDown(KEY_Q)){
            //ToggleFullscreen();
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            //IsWindowState();
        }


        int player_speed = 4;
        if (IsKeyDown(KEY_W)){player_y -= player_speed;}
        if (IsKeyDown(KEY_S)){player_y += player_speed;}
        if (IsKeyDown(KEY_A)){player_x -= player_speed;}
        if (IsKeyDown(KEY_D)){player_x += player_speed;}
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            player_x = scaled_mouse_x;
            player_y = scaled_mouse_y;
        }

        toggle_fullscreen(); // not finished

        // Game Logic -------------------------------------------------------------------




        //window content scaling


        if(IsWindowResized()) {
            window_width = GetScreenWidth();
            window_height = GetScreenHeight();
            scale_x = (float)GetScreenWidth() / initial_window_width ; // scales should be global i guess
            scale_y = (float)GetScreenHeight() / initial_window_height ;


            //texture2d_resize(&texture_bunny);// harder version of window resizing - not done - need to resize every texture - maybe overkill, not needed
        }

        // Draw -------------------------------------------------------------------------



        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        DrawTextureEx(texture_bunny, (Vector2){ (float)player_x * scale_x, (float)player_y * scale_y }, 0, 6, WHITE);

        if(IsWindowFullscreen()){
            DrawText("WINDOW IS FULLSCREEN", 200, 40, 40, RED);
        }
        if(IsWindowMaximized()){
            DrawText("WINDOW IS ''MAXIMIZED''", 200, 80, 40, RED);
        }
        DrawText(TextFormat( "CURRENT MONITOR: %i", GetCurrentMonitor()), 200, 0, 40, RED);
        //DrawText(TextFormat( "X: %i \nY: %i", int(GetWindowPosition().x),int(GetWindowPosition().y)), 200, 200, 20, DARKGRAY);


        DrawFPS(10,10);
        //draw_screen_center();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization -------------------------------------------------------------------
    UnloadTexture(texture_bunny);
    CloseWindow();        // Close window and OpenGL context

    return 0;
}


