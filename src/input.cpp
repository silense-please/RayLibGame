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
    toggle_fullscreen(); //@Unfinished


    if (IsKeyPressed(KEY_W) || IsGamepadButtonPressed(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)){
        if(player.is_standing && player.speed_y == 0){
            player.acceleration_up = GRAVITATION * 500;
        }
    }
    else{player.acceleration_up = 0;}
    if (IsKeyDown(KEY_A) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)){ player.acceleration_left = ACCELERATION;}
    else{player.acceleration_left = 0;}
    if (IsKeyDown(KEY_D) || IsGamepadButtonDown(active_gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)){ player.acceleration_right = ACCELERATION;}
    else{player.acceleration_right = 0;}

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        player.x = scaled_mouse_x;
        player.y = scaled_mouse_y;
        player.is_levitating = true;
        player.falling_time = INITIAL_FALLING_TIME;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        player.is_levitating = false;
}

