
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

void ground_collision(Player &player, Game_Level level) {
    for (int x = 0; x < level.width; ++x ) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + TILESIZE, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_y > y*TILESIZE - (player.y + TILESIZE))
                            player.speed_y = y*TILESIZE - (player.y + TILESIZE);
                    }
                    else
                        player.speed_y = -0.2 * GetFrameTime() * ACCELERATION; //to push out if stuck (actually this is suppoused to be an error cause player shouldn't be stuck in collided objects)
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - TILESIZE, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_y > player.y - (y*TILESIZE + TILESIZE))
                            player.speed_y = -(player.y - (y*TILESIZE + TILESIZE));
                    }
                    else
                        player.speed_y = 0.2 * GetFrameTime() * ACCELERATION; //to push out if stuck
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x+ TILESIZE, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_x > x*TILESIZE - (player.x + TILESIZE))
                            player.speed_x = x*TILESIZE - (player.x + TILESIZE);
                    }
                    else
                        player.speed_x = -0.2 * GetFrameTime() * ACCELERATION; //to push out if stuck
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - TILESIZE, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_x > player.x - (x*TILESIZE + TILESIZE))
                            player.speed_x = -(player.x - (x*TILESIZE + TILESIZE));
                    }
                    else
                        player.speed_x = 0.2 * GetFrameTime() * ACCELERATION; //to push out if stuck
                }

            }
        }
    }
}