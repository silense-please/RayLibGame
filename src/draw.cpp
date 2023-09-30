//Hard texture resize
void texture2d_resize(Texture2D& texture){

    //иногда пиксели подглючивают(в максимайзд например)
    static int tex_bunny_width = texture.width;
    static int tex_bunny_height = texture.height;

    //this is not fixing artefacts
//    player_image = LoadImage("Game_Data/player.png");
//    ImageResizeNN(&player_image,tex_bunny_width * scale_x, tex_bunny_height * scale_y);
//    UnloadTexture(player_texture);
//    player_texture = LoadTextureFromImage(player_image);

    texture.width = tex_bunny_width * scale_x ;
    texture.height = tex_bunny_height * scale_y;

}

// Crosshair in the center of screen demonstrating current window moonitor
void draw_screen_center(){
    DrawRectangle(initial_window_width / 2 - 2, initial_window_height / 2 - 35, 5, 70, RED);
    DrawRectangle(initial_window_width / 2 - 35, initial_window_height / 2 - 2, 70, 5, RED);
}

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
    if(IsWindowFullscreen()){
        DrawText("WINDOW IS FULLSCREEN", 200, 40, 40, RED);
    }
    else if(_is_borderless){
        DrawText("WINDOW IS BORDERLESS", 200, 40, 40, RED);
    }
    else if(IsWindowMaximized()){
        DrawText("WINDOW IS 'MAXIMIZED'", 200, 40, 40, RED);
    }

    DrawText(TextFormat( "CURRENT MONITOR: %i", GetCurrentMonitor()), 200, 0, 40, RED);
    //DrawText(TextFormat( "X: %i \nY: %i", int(GetWindowPosition().x),int(GetWindowPosition().y)), 200, 200, 20, DARKGRAY);
    DrawText(TextFormat( "FRAMETIME: %f", GetFrameTime() *1000), 10, 40, 20, LIME);
    draw_screen_center(); // make this toggle
    draw_gamepads();
    gamepad_disconnect_warning();
}


void static_objects_draw(Game_Level level, Texture2D texture, char object_symbol){
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == object_symbol)
                DrawTextureEx(texture, (Vector2){(float)x*64, (float)y*64}, 0, 1, WHITE);
        }
    }
}


// Draw resized target render texture on the screen (Final draw)
void draw_target_texture(RenderTexture2D& target){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Draw render texture to screen, properly scaled
    DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                   (Rectangle){(GetScreenWidth() - ((float)initial_window_width*scale_x))*0.5f, (GetScreenHeight() - ((float)initial_window_height*scale_y))*0.5f,
                               (float)initial_window_width*scale_x, (float)initial_window_height*scale_y }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}