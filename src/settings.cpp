//Apply window contents scaling
void apply_screen_scale(){
    if(IsWindowResized()) {
        scale_x = (float)GetScreenWidth() / initial_window_width;
        scale_y = (float)GetScreenHeight() / initial_window_height;
    }
}

void toggle_framelock(){
    if (_fps_lock){SetTargetFPS(0);}
    else{SetTargetFPS(TARGET_FPS);}
    _fps_lock = ! _fps_lock;
}

void toggle_vsync(){
    if (_vsync){ ClearWindowState(FLAG_VSYNC_HINT);}
    else{SetWindowState(FLAG_VSYNC_HINT);}
    _vsync = ! _vsync;
}

void toggle_borderless(){
    static int saved_width = GetScreenWidth();
    static int saved_height =  GetScreenHeight();
    if (_is_borderless){ //Restore Windowed mode
        SetWindowSize(saved_width, saved_height);
        SetWindowPosition(int(window_position.x), int(window_position.y));
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
    }
    else{ //Set Borderless
        window_position = GetWindowPosition();
        Vector2 monitor_position = GetMonitorPosition(GetCurrentMonitor()); //Current monitor is the one where the center of window is
        saved_width = GetScreenWidth();
        saved_height = GetScreenHeight();
        //^^^ Remember window state before turning on borderless ^^^

        SetWindowState(FLAG_WINDOW_UNDECORATED);
        SetWindowPosition(int(monitor_position.x),int(monitor_position.y));
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()),GetMonitorHeight(GetCurrentMonitor())); // this MUST be positive parameters
    }
    _is_borderless = ! _is_borderless;
}


void toggle_fullscreen(){ // @Unfinished - work in progress
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