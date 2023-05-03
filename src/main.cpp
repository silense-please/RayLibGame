#include "raylib.h"
#include "declaration.cpp"
#include "functions.cpp"


int main(void){
    // Initialization-----------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_RESIZABLE );// |FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "TEST RAYLIB WINDOW");

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(window_width, window_height);

    //not needed?
    //SetTextureFilter(target.texture, TEXTURE_FILTER_ANISOTROPIC_16X);  // Texture scale filter to use

    SetTargetFPS(120);

    Texture2D player_texture = LoadTexture("Game_Data/player.png");
    //Image player_image = LoadImage("Game_Data/wabbit_alpha.png");
    //Texture2D player_texture = LoadTextureFromImage(player_image);
    Texture2D background_texture = LoadTexture("Game_Data/background.png");

    InitAudioDevice();
    Music music = LoadMusicStream("Game_Data/nice_music.mp3");
    music.looping = false;
    SetMusicPitch(music, 1.0);
    SetMusicVolume(music, 1);
    SetMusicPan( music, 0.5);
    PlayMusicStream(music);

    float player_x = 0;
    float player_y = 0;
    float player_speed = 0;

    // Main game loop
    while (!WindowShouldClose()){

        // Process Input -----------------------------------------------------------------------
        switch_active_gamepad();
        int scaled_mouse_x = GetMouseX() / scale_x;
        int scaled_mouse_y = GetMouseY() / scale_y;
        //UpdateMusicStream(music); // PLAY MUSIC

        player_speed = GetFrameTime() * player_acceleration;

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

        if (IsKeyPressed(KEY_F)){toggle_borderless();}

        if (IsKeyDown(KEY_W) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)){ player_y -= player_speed;}
        if (IsKeyDown(KEY_S)|| IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)){ player_y += player_speed;}
        if (IsKeyDown(KEY_A)|| IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)){ player_x -= player_speed;}
        if (IsKeyDown(KEY_D)|| IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)){ player_x += player_speed;}
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            player_x = scaled_mouse_x;
            player_y = scaled_mouse_y;
        }

        toggle_fullscreen(); // not finished

        // Game Logic -------------------------------------------------------------------




        //window content scaling


        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
        if(IsWindowResized()) {
            scale_x = (float)GetScreenWidth() / initial_window_width ; // scales should be global i guess
            scale_y = (float)GetScreenHeight() / initial_window_height ;

            //texture2d_resize(&player_texture);// harder version of window resizing - not done - need to resize every texture - maybe overkill, not needed
        }

        // Draw -------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
        ClearBackground(LIGHTGRAY);  // Clear render texture background color

        DrawTextureEx(background_texture, (Vector2){(float)0, (float)0}, 0, 1, WHITE);
        DrawTextureEx(player_texture, (Vector2){(float)player_x, (float)player_y}, 0, 2, WHITE);
        if(IsWindowFullscreen()){
            DrawText("WINDOW IS FULLSCREEN", 200, 40, 40, RED);
        }
        if(IsWindowMaximized()){
            DrawText("WINDOW IS ''MAXIMIZED''", 200, 80, 40, RED);
        }
        DrawText(TextFormat( "CURRENT MONITOR: %i", GetCurrentMonitor()), 200, 0, 40, RED);
        //DrawText(TextFormat( "X: %i \nY: %i", int(GetWindowPosition().x),int(GetWindowPosition().y)), 200, 200, 20, DARKGRAY);
        DrawFPS(10,10);
        DrawText(TextFormat( "FRAMETIME: %f", GetFrameTime() *1000), 10, 40, 20, LIME);
        //draw_screen_center(); // make this toggle
        draw_gamepads();
        gamepad_disconnect_warning();

        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw render texture to screen, properly scaled
        DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                   (Rectangle){(GetScreenWidth() - ((float)initial_window_width*scale_x))*0.5f, (GetScreenHeight() - ((float)initial_window_height*scale_y))*0.5f,
                            (float)initial_window_width*scale_x, (float)initial_window_height*scale_y }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    // De-Initialization -------------------------------------------------------------------
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadTexture(player_texture);
    UnloadTexture(background_texture);
    CloseWindow();        // Close window and OpenGL context
    return 0;
}


