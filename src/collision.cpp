
//Collision of static level objects
void detect_collision(Player player, Game_Level level) {
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, 64, 64}, (Rectangle){(float)x*64, (float)y*64, 64, 64})){
                    DrawText("COLLISION WITH GROUND", 100, 200, 50, YELLOW);
                }
            }
        }
    }
}

//Prevents from out of bounds
void level_borders_collision(Player &player) {
    if(player.y+64 > initial_window_height) player.y  = initial_window_height - 64; // this should be level height instead of initial_window_height
    if(player.y < 0) player.y = 0;
    if(player.x+64 > initial_window_width) player.x = initial_window_width - 64; // this should be level width instead of initial_window_width
    if(player.x < 0) player.x = 0;
}