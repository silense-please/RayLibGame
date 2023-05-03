
void toggle_borderless(){
    static Vector2 window_position; //= GetWindowPosition(); //better be global value
    static int saved_width = GetScreenHeight();
    static int saved_height = GetScreenWidth();
    if (_is_borderless){ //Restore Windowed mode
        SetWindowSize(saved_width, saved_height);
        SetWindowPosition(int(window_position.x), int(window_position.y));
        //SetWindowPosition(100,100); delete this, this is for debugging
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
    }
    else{ //Set Borderless
        window_position = GetWindowPosition(); //a bit dangerous - vectors are float and position must be presize
        Vector2 monitor_position = GetMonitorPosition(GetCurrentMonitor()); //left-upper corner of current monitor
        saved_height = GetScreenHeight(); //or GetRenderHeight()???
        saved_width = GetScreenWidth();
        //^^^ Remember window state before turning on borderless ^^^

        SetWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowPosition(int(monitor_position.x),int(monitor_position.y));
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor())); // this MUST be positive parameters
    }
    _is_borderless = !_is_borderless;
}


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


//Hard texture resize
void texture2d_resize(Texture2D  *texture){


    //иногда пиксели подглючивают(в максимайзд например)
    static int tex_bunny_width = texture->width;
    static int tex_bunny_height = texture->height;

    //this is not fixing artefacts
//    player_image = LoadImage("Game_Data/player.png");
//    ImageResizeNN(&player_image,tex_bunny_width * scale_x, tex_bunny_height * scale_y);
//    UnloadTexture(player_texture);
//    player_texture = LoadTextureFromImage(player_image);

    texture->width = tex_bunny_width * scale_x ;
    texture->height = tex_bunny_height * scale_y;

}





// Crosshair in the center of screen demonstrating current window moonitor
void draw_screen_center(){
    DrawRectangle(initial_window_width / 2 - 2, initial_window_height / 2 - 35, 5, 70, RED);
    DrawRectangle(initial_window_width / 2 - 35, initial_window_height / 2 - 2, 70, 5, RED);
}


// Draw current connected gamepads (for debugging)
void draw_gamepads(){
    int max_gamepads = 4;// defined in raylib config - mb global
    for (int current_gamepad = 0; current_gamepad < max_gamepads; ++current_gamepad) {
        if (IsGamepadAvailable(current_gamepad)){
            DrawText(TextFormat("GP%current_gamepad: %s", current_gamepad + 1, GetGamepadName(current_gamepad)), 700, 10 + current_gamepad * 40, 30, BLACK);
        }
        else{
            DrawText(TextFormat("GP%current_gamepad: DISCONNECTED", current_gamepad + 1), 700, 10 + current_gamepad * 40, 30, BLACK);
        }
    }
    DrawText(TextFormat("CURRENT GAMEPAD: %i", active_gamepad + 1), 700, 160, 30, BLACK);
}


// Displays current gamepad dicsconnected warning
void gamepad_disconnect_warning(){
    if(!IsGamepadAvailable(active_gamepad)){
        DrawText("CURRENT GAMEPAD DISCONNECTED !!!", 50, 250, 60, RED);
    }
}







void switch_active_gamepad(){
    if (IsKeyPressed(KEY_ONE) && (IsKeyDown(KEY_G))){
        active_gamepad = 0;
    }
    else if (IsKeyPressed(KEY_TWO) && (IsKeyDown(KEY_G))){
        active_gamepad = 1;
    }
    else if (IsKeyPressed(KEY_THREE) && (IsKeyDown(KEY_G))){
        active_gamepad = 2;
    }
    else if (IsKeyPressed(KEY_FOUR) && (IsKeyDown(KEY_G))){
        active_gamepad = 3;
    }
}





