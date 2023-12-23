#include "declaration.cpp"

#include "settings.cpp" //these includes are either in main or at the bottom of declaration
#include "load.cpp"
#include "input.cpp"
#include "collision.cpp"
#include "draw.cpp"

int main(void){

    // Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); // |FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "RAYLIB-DEMO");
    //SetMouseCursor(0);

    // Render texture initialization - to hold the rendering result be able to resize it
    RenderTexture2D target = LoadRenderTexture(window_width, window_height);

    Image game_icon = LoadImage("Game_Data/player_64p.png");
    SetWindowIcon(game_icon);
    SetExitKey(KEY_END);
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(120);

    Texture2D player_texture = LoadTexture("Game_Data/player_64p.png");
    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");

    Player player;

    Game_Level current_level;
    load_level(current_level);
    load_player_spawn(current_level, player);

    Camera2D camera = {(Vector2){(float)window_width /2, (float)window_height /2},
                       (Vector2){ (float)window_width /2, (float)window_height /2 },0,1};


    const int MENU_BUTTONS = 2;
    Menu_Button menu_btns[MENU_BUTTONS] {{BTN_RESUME,"RESUME",(float)window_width/2 -125, 200, },
                                         {BTN_QUIT,  "QUIT",  (float)window_width/2 -125, 400, } };

    //InitAudioDevice();
    //Music music = LoadMusicStream("Game_Data/nice_music.mp3");
    //music.looping = false;
    //PlayMusicStream(music);


    bool close_window = 0;

    /// Main game loop
    while (!WindowShouldClose() && !close_window){
        //UpdateMusicStream(music); // PLAY MUSIC

        /// Process Input ----------------------------------------------------------------
        process_input(player, camera);

        if(_is_menu){
            for (int i = 0; i < MENU_BUTTONS; ++i) {
                if (CheckCollisionPointRec((Vector2) {GetMouseX() / scale_x, GetMouseY() / scale_y},
                                           (Rectangle) {menu_btns[i].x, menu_btns[i].y, menu_btns[i].width,
                                                        menu_btns[i].height})) {
                    menu_btns[i].state = 1;
                    if (IsButtonReleased(BUTTON_LMB)) {
                        switch (menu_btns[i].id) {
                            case BTN_RESUME: _is_menu = 0; _is_paused = 0; break;
                            case BTN_QUIT: close_window = 1; break;
                        }
                    }
                } else menu_btns[i].state = 0;
            }
        }

        /// Game Logic -------------------------------------------------------------------
        if(_is_paused) delta_time = 0;
        else delta_time = GetFrameTime() * GAME_SPEED;

        if(!player.is_standing && !player.is_levitating && player.speed_y >= 0){ //&& player.speed_y >= 0
            player.falling_time += delta_time;
        } else player.falling_time = INITIAL_FALLING_TIME;

        if (!player.is_standing) player.air_time += delta_time;
        else player.air_time = 0;

        if (player.is_standing){player.is_holding_jump = false; player.is_jumping = false;player.jump_input_hold_time = 0;}




        player.speed_x += delta_time * (player.acceleration_right - player.acceleration_left);
        player.speed_y += delta_time * (player.acceleration_down - player.acceleration_up);


        static_object_collision_by_speed(player, current_level);


        if (player.speed_y > 2000) {player.speed_y = 2000;} //@Define max speed

        player.x += player.speed_x * delta_time;
        player.y += player.speed_y * delta_time;
        //static_object_collision_by_coordinates(player, current_level);


        level_borders_collision(player, current_level);


        camera.zoom += ((float)GetMouseWheelMove()*0.1f * camera.zoom);
        if(!free_cam) // Camera just follows player for now
            camera.target = (Vector2){ player.x + player.width/2, player.y + player.height/2};



        BeginTextureMode(target);{ /// Draw everything to target texture to enable window scaling/stretching
            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);{
                DrawTextureEx(background_texture, (Vector2) {(float) 0, (float) 0}, 0, 3, WHITE);
                DrawTextureEx(player_texture, (Vector2) {(float) player.x - player.width/2, (float) player.y}, 0, 1, WHITE);
                static_objects_draw(current_level, ground_texture, 'G');
            } EndMode2D();

            if (_is_menu) {
                BeginBlendMode(2);
                DrawRectangle(0, 0, window_width, window_height, (Color) {0, 0, 0, 70});//darken the game when menu opened
                EndBlendMode();

                for (int i = 0; i < MENU_BUTTONS; ++i) {
                    DrawRectangle(menu_btns[i].x, menu_btns[i].y, menu_btns[i].width, menu_btns[i].height,
                                  (menu_btns[i].state == 0) ? menu_btns[i].color : LIME);
                    DrawText(menu_btns[i].label, menu_btns[i].x, menu_btns[i].y, 40, BLACK);
                }
            }

            if (_is_paused) DrawText("PAUSED", 500, 50, 60, LIME);

            if (_draw_debug_info) draw_debug_info(); //@Maybe just put func contents here

            //@Temporary - move to draw debug info
            DrawRectangle(0,0,250,400,Color{0,0,0,180}); // Debug info opacity
            DrawText(TextFormat("standing: %d", player.is_standing), 10, 40, 20, LIME);
            DrawText(TextFormat("falling_time: %f ", player.falling_time), 10, 80, 20, LIME);
            DrawText(TextFormat("jump hold t: %f", player.jump_input_hold_time), 10, 120, 20, LIME);
            DrawText(TextFormat("jump hold b: %d", player.is_holding_jump), 10, 160, 20, LIME);
            DrawText(TextFormat("air time: %f", player.air_time), 10, 200, 20, LIME);
            DrawText(TextFormat("Speed Y: %f", player.speed_y), 10, 240, 20, LIME);
            if (free_cam) DrawText("FREE CAMREA", 700, 100, 40, DARKGREEN);
            show_collision(player,current_level); //@Temporary - maybe move to draw_debug_info after collision system is finished

            // -------------
            DrawFPS(10,10);
        } EndTextureMode();
        draw_target_texture(target);

    } ///---Main loop

    /// De-Initialization -------------------------------------------------------------------
    //UnloadMusicStream(music);
    //CloseAudioDevice();
    UnloadTexture(player_texture);
    UnloadTexture(ground_texture);
    UnloadTexture(background_texture);
    UnloadRenderTexture(target);
    CloseWindow();
    printf("Errors: %i", total_errors);
    return 0;
}
