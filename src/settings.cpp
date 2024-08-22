
void toggle_framelock(){
    if (_fps_lock){SetTargetFPS(0);}
    else{SetTargetFPS(TARGET_FPS);}
    _fps_lock = ! _fps_lock;
}

void toggle_vsync(){
    if (_vsync){ ClearWindowState(FLAG_VSYNC_HINT);}
    else{SetWindowState(FLAG_VSYNC_HINT);}
    _vsync = ! _vsync;
    //@Bug: Something weird is going on whith vsync (input lag and high cpu usage) but if you target lock to monitor refresh rate first and then turn on vsync it's fine
    // useful stuff: GetMonitorRefreshRate() - to lock fps with that before vsyncing
    //DrawRectangle(scaled_mouse.x, scaled_mouse_y,5,5,RED ); - to see input lag
}

void toggle_borderless(){ // Great, Raylib has his own ToggleBorderlessWindowed() now ...
    static int saved_width = GetScreenWidth();
    static int saved_height =  GetScreenHeight();
    if (_is_borderless){ //Restore Windowed mode
        SetWindowSize(saved_width, saved_height);
        SetWindowPosition(int(window_position.x), int(window_position.y));
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
    }
    else{ //Set Borderless
        window_position = GetWindowPosition();
        Vector2 monitor_position = GetMonitorPosition(GetCurrentMonitor());
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