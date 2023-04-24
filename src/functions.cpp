
void toggle_borderless(){
    static Vector2 window_position; //= GetWindowPosition(); //better be global value

    if (_is_borderless){ //Restore Windowed mode
        SetWindowSize(screen_width, screen_height);
        SetWindowPosition(int(window_position.x), int(window_position.y));
        //SetWindowPosition(100,100); delete this, this is for debugging
        ClearWindowState(FLAG_WINDOW_UNDECORATED);
    }
    else{ //Set Borderless
        window_position = GetWindowPosition(); //a bit dangerous - vectors are float and position must be presize
        Vector2 monitor_position = GetMonitorPosition(GetCurrentMonitor()); //left-upper corner of current monitor
        screen_height = GetScreenHeight(); //or GetRenderHeight()???
        screen_width = GetScreenWidth();
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
