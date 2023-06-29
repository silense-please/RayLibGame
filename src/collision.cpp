
//Collision of static level objects
void detect_collision(Player player, Game_Level level) {
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE}))
                    DrawText("COLLISION WITH GROUND", 100, 200, 50, YELLOW);
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

void static_object_collision(Player &player, Game_Level level) {
    player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                bool stuck_up = 0, stuck_down = 0, stuck_left = 0, stuck_right = 0;
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + TILESIZE, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_y > y*TILESIZE - (player.y + TILESIZE))
                            player.speed_y = y*TILESIZE - (player.y + TILESIZE);
                    }
                    else
                        stuck_down = 1; //to push out if stuck (actually this is suppoused to be an error cause player shouldn't be stuck in collided objects)
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - TILESIZE, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_y > player.y - (y*TILESIZE + TILESIZE))
                            player.speed_y = -(player.y - (y*TILESIZE + TILESIZE));
                    }
                    else
                        stuck_up = 1;
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x+ TILESIZE, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_x > x*TILESIZE - (player.x + TILESIZE))
                            player.speed_x = x*TILESIZE - (player.x + TILESIZE);
                    }
                    else
                        stuck_right = 1;
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - TILESIZE, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_x > player.x - (x*TILESIZE + TILESIZE))
                            player.speed_x = -(player.x - (x*TILESIZE + TILESIZE));
                    }
                    else
                        stuck_left = 1; //to push out if stuck
                }
                //If player is stuck inside collision
                if(stuck_up || stuck_down || stuck_left || stuck_right){
                    player.speed_y = (stuck_up - stuck_down) * 0.2 * GetFrameTime() * ACCELERATION;
                    player.speed_x = (stuck_left - stuck_right) * 0.2 * GetFrameTime() * ACCELERATION;
                }
                //Check if player is standing on the ground
                if(player.y + TILESIZE == y*TILESIZE){
                    player.is_standing = true;
                    player.falling_time = INITIAL_FALLING_TIME;
                }

            }
        }
    }
}