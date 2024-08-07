#include "declaration.cpp"

#include "settings.cpp" //these includes are either in main or at the bottom of declaration
#include "load.cpp"
#include "input.cpp"
#include "collision.cpp"
#include "draw.cpp"
#include "debugging.cpp"


int main(void){


    /// Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_ALWAYS_RUN ); // |FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "RAYLIB-DEMO");
    //SetMouseCursor(0);

    // Render texture initialization - to hold the rendering result and be able to resize it
    RenderTexture2D render_texture = LoadRenderTexture(window_width, window_height);

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

    //@TESTING
    //GenTextureMipmaps(&anims_player[PLAYER_IDLE].texture);
    //SetTextureFilter(anims_player[PLAYER_IDLE].texture, TEXTURE_FILTER_ANISOTROPIC_16X);
    //SetTextureFilter(render_texture.texture, TEXTURE_FILTER_BILINEAR);
    //SetTextureWrap(anims_player[PLAYER_IDLE].texture, TEXTURE_WRAP_CLAMP);



    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");


    Player player;
    Game_Level current_level;
    load_level(current_level);
    load_player_spawn(current_level, player);

    Camera2D camera = {(Vector2){(float)window_width /2, (float)window_height /2},
                       (Vector2){ (float)window_width /2, (float)window_height /2 },0,1.5};

    const int MENU_BUTTONS = 2;
    Menu_Button menu_btns[MENU_BUTTONS]
            {
                    {BTN_RESUME,"RESUME",(float)window_width/2 -125, 200, },
                    {BTN_QUIT,  "QUIT",  (float)window_width/2 -125, 400, }
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

            static_object_collision_by_coordinates(player, current_level);

            level_borders_collision(player, current_level);

        }


        camera.zoom += ((float)GetMouseWheelMove() * 0.1f); //(0.1f*camera.zoom));
        if(!free_cam) // Camera just follows player for now
            camera.target = (Vector2){ player.x + player.width/2, player.y + player.height/2};

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

//        for ( auto& [name, anim] : anims_player) { // C++17 fancy version - test speed
//            reset_animation(anim);
//        }


        /// Draw everything to target texture to enable window scaling/stretching
        BeginTextureMode(render_texture);
        {
            ClearBackground(LIGHTGRAY);
            BeginMode2D(camera);
            {
                DrawTextureEx(background_texture, (Vector2) {(float) 0, (float) 0}, 0, 3, WHITE);
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
            show_collision(player,current_level); //@Temporary - maybe move to draw_debug_info after collision system is finished

            //@Temporary - move to draw debug info later
            DrawRectangle(0,0,250,400,Color{0,0,0,180}); // Debug info opacity
            //DrawText(TextFormat("standing: %d", player.is_standing), 10, 40, 20, LIME);
            //DrawText(TextFormat("jumping: %d", player.is_jumping), 10, 40, 20, LIME);
            //DrawText(TextFormat("has landed: %d", player.has_landed), 10, 40, 20, LIME);
            DrawText(TextFormat(PRINT_INTEGER(player.has_landed)), 10, 40, 20, player.has_landed == 1?ORANGE:LIME);
            DrawText(TextFormat(PRINT_INTEGER(player.is_standing)), 10, 80, 20, player.is_standing == 1?ORANGE:LIME);


            /*
            DrawText(TextFormat("jump hold t: %f", player.jump_input_hold_time), 10, 120, 20, LIME);
            DrawText(TextFormat("jump hold b: %d", player.is_holding_jump), 10, 160, 20, LIME);
            DrawText(TextFormat("air time: %f", player.air_time), 10, 200, 20, LIME);

            DrawText(TextFormat("Speed Y: %f", player.speed_y), 10, 280, 20, LIME);

             */
            DrawText(TextFormat(PRINT_FLOAT(camera.zoom)), 10, 200, 20, LIME);

            DrawText(TextFormat("falling_time: %f ", player.falling_time), 10, 240, 20, LIME);
            DrawText(TextFormat("%f us", average_measured_time), 10, 320, 20, LIME);

            if (free_cam) DrawText("FREE CAMREA", 700, 100, 40, DARKGREEN);

            DrawText(TextFormat( "%fms", GetFrameTime() *1000), 120, 10, 20, LIME);
            DrawFPS(10,10);
        } EndTextureMode();

        { /// Draw resized target render texture on the screen, properly scaled (Final draw)
            BeginDrawing();
            ClearBackground(LIGHTGRAY);

            Rectangle source = { 0.0f, 0.0f, (float)render_texture.texture.width, (float)-render_texture.texture.height};
            Rectangle dest = {((float)GetScreenWidth() - ((float)initial_window_width*scale_x))*0.5f, ((float)GetScreenHeight() - ((float)initial_window_height*scale_y))*0.5f, (float)initial_window_width*scale_x, (float)initial_window_height*scale_y };
            Vector2 origin = { 0.0f, 0.0f };

            DrawTexturePro(render_texture.texture, source, dest, origin, 0.0f, WHITE);

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

