
// Draw current connected gamepads (for debugging)
void draw_gamepads(){
    for (int current_gamepad = 0; current_gamepad < max_gamepads; ++current_gamepad) {
        if (IsGamepadAvailable(current_gamepad)){
            DrawText(TextFormat("GP%d: %s", current_gamepad + 1, GetGamepadName(current_gamepad)), 700, 10 + current_gamepad * 40, 30, BLACK);
        }
        else{
            DrawText(TextFormat("GP%d: DISCONNECTED", current_gamepad + 1), 700, 10 + current_gamepad * 40, 30, BLACK);
        }
    }
    DrawText(TextFormat("CURRENT GAMEPAD: %d", active_gamepad + 1), 700, 160, 30, BLACK);
}

// Displays current gamepad dicsconnected warning
void gamepad_disconnect_warning(){
    int count = 0;
    for (int gamepad = 0; gamepad < max_gamepads; ++gamepad) {
        if(IsGamepadAvailable(gamepad))
            count++;
    }
    if((count > 0) && (!IsGamepadAvailable(active_gamepad))){
        DrawText("CURRENT GAMEPAD DISCONNECTED !!!", 50, 250, 60, RED);
    }
}


// Draw all debugging text
void draw_debug_info(){
    //@ Add more stages here for displaying different info like: input, jump times, gamepads and screen stuff, ... /
    //  to just switch what you need with hotkeys (if Alt+S draw screen stuff... and so on)
    { /// Draw screen info
        if (IsWindowFullscreen()) {
            DrawText("WINDOW IS FULLSCREEN", 200, 40, 40, RED);
        } else if (_is_borderless) {
            DrawText("WINDOW IS BORDERLESS", 200, 40, 40, RED);
        } else if (IsWindowMaximized()) {
            DrawText("WINDOW IS 'MAXIMIZED'", 200, 40, 40, RED);
        }
        DrawText(TextFormat("CURRENT MONITOR: %i", GetCurrentMonitor()), 200, 0, 40, RED);
    }
    draw_gamepads();
    gamepad_disconnect_warning();

}


void static_objects_draw(Game_Level level, Texture2D &texture, char object_symbol){
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == object_symbol)
                DrawTextureEx(texture, (Vector2){(float)x*TILESIZE, (float)y*TILESIZE}, 0, 1, GRAY);
        }
    }
}

// Draw animation from one row spritesheet
void draw_player_animation(Player player, Animation &animation){
    int frames = animation.texture.width / TILESIZE;
    int flip = player.facing_left ? -1: 1; //minus(-) in source's width mirrors the image

    Rectangle run_frame_rec = {(float)animation.current_frame*(float)animation.texture.width / frames,0, (float)animation.texture.width / frames * flip, (float)animation.texture.height};
    DrawTextureRec(animation.texture,run_frame_rec , (Vector2) {(float) player.x - player.width/2, (float) player.y}, WHITE);
    animation.timer += delta_time;

    if(_draw_debug_info){ // Visualising to see if animations are working properly
        DrawText(TextFormat("%d", animation.current_frame ), player.x, player.y - 20, 5, animation.current_frame%2 == 0?LIME:BLUE);
        DrawText(TextFormat("timer: %f", animation.timer), player.x, player.y - 40, 20, animation.current_frame%2 == 0?LIME:BLUE);
    }

    if (animation.timer >= animation.speed){animation.current_frame ++; animation.timer = 0;} // Proceed to the next frame

    if (animation.current_frame > frames-1) {
        if (animation.is_looping){ // Loop animation.
            animation.current_frame = 0;
        } else{ // Animation is finished.
            animation.is_playing = false;
            animation.current_frame = 0;
            animation.timer = 0;
        }
    }
}

// Reset animation if it's finished.
void reset_animation(Animation &animation){
    if (!animation.is_playing){
        animation.current_frame = 0;
        animation.timer = 0;
    }
}
