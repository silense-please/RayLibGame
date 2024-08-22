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
    InitWindow(screen_width, screen_height, "RAYLIB-DEMO");
    //SetMouseCursor(0);

    // Render texture initialization - to hold the rendering result and be able to resize it
    RenderTexture2D render_texture = LoadRenderTexture(screen_width, screen_height);

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
    for ( auto& anim : anims_player) { // anim.first is a 'const' key, anim.second is a value.
        SetTextureWrap(anim.second.texture, TEXTURE_WRAP_CLAMP);
    }
    // @ Also generate mipmaps? We're gonna have to make high-res textures than, so they can be squeezed.
    //GenTextureMipmaps(&anims_player[PLAYER_IDLE].texture);

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
        {(Vector2){(float)screen_width / 2, (float)screen_height / 2},
         (Vector2){(float)screen_width / 2, (float)screen_height / 2 }, 0, DEFAULT_ZOOM};

    const int MENU_BUTTONS = 2;
    Menu_Button menu_btns[MENU_BUTTONS]
            {
                    {BTN_RESUME,"RESUME", (float)screen_width / 2 - 125, 200, },
                    {BTN_QUIT,  "QUIT",   (float)screen_width / 2 - 125, 400, }
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

        /// Process Input
        if(_is_menu){
            for (int i = 0; i < MENU_BUTTONS; ++i) {
                if (CheckCollisionPointRec((Vector2) {GetMouseX() / screen_scale_x, GetMouseY() / screen_scale_y}, //@ Scaled mouse maybe???
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


        process_input(player, camera, current_level); //@ Unfold in here???

        { ///APPLY INPUTS TO PLAYER - walking, jumping, collision

            if (!player.is_standing && !player.is_levitating && player.speed_y > 0) { // @move this "if" to Game Logic?
                player.falling_time += delta_time;
            } else player.falling_time = INITIAL_FALLING_TIME;

            if (!player.is_standing) player.air_time += delta_time;
            else player.air_time = 0;

            if (player.is_standing) {
                player.is_holding_jump = false;
                player.is_jumping = false;
                player.jump_input_hold_time = 0;
            }


            player.speed_x += delta_time * (player.acceleration_right - player.acceleration_left);
            player.speed_y += delta_time * (player.acceleration_down - player.acceleration_up);


            static_object_collision_by_speed(player, current_level);

            if (player.speed_y > MAX_FALL_SPEED) { player.speed_y = MAX_FALL_SPEED; }
            player.x += player.speed_x * delta_time;
            player.y += player.speed_y * delta_time;

            prevent_collision_stuck(player, current_level);

            level_borders_collision(player, current_level);

        }

        camera.zoom += ((float)GetMouseWheelMove() * 0.1f); //(0.1f*camera.zoom));
        if(!free_cam) // Camera just follows player for now
            camera.target = (Vector2){ player.x + player.width/2, player.y + player.height/2};

        //WARNING: This should be the same as in prcess input(or make func already); Ideally this shouldn't duplicate at all
        //scaled_mouse.x = (GetMouseX() / screen_scale_x) / camera.zoom + (camera.target.x - camera.offset.x / camera.zoom);
        //scaled_mouse.y = (GetMouseY() / screen_scale_y) / camera.zoom + (camera.target.y - camera.offset.y / camera.zoom);

        if(letterboxing){
            SetMouseScale(1/screen_scale_min, 1/screen_scale_min);
            SetMouseOffset(-(GetScreenWidth() - _window_width)*0.5f, -(GetScreenHeight() - _window_height)*0.5f);
        } else {
                SetMouseScale(1/screen_scale_x, 1/screen_scale_y);
        }

        scaled_mouse.x = ((GetMouseX() )) / camera.zoom + (camera.target.x - camera.offset.x / camera.zoom);
        scaled_mouse.y = ((GetMouseY() )) / camera.zoom + (camera.target.y - camera.offset.y / camera.zoom);
        //Vector2Clamp(scaled_mouse, (Vector2){0,0}, (Vector2){(float)screen_height,(float)screen_width});
        // @WHY DO I NEED THIS? (clamped mouse value behind game screen)

        /// Game Logic
        // stuff like sound, event triggers, animations logic will be here (everything that isn't Input or Draw)
        //camera.zoom = 1.3; // @animation testing @Actually zoom 1 is too far, make it ~1.4 (change init window properly)


        {/// Player started falling - one frame flag
            static float previous_speed_y = -1;
            if (player.started_falling) { player.started_falling = false; }
            if (previous_speed_y <= 0 && player.speed_y > 0) {
                player.started_falling = true; }
            previous_speed_y = player.speed_y;
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
            if (player.falling_time > 0 || player.is_standing) { anims_player[PLAYER_JUMP_START].is_playing = false; }
            ///  IDLE
            if(player.speed_x == 0 && player.speed_y == 0){
                anims_player[PLAYER_IDLE].is_playing = true;
            } else{
                anims_player[PLAYER_IDLE].is_playing = false;
            }
            ///  RUNNING
            if(abs(player.speed_x) > 0 && player.speed_y == 0){
                anims_player[PLAYER_RUN].is_playing = true;
            } else{
                anims_player[PLAYER_RUN].is_playing = false;
            }
            /// JUMPING
            if(player.speed_y < 0 && !anims_player[PLAYER_JUMP_START].is_playing){
                anims_player[PLAYER_JUMP].is_playing = true;
            } else anims_player[PLAYER_JUMP].is_playing = false;
            /// FALLING
            if(player.speed_y >= 0 && !anims_player[PLAYER_FALL_START].is_playing){
                anims_player[PLAYER_FALL].is_playing = true;
            } else anims_player[PLAYER_FALL].is_playing = false;
        }


//        for (auto anim = anims_player.begin(); anim != anims_player.end(); ++anim) { // very old version @Deletable!!!
//            reset_animation(anim->second);
//        }

        for ( auto& anim : anims_player) { // anim.first is a 'const' key, anim.second is a value.
            reset_animation(anim.second);
        }

//        for ( auto& [name, anim] : anims_player) { // C++17 fancy version - @Test speed. (of C++17 also)
//            reset_animation(anim);
//        }


        /// Draw everything to target texture to enable window stretching
        //BeginDrawing();
        if (IsWindowResized()){
            UnloadRenderTexture(render_texture);
            render_texture = LoadRenderTexture(_window_width, _window_height); // We can't just change width & height, we need to reload render texture to properly change it's size.
        }
        BeginTextureMode(render_texture);
        {
            ClearBackground(ORANGE);

            Camera2D game_screen_scaler = camera; // We scale screen resolution by zooming camera
            game_screen_scaler.zoom *= screen_scale_min;
            game_screen_scaler.offset.x = (float)_window_width * 0.5f;
            game_screen_scaler.offset.y = (float)_window_height * 0.5f;

            BeginMode2D(game_screen_scaler);
            {
                DrawTextureEx(background_texture, (Vector2) { 0,  0}, 0, 3, WHITE);
                //draw player collision box
                if(_draw_debug_info)DrawRectangleRec((Rectangle){player.x, player.y, player.width, player.height}, RED);

                DrawRectangle(scaled_mouse.x-2, scaled_mouse.y-2,5,5,RED );


                { // Draw player func
                    if (anims_player[PLAYER_LANDED].is_playing){
                        draw_player_animation(player, anims_player[PLAYER_LANDED]);
                    }
                    else if(anims_player[PLAYER_IDLE].is_playing) { // "is_idle"
                        draw_player_animation(player, anims_player[PLAYER_IDLE]);
                    }else if(anims_player[PLAYER_RUN].is_playing){// "is_running"
                        draw_player_animation(player, anims_player[PLAYER_RUN]);
                    }else if(player.speed_y < 0) //is jumping
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

            } EndMode2D(); // scaled_camera


            Camera2D overlay_screen_scaler = camera; // We scale screen resolution by zooming camera
            overlay_screen_scaler.target.x = (float)screen_width * 0.5f;
            overlay_screen_scaler.target.y = (float)screen_height * 0.5f;
            overlay_screen_scaler.zoom = screen_scale_min;
            overlay_screen_scaler.offset.x = (float)_window_width * 0.5f;
            overlay_screen_scaler.offset.y = (float)_window_height * 0.5f;

            BeginMode2D(overlay_screen_scaler);
            {  /// Overlay (everything that is not the game)
                if (_is_menu)
                {
                    BeginBlendMode(2);
                    DrawRectangle(0, 0, screen_width, screen_height, (Color) {0, 0, 0, 70});//darken the game when menu opened
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
                    show_collision(player,current_level); //@Temporary - maybe move to draw_debug_info after collision system is finished

                    //@Temporary - move to draw debug info later
                    DrawRectangle(0,0,250,400,Color{0,0,0,180}); // Debug info opacity

                    DrawText(TextFormat(PRINT_INT(player.has_landed)), 10, 40, 20, player.has_landed == 1 ? ORANGE : LIME);
                    DrawText(TextFormat(PRINT_INT(player.is_standing)), 10, 80, 20, player.is_standing == 1 ? ORANGE : LIME);

                    //DrawText(TextFormat(PRINT_INT(_vsync)), 10, 200, 20,_vsync == 0 ? ORANGE : LIME);
                    //DrawText(TextFormat(PRINT_INT(_fps_lock)), 10, 240, 20,_fps_lock== 0 ? ORANGE : LIME);

                    DrawText(TextFormat(PRINT_FLOAT(scaled_mouse.x)), 10, 240, 20,LIME);
                    DrawText(TextFormat(PRINT_FLOAT(game_screen_scaler.zoom)), 10, 280, 20, LIME);
                    DrawText(TextFormat("avg time %.4f us", average_measured_time), 10, 320, 20, LIME);

                    if (free_cam) DrawText("FREE CAMREA", 700, 100, 40, DARKGREEN);

                    DrawText(TextFormat( "%.4fms", GetFrameTime() *1000), 120, 10, 20, LIME);
                    DrawFPS(10,10);
                }
            }
            EndMode2D(); //Overlay scaler

        }
        EndTextureMode();
        //EndDrawing(); // "Target FPS" lock slows fps here

        { /// Draw target render texture on the screen, properly scaled (Final draw)
            BeginDrawing();
            ClearBackground(BLACK);

            if(letterboxing)
            { ///LETTERBOX; @ fix virtual mouse
                Rectangle source = { 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height};
                Rectangle dest = {(GetScreenWidth() - ((float)_window_width))*0.5f, (GetScreenHeight() - ((float)_window_height))*0.5f,(float)_window_width, (float)_window_height  };
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



            // УДОЛИИИИИИИИИИИ
            //DrawTextureRec(render_texture.texture, source, origin, WHITE);
            //DrawTexturePro(render_texture.texture, (Rectangle){ 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height}, (Rectangle){0, 0,(float)GetScreenWidth(), (float)GetScreenHeight() }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            //DrawTexturePro(render_texture.texture, (Rectangle){ 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height }, (Rectangle){ (GetScreenWidth() - ((float)_window_width))*0.5f, (GetScreenHeight() - ((float)_window_height))*0.5f,(float)_window_width, (float)_window_height }, (Vector2){ 0, 0 }, 0.0f, WHITE);


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

