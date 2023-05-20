#include "raylib.h"
#include "declaration.cpp"
#include "load.cpp"
#include "input.cpp"
#include "draw.cpp"

int main(void){
    // Initialization-----------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_RESIZABLE );// |FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "TEST RAYLIB WINDOW");

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(window_width, window_height);

    //not needed?
    //SetTextureFilter(target.texture, TEXTURE_FILTER_ANISOTROPIC_16X);  // Texture scale filter to use

    SetTargetFPS(120);

    Texture2D player_texture = LoadTexture("Game_Data/player_64p.png");
    //Image player_image = LoadImage("Game_Data/wabbit_alpha.png");
    //Texture2D player_texture = LoadTextureFromImage(player_image);
    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");

    Player player;

    Game_Level level_0;
    load_level(level_0);
    load_player_spawn(level_0, player);

    InitAudioDevice();
    Music music = LoadMusicStream("Game_Data/nice_music.mp3");
    music.looping = false;
    SetMusicPitch(music, 1.0);
    SetMusicVolume(music, 1);
    SetMusicPan(music, 0.5);
    PlayMusicStream(music);


    // Main game loop
    while (!WindowShouldClose()){
        window_width = GetScreenWidth();
        window_height = GetScreenHeight();
        //UpdateMusicStream(music); // PLAY MUSIC

        // Process Input ----------------------------------------------------------------
        process_input(player);

        // Game Logic -------------------------------------------------------------------



        // Draw -------------------------------------------------------------------------
        apply_screen_scale();

        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
            ClearBackground(LIGHTGRAY);  // Clear render texture background color

            DrawTextureEx(background_texture, (Vector2){(float)0, (float)0}, 0, 1, WHITE);
            DrawTextureEx(player_texture, (Vector2){(float)player.x, (float)player.y}, 0, 2, WHITE);

            static_objects_draw(level_0, ground_texture, 'G');
            if (_draw_debug_info) draw_debug_info();

            DrawFPS(10,10);
        EndTextureMode();

        draw_target_texture(target);
    }

    // De-Initialization -------------------------------------------------------------------
    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadTexture(player_texture);
    UnloadTexture(ground_texture);
    UnloadTexture(background_texture);
    CloseWindow();        // Close window and OpenGL context
    printf("errors: %i", total_errors);
    return 0;
}
