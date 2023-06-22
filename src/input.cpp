

void toggle_framelock(){
    if (_fps_lock){
        SetTargetFPS(0);
    }
    else{
        SetTargetFPS(TARGET_FPS);
    }
    _fps_lock = ! _fps_lock;
}


void toggle_borderless(){
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
    _is_borderless = ! _is_borderless;
}


void toggle_fullscreen(){ // not finished - work in progress
    //this is fullscreen - works fine entering fullscreen but cant exit fullscreen, ToggleFullscreen works like shit
    // because refresh rate is not specified. Need to mess with RayLib functions(or add new) to make this work.
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
    if (IsKeyPressed(KEY_Q) && (IsKeyDown(KEY_LEFT_CONTROL))){
        ToggleFullscreen();
    }
}


// Switch active used gamepad from 1 up to 4
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

// All user input
void process_input(Player& player){
    switch_active_gamepad();
    int scaled_mouse_x = GetMouseX() / scale_x;
    int scaled_mouse_y = GetMouseY() / scale_y;

    if (IsKeyPressed(KEY_I)) _draw_debug_info = !_draw_debug_info;
    if (IsKeyPressed(KEY_L)) toggle_framelock();
    if (IsKeyPressed(KEY_F)) toggle_borderless();
    toggle_fullscreen(); // not finished


    if (IsKeyDown(KEY_W) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)){ player.acceleration_up = ACCELERATION;}
    else{player.acceleration_up = 0;}
    if (IsKeyDown(KEY_S) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)){ player.acceleration_down = ACCELERATION;}
    else{player.acceleration_down = 0;}
    if (IsKeyDown(KEY_A) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)){ player.acceleration_left = ACCELERATION;}
    else{player.acceleration_left = 0;}
    if (IsKeyDown(KEY_D) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)){ player.acceleration_right = ACCELERATION;}
    else{player.acceleration_right = 0;}

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        player.x = scaled_mouse_x;
        player.y = scaled_mouse_y;
    }
}

