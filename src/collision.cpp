
//Display collision of static objects
void show_collision(Player player, Game_Level level) {
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE}))
                    DrawText("COLLISION WITH GROUND", 100, 200, 50, YELLOW);
            }
        }
    }
}

//Prevents from out of bounds
void level_borders_collision(Player &player, Game_Level &level) {
    if(player.y+player.height > level.height*TILESIZE) player.y  = level.height*TILESIZE - player.height; // this should be level height instead of initial_window_height
    if(player.y < 0) player.y = 0;
    if(player.x+player.width > level.width*TILESIZE) player.x = level.width*TILESIZE - player.width; // this should be level width instead of initial_window_width
    if(player.x < 0) player.x = 0;
}

//Check & apply collision of static objects
void static_object_collision(Player &player, Game_Level level) {
    player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                bool stuck_up = 0, stuck_down = 0, stuck_left = 0, stuck_right = 0;
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + TILESIZE, player.width, player.height}, // Check if player is one tile away of collision or closer
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_y > y*TILESIZE - (player.y + player.height)) //Apply collision - if player wants to move into collision, move him right next to it
                            player.speed_y = y*TILESIZE - (player.y + player.height);
                    } else
                        stuck_down = 1; //to push out if stuck (actually this is suppoused to be an error cause player shouldn't be stuck in collided objects)
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - TILESIZE, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_y > player.y - (y*TILESIZE + TILESIZE))
                            player.speed_y = -(player.y - (y*TILESIZE + TILESIZE));
                    } else
                        stuck_up = 1;
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x + TILESIZE, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_x > x*TILESIZE - (player.x + player.height))
                            player.speed_x = x*TILESIZE - (player.x + player.height);
                    } else
                        stuck_right = 1;
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - TILESIZE, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_x > player.x - (x*TILESIZE + TILESIZE))
                            player.speed_x = -(player.x - (x*TILESIZE + TILESIZE));
                    } else
                        stuck_left = 1; //to push out if stuck
                }
                //If player is stuck inside collision
                if(stuck_up || stuck_down || stuck_left || stuck_right){
                    player.speed_y = (stuck_up - stuck_down) * 0.2 * GetFrameTime() * ACCELERATION;
                    player.speed_x = (stuck_left - stuck_right) * 0.2 * GetFrameTime() * ACCELERATION;
                }
                //Check if player is standing on the ground
                if(player.y + player.height == y*TILESIZE){
                    player.is_standing = true;
                    player.falling_time = INITIAL_FALLING_TIME;
                }
            }
        }
    }
}