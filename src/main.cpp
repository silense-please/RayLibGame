#include "raylib.h"
#include "declaration.cpp"
#include "settings.cpp"
#include "load.cpp"
#include "input.cpp"
#include "collision.cpp"
#include "draw.cpp"

int main(void){

    // Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE );// |FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "TEST RAYLIB WINDOW");

    // Render texture initialization - to hold the rendering result be able to resize it
    RenderTexture2D target = LoadRenderTexture(window_width, window_height);

    SetTargetFPS(TARGET_FPS);

    Texture2D player_texture = LoadTexture("Game_Data/player_64p.png");
    //Image player_image = LoadImage("Game_Data/wabbit_alpha.png");
    //Texture2D player_texture = LoadTextureFromImage(player_image);
    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");

    Player player;

    Game_Level current_level;
    load_level(current_level);
    load_player_spawn(current_level, player);

    //InitAudioDevice();
    //Music music = LoadMusicStream("Game_Data/nice_music.mp3");
    //music.looping = false;
    //PlayMusicStream(music);


    // Main game loop
    while (!WindowShouldClose()){
        //UpdateMusicStream(music); // PLAY MUSIC

        // Process Input ----------------------------------------------------------------
        process_input(player);

        // Game Logic -------------------------------------------------------------------

        if(!player.is_standing && !player.is_levitating && player.speed_y > 0){
            player.falling_time += GetFrameTime();
        }

        player.speed_x = GetFrameTime() * (player.acceleration_right - player.acceleration_left);
        player.speed_y = player.falling_time * (GRAVITATION - player.acceleration_up) * GetFrameTime(); // @Херня - переделывай

        static_object_collision(player, current_level);

        player.x += player.speed_x;
        player.y += player.speed_y;

        level_borders_collision(player);
        apply_screen_scale();

        BeginTextureMode(target); // Draw everything to target texture to enable window scaling/stretching
        {   // Draw
            ClearBackground(LIGHTGRAY);  // Clear render texture background color

            DrawTextureEx(background_texture, (Vector2) {(float) 0, (float) 0}, 0, 1, WHITE);
            DrawTextureEx(player_texture, (Vector2) {(float) player.x, (float) player.y}, 0, 1, WHITE);

            static_objects_draw(current_level, ground_texture, 'G');
            if (_draw_debug_info) draw_debug_info();

            //@Temporary - move to draw debug info
            DrawText(TextFormat("standing: %d", player.is_standing), 10, 40, 20, LIME);
            DrawText(TextFormat("floating: %d", player.is_levitating), 10, 120, 20, LIME);
            DrawText(TextFormat("falling_time: %f ", player.falling_time), 10, 80, 20, LIME);
            show_collision(player,
                             current_level); //@Temporary - maybe move to draw_debug_info after collision system is finished
            // -------------
            DrawFPS(10,10);
        }
        EndTextureMode();

        draw_target_texture(target);
    }

    // De-Initialization -------------------------------------------------------------------
    //UnloadMusicStream(music);
    //CloseAudioDevice();
    UnloadTexture(player_texture);
    UnloadTexture(ground_texture);
    UnloadTexture(background_texture);
    CloseWindow();        // Close window and OpenGL context
    printf("Errors: %i", total_errors);
    return 0;
}
