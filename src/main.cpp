#include "declaration.cpp"

#include "settings.cpp" //these includes are either in main or at the bottom of declaration
#include "load.cpp"
#include "input.cpp"
#include "collision.cpp"
#include "draw.cpp"
#include "debugging.cpp"


int main(void){

    //Actions ACTION; - //TODO @ wrap buttons into struct and walk through them all inside the func to spellcheck
    input_mapping_check(BUTTON_JUMP);
    input_mapping_check(BUTTON_FIRE);


    /// Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE /*| FLAG_MSAA_4X_HINT*/ | FLAG_WINDOW_ALWAYS_RUN ); // |FLAG_VSYNC_HINT);
    InitWindow(game_screen_width, game_screen_height, "RAYLIB-DEMO");
    //SetMouseCursor(0);


    toggle_borderless(); // Set borderless at startup
    apply_screen_scale(); // Startup Resize (after toggle_borderless)

    // Render texture initialization - to hold the rendering result and be able to resize it
    RenderTexture2D render_texture = LoadRenderTexture(_screen_width,_screen_height);
    SetTextureWrap(render_texture.texture, TEXTURE_WRAP_CLAMP);

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
    //and get rid of STL map, do we even need to optimise this with hash table? Maybe just Array of Structs? (as JBlow advised) Or maybe it's fine as it is.
    // Use .at() instead of [] ??   ( [] operator generates new map member if it didn't find one)
    //https://www.youtube.com/watch?v=KiB0vRi2wlc
    std::unordered_map<unsigned int, Animation> anims_player  {
            {PLAYER_RUN,        {LoadTexture("Game_Data/animations/player_run.png"), 0.04, true}},
            {PLAYER_IDLE,       {LoadTexture("Game_Data/animations/player_idle.png"), 0.4, true}},
            {PLAYER_JUMP,       {LoadTexture("Game_Data/animations/player_jump.png"), 0.1, true}},
            {PLAYER_FALL,       {LoadTexture("Game_Data/animations/player_fall.png"),0.1, true}},
            {PLAYER_FALL_START, {LoadTexture("Game_Data/animations/player_fall_start.png"),0.06, false}},
            {PLAYER_JUMP_START, {LoadTexture("Game_Data/animations/player_jump_start.png"), 0.1, false}},
            {PLAYER_LANDED,     {LoadTexture("Game_Data/animations/player_fall_end.png"),0.05, false}},
    };

    Animation explosion = {LoadTexture("Game_Data/animations/explosion.png"), 0.05, false};


    // Default wrapping (repeat) distorts edges of sprites when they're scaled imperfectly (e.g. on camera zoom = 1.6).
    for ( auto& [name, anim] : anims_player) { //@ NO NEED ANYMORE? FIXED?
        SetTextureWrap(anim.texture, TEXTURE_WRAP_CLAMP);
    }

    Texture2D background_texture = LoadTexture("Game_Data/background.png");
    Texture2D ground_texture = LoadTexture("Game_Data/ground.png");
    //Texture2D RPG = LoadTexture("Game_Data/animations/rpg-7.png");
    Texture2D rpg_hand = LoadTexture("Game_Data/animations/hand.png");
    Texture2D rocket_tex = LoadTexture("Game_Data/animations/rocket.png");

    Player player;
    RPG rpg{LoadTexture("Game_Data/animations/rpg-7.png")};
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
    Sound boom = LoadSound("Game_Data/boom.mp3");
    SetSoundVolume(boom, 0.03);

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

        {
            /// Player started falling - one frame flag
            static float previous_speed_y = -1;
            if (player.started_falling) { player.started_falling = false; }
            if (previous_speed_y <= 0 && player.speed.y > 0) {
                player.started_falling = true; }
            previous_speed_y = player.speed.y;

            /// Player landed
            static bool previous_standing = false;
            if (player.has_landed) player.has_landed = false;
            if (!previous_standing && player.is_standing) player.has_landed = true;
            previous_standing = player.is_standing;

            /// PLayer started jumping - flag goes for 1.5 loops, so be cautious
            static bool previous_jumped = false;
            if (previous_jumped && player.started_jumping) {
                player.started_jumping = false;
                previous_jumped = false;
            }
            if (player.started_jumping) { previous_jumped = true; }

            /// Player is idle
            if (player.speed.x == 0 && player.is_standing == 1) {player.is_idle = true;}
            else {player.is_idle = false;}

            /// Player is running
            if (abs(player.speed.x) > 0 && player.is_standing == 1) {player.is_running = true;}
            else {player.is_running = false;}
        }

        { ///Animation management
            /// LANDED
            if (player.has_landed || anims_player[PLAYER_LANDED].is_playing) {
                player.current_anim = PLAYER_LANDED;
            }
            ///  IDLE
            else if (player.is_idle){
                player.current_anim = PLAYER_IDLE;
            }
            ///  RUNNING
            else if (player.is_running) {
                player.current_anim = PLAYER_RUN;
                float speed = 0.07;
                anims_player[PLAYER_RUN].speed = speed /   (fabs(player.speed.x) / WALK_SPEED ); //@ we need curve instead of line function
            }
            /// JUMPING
            else if (player.speed.y < 0) {
                if (player.started_jumping || anims_player[PLAYER_JUMP_START].is_playing) {
                    player.current_anim = PLAYER_JUMP_START;
                } else {
                    player.current_anim = PLAYER_JUMP;
                }
            }
            /// FALLING
            else /* if (player.speed.y >= 0)*/{
                if (player.started_falling || anims_player[PLAYER_FALL_START].is_playing) {
                    player.current_anim = PLAYER_FALL_START;
                }
                else{
                    player.current_anim = PLAYER_FALL;
                }
            }

            //if (player.is_standing) anims_player[PLAYER_FALL_START].is_playing = false;
            //if (player.current_anim != PLAYER_IDLE) { anims_player[PLAYER_IDLE].is_playing = false;}
            //else { anims_player[PLAYER_RUN].is_playing = false; }

            //if (player.time_of_fall > INITIAL_TIME_OF_FALL || player.is_standing)
            //{ anims_player[PLAYER_JUMP_START].is_playing = false; }
            //if (!player.is_standing) anims_player[PLAYER_LANDED].is_playing = false;

            for ( auto& [name, anim] : anims_player){
                anim.is_playing = false;
            }
            anims_player[player.current_anim].is_playing = true;


            //for ( auto& [name, anim] : anims_player)
            //    { reset_animation(anim); }
        }


        //@ TESTING
        if (IsKeyPressed(KEY_Z)) {camera.zoom = 3;}


        /// --- Draw ---
        { /// Pre-Draw adjustments
            if (IsWindowResized() && !IsWindowState(FLAG_WINDOW_MINIMIZED)) {
                apply_screen_scale(); // This comes first, then the scale adjustments.
            }

            static float _previous_screen_scale_min = screen_scale_min;
            if (_previous_screen_scale_min != screen_scale_min) {
                _previous_screen_scale_min = screen_scale_min;

                UnloadRenderTexture(render_texture);
                render_texture = LoadRenderTexture(_screen_width,_screen_height); // We can't just change width & height, we need to reload render texture to properly change it's size.
                SetTextureWrap(render_texture.texture, TEXTURE_WRAP_CLAMP);
                printf("---Screen resized---\n");
            }
            /// Mouse screen scale
            if(letterboxing){
                // NOTE: When mouse is at the black stripes it should be clamped to the edges of gamescreen ideally, but it's too long to implement and the current state won't break the game too much, so I leave it as it is.
                SetMouseScale(1/screen_scale_min, 1/screen_scale_min);
                SetMouseOffset(-(GetScreenWidth() - _screen_width) * 0.5f, -(GetScreenHeight() - _screen_height) * 0.5f);
            } else {
                SetMouseScale(1/screen_scale.x, 1/screen_scale.y);
                SetMouseOffset(0, 0);
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

                { /// DRAW PLAYER
                    draw_player_animation(player, anims_player[player.current_anim]);
                }



                { /// Draw RPG
                // Sorry, this is just a mess instead of a proper animation system but it's enough for now.
                    //float swing_test = anims_player[player.current_anim].current_frame == 0? 0 : 2; // use player_anim.speed to be in tact with it - @deletable
                    static float rpg_draw_timer = 0;
                    rpg_draw_timer += delta_time;
                    if (rpg_draw_timer > 0.8){
                        rpg_draw_timer = 0;
                    }
                    float x_swing = rpg_draw_timer < 0.4 ? 0 : -1;
                    float y_swing = rpg_draw_timer < 0.4 ? 0 : 2;

                    if (player.current_anim != PLAYER_IDLE){  // !player.is_idle/*player.is_running*/
                        //fast swing
                        rpg_draw_timer += delta_time;
                    }
                    else{
                        //slow swing
                        x_swing = anims_player[player.current_anim].current_frame == 0? 0 :-1;
                        y_swing = anims_player[player.current_anim].current_frame == 0? 0 : 2;
                    }

                    float player_center_x = (player.x + (player.width/2));
                    float player_center_y = (player.y + (player.height/2));

                    //TODO:  pixelate and polish explosion draw, after this is done, clean up and move part of this in game logic
                    // then - multiple rockets, reloading missile when rpg looks down,
                    // add explosion collision circle to destroy surrounding breakable blocks

                    int flip_h = player.facing_left ? -1: 1;


                    float rpg_x = player_center_x + -32 + (6 + x_swing) *flip_h;
                    float rpg_y = player_center_y - 17 + y_swing ;

                    Vector2 center = {rpg.width/2 -15*flip_h, rpg.height/2 +1}; // rotation point



                    if (!IsGamepadAvailable(active_gamepad)){ // MOUSE OR GAMEPAD
                        rpg.rotation = -RAD2DEG * Vector2Angle({rpg_x + center.x + rpg.width, center.y},
                            {ingame_mouse.x - (rpg_x + center.x),(rpg_y + center.y) - ingame_mouse.y}); // @Later. Angle is slightly wrong. We need circle tangent line for this.
                        if(player.facing_left) rpg.rotation -= 180;
                    }
                    else{

                        float x_axis = GetGamepadAxisMovement(active_gamepad, 2);
                        float y_axis = GetGamepadAxisMovement(active_gamepad, 3);

                        float stick_axis = Vector2Angle({1,0}, {x_axis, y_axis});
                        float dead_zone = 0.35;
                        if (fabsf(x_axis) > dead_zone || fabsf(y_axis) > dead_zone){
                            rpg.rotation = RAD2DEG * stick_axis;
                            if(player.facing_left) rpg.rotation -= 180;
                        }
                        else{ // We are not updating rotation if stick is at {0,0} to store the angle and don't reset it.
                            static bool previous_facing_left = player.facing_left;
                            if (previous_facing_left != player.facing_left){
                                previous_facing_left = player.facing_left;
                                rpg.rotation = -rpg.rotation;
                            }
                        }
                    }

                    /// Actually, we dont need to store rotation when we'll have a mouse position updating every frame (but what if it's a stick?)

                    Rectangle source = {0,0 , (float)rpg.texture.width * flip_h, (float)rpg.texture.height};


                    if (abs(rpg.rotation) > 120 && abs(rpg.rotation) <= 270) {
                        source.height = -source.height;
                        center = {rpg.width/2 -15*flip_h, rpg.height/2 -2};
                        rpg_y += 3;
                    }

                    Vector2 position = {rpg_x,rpg_y};
                    Rectangle dest = { position.x + center.x, position.y + center.y, fabsf(source.width), fabsf(source.height) };

                    /*if (rpg.rotation > 360) {rpg.rotation -= 360;}        //@ deletable
                    else if (rpg.rotation < -360) {rpg.rotation += 360;}*/

                    if(_draw_debug_info) DrawRectanglePro(dest, center, rpg.rotation, {220, 30, 30, 150}); // Draw rpg tex box



                    const int RPG_CENTER_OFFSET_WHEN_FORWARD = -3;
                    Vector2 rocket_center =  Vector2Rotate({0,  RPG_CENTER_OFFSET_WHEN_FORWARD}, rpg.rotation * DEG2RAD);
                    if (abs(rpg.rotation) > 120 && abs(rpg.rotation) <= 270) {
                        const int RPG_CENTER_OFFSET_WHEN_BACKWARD = -4;
                        rocket_center =  Vector2Rotate({0,  RPG_CENTER_OFFSET_WHEN_BACKWARD}, (rpg.rotation-180) * DEG2RAD);
                    }
                    rocket_center.x += center.x;
                    rocket_center.y += center.y;

                    const float RPG_LENGTH = 35.f;
                    Vector2 rocket_dir = Vector2Rotate({RPG_LENGTH * flip_h, 0}, rpg.rotation * DEG2RAD); /// НАПРАВЛЕНИЕ ВЫСТРЕЛА

                    if(!rpg.rocket.launched){
                        rpg.rocket.pos = {position.x +rocket_center.x +rocket_dir.x, position.y + rocket_center.y + rocket_dir.y};
                        rpg.rocket.rotation = rpg.rotation;

                        if(IsButtonPressed(BUTTON_FIRE)){
                            rpg.rocket.launched = true;
                            rpg.rocket.dir = rocket_dir; // We save the direction so rocket flies straight and doesn't become homing. (Wink wink)
                        }
                    }

                    static Vector2 explosion_pos = {0};

                    if(rpg.rocket.launched){
                        //draw rpg firing

                        const int ROCKET_SPEED = 20;
                        rpg.rocket.pos.x += rpg.rocket.dir.x * ROCKET_SPEED * delta_time;
                        rpg.rocket.pos.y += rpg.rocket.dir.y * ROCKET_SPEED * delta_time;

                        static double launch_timer = 0; // That's just a hack. Reset "launched" upon collision. No it's not a hack, we need to kill rocket if its out of bounds after some time.
                        launch_timer += delta_time;
                        if (launch_timer > 6.0) { rpg.rocket.launched = false; launch_timer = 0;}

                        if(_draw_debug_info) DrawCircle(rpg.rocket.pos.x, rpg.rocket.pos.y, rpg.rocket.RADIUS, RED); /// draw rocket collision


                        if (rocket_collision(current_level, rpg)){ /// DETONATE
                            rpg.rocket.launched = false;
                            launch_timer = 0;
                            PlaySound(boom);
                            // rpg.rocket.pos - where to detonate
                            { /// EXPLOTION DRAW (MOVE IT)
                                explosion.is_playing = true;
                                explosion_pos = rpg.rocket.pos;
                                explosion.current_frame = 0;

                            }

                            /// if explosion radius(cirle) collides with breakable ground -> level.data[index_x][index_y] = ' ';
                        }

                    }

                    if(explosion.is_playing) draw_animation(explosion, 25, explosion_pos);


                    /// ROCKET DRAW
                    Rectangle rocket_source = {0,0, (float)rocket_tex.width * flip_h, (float)rocket_tex.height};
                    Rectangle rocket_dest = {rpg.rocket.pos.x,rpg.rocket.pos.y, (float)rocket_tex.width, (float)rocket_tex.height};
                    DrawTexturePro(rocket_tex, rocket_source, rocket_dest, {float(10 - 2*flip_h),4}, rpg.rocket.rotation, WHITE);

                    // REFACTOR THIS?
                    /// RPG DRAW
                    Rectangle hand_source = {0,0 , (float)rpg_hand.width * flip_h, (float)rpg_hand.height};
                    if (abs(rpg.rotation) > 120 && abs(rpg.rotation) <= 270) {
                        hand_source.height = -hand_source.height;
                        DrawTexturePro(rpg_hand, hand_source,{position.x + center.x , position.y + center.y, (float)rpg_hand.width , (float)rpg_hand.height}, {float(16 - 12*flip_h), 6}, rpg.rotation - 5*flip_h, WHITE); // DRAW HAND
                        DrawTexturePro(rpg.texture, source, dest, center, rpg.rotation, WHITE);
                    }
                    else{
                        DrawTexturePro(rpg.texture, source, dest, center, rpg.rotation, WHITE);
                        DrawTexturePro(rpg_hand, hand_source,{position.x + center.x, position.y + center.y, (float)rpg_hand.width , (float)rpg_hand.height}, {float(16 - 12*flip_h), 2}, rpg.rotation + 3*flip_h, WHITE); // DRAW HAND
                    }

                    if(_draw_debug_info) {

                        DrawRectangle(position.x + center.x , position.y + center.y , 1, 1,RED); // draw rpg center
                        DrawRectangleV({position.x +rocket_center.x , position.y + rocket_center.y }, {2, 2},BLACK); //draw rocket vector origin
                        DrawRectangleV(rpg.rocket.pos, {1,1},GREEN); /// ROCKET STARTING POINT
                    }
                }  /// --- RPG DRAW

                static_objects_draw(current_level, ground_texture, 'G');

                DrawText("Somewhere between the sacred silense and sleep...", 2200, 2500, 50, {194, 144, 87, 255});
                if(_draw_debug_info)DrawRectangle(ingame_mouse.x - 3, ingame_mouse.y - 3, 8, 8, RED ); /// GAME MOUSE DEBUG
            } EndMode2D(); // scaled_camera

            Camera2D overlay_screen_scaler = {{0,0},{0,0},0,1}; // We scale screen resolution by zooming camera
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

                    DrawText(PRINT_INT(player.has_landed), 10, 40, 20, player.has_landed == 1 ? ORANGE : LIME);
                    DrawText(PRINT_INT(player.is_standing), 10, 80, 20, player.is_standing == 1 ? ORANGE : LIME);
                    DrawText(TextFormat("time of fall %.4f", player.time_of_fall), 10, 100, 20, LIME);

                    //DrawText(TextFormat(PRINT_INT(GetScreenWidth())), 10, 220, 20, LIME);
                    DrawText(PRINT_FLOAT(rpg.rotation), 10, 240, 20, LIME);
                    DrawText(PRINT_FLOAT(ingame_screen_scaler.zoom), 10, 280, 20, LIME);
                    DrawText(TextFormat("avg time %.4f us", average_measured_time), 10, 320, 20, LIME);


                    if (free_cam) DrawText("FREE CAMREA", 700, 100, 40, DARKGREEN);

                    DrawFPS(10,10);
                    DrawText(TextFormat( "%.4fms", GetFrameTime() *1000), 120, 10, 20, LIME);

                    if(_vsync) DrawText("V-sync", 210, 9, 10, GREEN);
                    if(_fps_lock) DrawText("LOCKED", 210, 18, 10, GREEN);

                    DrawRectangle(GetMouseX()-2, GetMouseY()-2,5,5,GOLD); /// OVERLAY MOUSE

                    DrawText(TextFormat("X axis %.4f",GetGamepadAxisMovement(active_gamepad, 2)),300, 10, 20, LIME );
                    DrawText(TextFormat("Y axis %.4f",GetGamepadAxisMovement(active_gamepad, 3)),300, 30, 20, LIME );
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

