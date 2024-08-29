#include "declaration.cpp"

#include "settings.cpp" //these includes are either in main or at the bottom of declaration
#include "load.cpp"
#include "input.cpp"
#include "collision.cpp"
#include "draw.cpp"
#include "debugging.cpp"


int main(void){


    /// Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE /*| FLAG_MSAA_4X_HINT*/ | FLAG_WINDOW_ALWAYS_RUN ); // |FLAG_VSYNC_HINT);
    InitWindow(game_screen_width, game_screen_height, "RAYLIB-DEMO");
    //SetMouseCursor(0);

    // Render texture initialization - to hold the rendering result and be able to resize it
    RenderTexture2D render_texture = LoadRenderTexture(game_screen_width, game_screen_height);
    SetTextureWrap(render_texture.texture, TEXTURE_WRAP_CLAMP);
    //toggle_borderless(); // Set borderless at startup


    Image game_icon = LoadImage("Game_Data/game_icon.png");
    SetWindowIcon(game_icon);
    SetExitKey(KEY_END);
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(TARGET_FPS);

    Texture2D player_texture = LoadTexture("Game_Data/animations/player_64p.png"); // Placeholder player tex.
    enum Animations // I don't want to pollute global scope, but if you need it outside of main - move it to declaration.
    {
        PLAYER_RUN,
        PLAYER_IDLE,
        PLAYER_JUMP,
        PLAYER_FALL,
        PLAYER_FALL_START,
        PLAYER_JUMP_START,
        PLAYER_LANDED,

    };

    // I tried grouping animations without map, just with array, but it was a clusterfuck.
    // @ I don't like using unfamiliar expressins - so find out how hash table work (or whatever unordered map is) 
    //and get rid of STL map, do we even need to optimise this with hash table? Maybe just Array of Structs? (as JBlow advised)

    // Use .at() instead of [] ??   ( [] operator generates new map member if it didn't find one)
    //https://www.youtube.com/watch?v=KiB0vRi2wlc
    std::unordered_map<unsigned int, Animation> anims_player  {
            {PLAYER_RUN,        {LoadTexture("Game_Data/animations/player_run.png"), 0.07, true}},
            {PLAYER_IDLE,       {LoadTexture("Game_Data/animations/player_idle.png"), 0.4, true}},
            {PLAYER_JUMP,       {LoadTexture("Game_Data/animations/player_jump.png"), 0.1, true}},
            {PLAYER_FALL,       {LoadTexture("Game_Data/animations/player_fall.png"),0.1, true}},
            {PLAYER_FALL_START, {LoadTexture("Game_Data/animations/player_fall_start.png"),0.06, false}},
            {PLAYER_JUMP_START, {LoadTexture("Game_Data/animations/player_jump_start.png"), 0.1, false}},
            {PLAYER_LANDED,     {LoadTexture("Game_Data/animations/player_fall_end.png"),0.05, false}},
    };
    // Default wrapping (repeat) distorts edges of sprites when they're scaled imperfectly (e.g. on camera zoom = 1.6).
    for ( auto& [name, anim] : anims_player) { //@ NO NEED ANYMORE? FIXED?
        SetTextureWrap(anim.texture, TEXTURE_WRAP_CLAMP);
    }

    //@TESTING
    //SetTextureFilter(anims_player[PLAYER_IDLE].texture, TEXTURE_FILTER_ANISOTROPIC_16X);
    //SetTextureFilter(render_texture.texture, TEXTURE_FILTER_BILINEAR);


    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");
    //SetTextureWrap(ground_texture, TEXTURE_WRAP_CLAMP);
    //SetTextureFilter(ground_texture, TEXTURE_FILTER_BILINEAR);


    Player player;
    Game_Level current_level;
    load_level(current_level);
    load_player_spawn(current_level, player);

    Camera2D camera =
        {(Vector2){(float)game_screen_width / 2, (float)game_screen_height / 2},
         (Vector2){(float)game_screen_width / 2, (float)game_screen_height / 2 }, 0, DEFAULT_ZOOM};

    const int MENU_BUTTONS = 2;
    Menu_Button menu_btns[MENU_BUTTONS]
            {
                    {BTN_RESUME,"RESUME", (float)game_screen_width / 2 - 125, 200, },
                    {BTN_QUIT,  "QUIT",   (float)game_screen_width / 2 - 125, 400, }
            };

    InitAudioDevice();
    Music music = LoadMusicStream("Game_Data/test_music.mp3");
    music.looping = false;
    PlayMusicStream(music);

    bool close_window = 0;

    double measure_time_start = GetTime();
    double average_measured_time = 0; //@ Move to global?? (to be able to measure inside funcs)

    while (!WindowShouldClose() && !close_window){ /// Main game loop
        average_measured_time = measure_time(measure_time_start); //Leave these two lines here if you want to measure whole frame time
        measure_time_start = GetTime(); // This is debugging - for measure_time func.


        //UpdateMusicStream(music); // PLAY MUSIC

        if(_is_paused) delta_time = 0;
        else delta_time = GetFrameTime() * GAME_SPEED; //@Add speed change ingame for debugging (with mousewheel)
        //else delta_time = 0.008 * GAME_SPEED;//!!! THIS IS FOR BREAKPOINT DEBUGGING ONLY

        /// --- Process Input ---
        if(_is_menu){
            for (int i = 0; i < MENU_BUTTONS; ++i) {
                if (CheckCollisionPointRec((Vector2) {(float)GetMouseX(), (float)GetMouseY()},                                           (Rectangle) {menu_btns[i].x, menu_btns[i].y, menu_btns[i].width, menu_btns[i].height})) {
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

        process_input(player, camera, current_level); //@ Unfold in here???

        { ///APPLY INPUTS TO PLAYER - walking, jumping, collision

            player.speed.x += delta_time * (player.acceleration_right - player.acceleration_left);
            player.speed.y += delta_time * (player.acceleration_down - player.acceleration_up);


            static_object_collision_by_speed(player, current_level);

            if (player.speed.y > MAX_FALL_SPEED) { player.speed.y = MAX_FALL_SPEED; }
            player.x += player.speed.x * delta_time;
            player.y += player.speed.y * delta_time;

            prevent_collision_stuck(player, current_level);

            level_borders_collision(player, current_level);

        }

        camera.zoom += ((float)GetMouseWheelMove() * 0.1f); //(0.1f*camera.zoom));
        if(!free_cam) // Camera just follows player for now
            camera.target = (Vector2){ player.x + player.width/2, player.y + player.height/2};


        ingame_mouse.x = ((GetMouseX() )) / camera.zoom + (camera.target.x - camera.offset.x / camera.zoom);
        ingame_mouse.y = ((GetMouseY() )) / camera.zoom + (camera.target.y - camera.offset.y / camera.zoom);


        /// --- Game Logic ---
        // stuff like sound, event triggers, animations logic will be here (everything that isn't Input or Draw)

        if (!player.is_standing && !player.is_levitating && player.speed.y > 0) {
            player.time_of_fall += delta_time;
        } else player.time_of_fall = INITIAL_TIME_OF_FALL;

        if (!player.is_standing) player.air_time += delta_time;
        else player.air_time = 0;

        if (player.is_standing) {
            player.is_holding_jump = false;
            player.is_jumping = false;
            player.jump_input_hold_time = 0;
        }

        {/// Player started falling - one frame flag
            static float previous_speed_y = -1;
            if (player.started_falling) { player.started_falling = false; }
            if (previous_speed_y <= 0 && player.speed.y > 0) {
                player.started_falling = true; }
            previous_speed_y = player.speed.y;
        }
        { /// Player landed
            static bool previous_standing = false;
            if (player.has_landed) player.has_landed = false;
            if (!previous_standing && player.is_standing) player.has_landed = true;
            previous_standing = player.is_standing;
        }
        { /// PLayer started jumping - flag goes for 1.5 loops, so be cautious
            static bool previous_jumped = false;
            if (previous_jumped && player.started_jumping) {
                player.started_jumping = false;
                previous_jumped = false;
            }
            if (player.started_jumping) { previous_jumped = true; }
        }


        { ///Animation management
            /// STARTED FALLING
            if (player.started_falling) { anims_player[PLAYER_FALL_START].is_playing = true; }
            if (player.is_standing) anims_player[PLAYER_FALL_START].is_playing = false;
            /// LANDED
            if (player.has_landed) anims_player[PLAYER_LANDED].is_playing = true;
            if (!player.is_standing) anims_player[PLAYER_LANDED].is_playing = false;
            /// STARTED JUMPING
            if (player.started_jumping) anims_player[PLAYER_JUMP_START].is_playing = true;
            if (player.time_of_fall > INITIAL_TIME_OF_FALL || player.is_standing)
                { anims_player[PLAYER_JUMP_START].is_playing = false; }
            ///  IDLE
            if(player.speed.x == 0 && player.is_standing == 1){
                anims_player[PLAYER_IDLE].is_playing = true;
            } else{
                anims_player[PLAYER_IDLE].is_playing = false;
            }
            ///  RUNNING
            if(abs(player.speed.x) > 0 && player.is_standing == 1){
                anims_player[PLAYER_RUN].is_playing = true;
            } else{
                anims_player[PLAYER_RUN].is_playing = false;
            }
            /// JUMPING
            if(player.speed.y < 0 && !anims_player[PLAYER_JUMP_START].is_playing){
                anims_player[PLAYER_JUMP].is_playing = true;
            } else anims_player[PLAYER_JUMP].is_playing = false;
            /// FALLING
            if(player.speed.y >= 0 && !anims_player[PLAYER_FALL_START].is_playing){
                anims_player[PLAYER_FALL].is_playing = true;
            } else anims_player[PLAYER_FALL].is_playing = false;

            for ( auto& [name, anim] : anims_player) {
                reset_animation(anim);
            }
        }


        /// --- Draw ---
        { /// Pre-Draw adjustments
            if (IsWindowResized() && !IsWindowState(FLAG_WINDOW_MINIMIZED)) { /// Apply screen scale
                screen_scale.x = (float) GetScreenWidth() / game_screen_width;
                screen_scale.y = (float) GetScreenHeight() / game_screen_height;
                screen_scale_min = MIN(screen_scale.x, screen_scale.y);
                if (screen_scale_min == screen_scale.x) {
                    _screen_width = GetScreenWidth();
                    _screen_height = _screen_width / 1.77777f;
                } else {
                    _screen_height = GetScreenHeight();
                    _screen_width = _screen_height * (16.0f / 9.0f);
                }
            }

            static float _previous_screen_scale_min = screen_scale_min;
            if (_previous_screen_scale_min != screen_scale_min) {
                _previous_screen_scale_min = screen_scale_min;

                UnloadRenderTexture(render_texture);
                render_texture = LoadRenderTexture(_screen_width,
                                                   _screen_height); // We can't just change width & height, we need to reload render texture to properly change it's size.
                SetTextureWrap(render_texture.texture, TEXTURE_WRAP_CLAMP);
                printf("---Screen resized---\n");
            }
            {  // Mouse screen scale
                if(letterboxing){
                    // NOTE: When mouse is at the black stripes it should be clamped to the edges of gamescreen ideally, but it's too long to implement and the current state won't break the game too much, so I leave it as it is.
                    SetMouseScale(1/screen_scale_min, 1/screen_scale_min);
                    SetMouseOffset(-(GetScreenWidth() - _screen_width) * 0.5f, -(GetScreenHeight() - _screen_height) * 0.5f);
                } else {
                    SetMouseScale(1/screen_scale.x, 1/screen_scale.y);
                    SetMouseOffset(0, 0);
                }
            }
        }

        BeginTextureMode(render_texture); /// Draw everything to target texture to enable window stretching
        {
            ClearBackground(LIGHTGRAY);

            Camera2D ingame_screen_scaler = camera; // We scale screen resolution by zooming camera
            ingame_screen_scaler.zoom *= screen_scale_min;
            ingame_screen_scaler.offset.x = (float)_screen_width * 0.5f;
            ingame_screen_scaler.offset.y = (float)_screen_height * 0.5f;

            BeginMode2D(ingame_screen_scaler);
            {
                DrawTextureEx(background_texture, (Vector2) { 0,  0}, 0, 3, WHITE);
                //draw player collision box
                if(_draw_debug_info)DrawRectangleRec((Rectangle){player.x, player.y, player.width, player.height}, RED);

                { // Draw player func
                    if (anims_player[PLAYER_LANDED].is_playing){
                        draw_player_animation(player, anims_player[PLAYER_LANDED]);
                    }
                    else if(anims_player[PLAYER_IDLE].is_playing) { // "is_idle"
                        draw_player_animation(player, anims_player[PLAYER_IDLE]);
                    }else if(anims_player[PLAYER_RUN].is_playing){// "is_running"
                        draw_player_animation(player, anims_player[PLAYER_RUN]);
                    }else if(player.speed.y < 0) //is jumping
                    {
                        if (anims_player[PLAYER_JUMP_START].is_playing){
                            draw_player_animation(player, anims_player[PLAYER_JUMP_START]);
                        }
                        else{ draw_player_animation(player,anims_player[PLAYER_JUMP]);}
                    }else //is falling
                    {
                        if (anims_player[PLAYER_FALL_START].is_playing)
                            draw_player_animation(player, anims_player[PLAYER_FALL_START]);
                        else
                            draw_player_animation(player,anims_player[PLAYER_FALL]);
                    }
                }

                static_objects_draw(current_level, ground_texture, 'G');

                if(_draw_debug_info)DrawRectangle(ingame_mouse.x - 3, ingame_mouse.y - 3, 8, 8, RED ); /// GAME MOUSE DEBUG
            } EndMode2D(); // scaled_camera

            Camera2D overlay_screen_scaler = {0,0,0,0,0,1}; // We scale screen resolution by zooming camera
            overlay_screen_scaler.target.x = (float)game_screen_width * 0.5f;
            overlay_screen_scaler.target.y = (float)game_screen_height * 0.5f;
            overlay_screen_scaler.zoom = screen_scale_min;
            overlay_screen_scaler.offset.x = (float)_screen_width * 0.5f;
            overlay_screen_scaler.offset.y = (float)_screen_height * 0.5f;

            BeginMode2D(overlay_screen_scaler);
            {  /// Overlay (everything that is not the game)
                if (_is_menu)
                {
                    BeginBlendMode(2);
                    DrawRectangle(0, 0, game_screen_width, game_screen_height, (Color) {0, 0, 0, 70});//darken the game when menu opened
                    EndBlendMode();

                    for (int i = 0; i < MENU_BUTTONS; ++i) {
                        DrawRectangle(menu_btns[i].x, menu_btns[i].y, menu_btns[i].width, menu_btns[i].height,
                                      (menu_btns[i].state == 0) ? menu_btns[i].color : LIME);
                        DrawText(menu_btns[i].label, menu_btns[i].x, menu_btns[i].y, 40, BLACK);
                    }
                }
                if (_is_paused) DrawText("PAUSED", 500, 50, 60, LIME);

                { /// DRAW DEBUG INFO
                    if (_draw_debug_info) draw_debug_info(); //@Maybe just put func contents here
                    show_collision(player,current_level); //@ Move to draw_debug_info

                    //@Temporary - move this ALL to draw debug info later
                    DrawRectangle(0,0,250,400,Color{0,0,0,180}); // Debug info opacity

                    DrawText(TextFormat(PRINT_INT(player.has_landed)), 10, 40, 20, player.has_landed == 1 ? ORANGE : LIME);
                    DrawText(TextFormat(PRINT_INT(player.is_standing)), 10, 80, 20, player.is_standing == 1 ? ORANGE : LIME);
                    DrawText(TextFormat("time of fall %.4f", player.time_of_fall), 10, 100, 20, LIME);

                    //DrawText(TextFormat(PRINT_INT(GetScreenWidth())), 10, 220, 20, LIME);
                    //DrawText(TextFormat(PRINT_INT(_screen_height)), 10, 240, 20, LIME);
                    DrawText(TextFormat(PRINT_FLOAT(ingame_screen_scaler.zoom)), 10, 280, 20, LIME);
                    DrawText(TextFormat("avg time %.4f us", average_measured_time), 10, 320, 20, LIME);

                    if (free_cam) DrawText("FREE CAMREA", 700, 100, 40, DARKGREEN);

                    DrawFPS(10,10);
                    DrawText(TextFormat( "%.4fms", GetFrameTime() *1000), 120, 10, 20, LIME);

                    if(_vsync) DrawText("V-sync", 210, 9, 10, GREEN);
                    if(_fps_lock) DrawText("LOCKED", 210, 18, 10, GREEN);

                    DrawRectangle(GetMouseX()-2, GetMouseY()-2,5,5,GOLD); /// OVERLAY MOUSE
                }
            }
            EndMode2D(); //Overlay scaler

        }
        EndTextureMode();
        //EndDrawing(); // "Target FPS" lock slows fps here

        { /// Draw target render texture on the screen, properly scaled (Final draw)
            BeginDrawing();
            ClearBackground(BLACK);

            if(_screen_filtering) SetTextureFilter(render_texture.texture, TEXTURE_FILTER_BILINEAR);
            else SetTextureFilter(render_texture.texture, TEXTURE_FILTER_POINT);

            if(letterboxing)
            { ///LETTERBOX
                Rectangle source = { 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height};
                Rectangle dest = {(float) ((GetScreenWidth() - _screen_width) /2),
                                  (float) ((GetScreenHeight() - _screen_height) /2),
                                  (float)_screen_width, (float)_screen_height };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(render_texture.texture, source, dest, origin, 0.0f, WHITE);
            }
            else
            { /// STRETCH
                Rectangle source = { 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height};
                Rectangle dest = {0,0,(float)GetScreenWidth(), (float)GetScreenHeight()};
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(render_texture.texture, source, dest, origin, 0.0f, WHITE);
            }
            EndDrawing(); // "Target FPS" lock slows fps here
        }

    } ///---Main loop


    /*
    We actually don't need to unload anything after the main loop because at this point the game will be closed and the OS will wipe all of the program's memory. The only thing should be happening after main loop is saving and similar stuff.
    DON'T DELETE THIS: leave commented out to remember that we need to unload textures if we're NOT closing program (e.g. changing levels) to avoid LEAKS. - silense_please, 06 aug 24.

    for ( auto& anim_pair : anims_player) { // anim.first is a 'const' key, anim.second is a value.
        UnloadTexture(anim_pair.second.texture);
    }

    UnloadTexture(player_texture);
    UnloadTexture(ground_texture);
    UnloadTexture(background_texture);
    UnloadRenderTexture(render_texture);

    //UnloadMusicStream(music);
    */

    //@ Investigate later: I'm not sure that we don't need these, what if raylib breaks? or "audio device" won't close?
    //CloseAudioDevice();
    //CloseWindow();

    printf("Errors: %i\n", total_errors); // Make error system later
    return 0;
}

