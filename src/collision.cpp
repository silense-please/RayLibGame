
//Display collision of static objects
void show_collision(Player player, Game_Level level) {
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE}))
                    DrawText("COLLISION WITH GROUND", 350, 50, 50, YELLOW);
            }
        }
    }
}

//Prevents from out of bounds
void level_borders_collision(Player &player, Game_Level &level) {
    if(player.y+player.height > level.height*TILESIZE) player.y  = level.height*TILESIZE - player.height;
    if(player.y < 0) player.y = 0;
    if(player.x+player.width > level.width*TILESIZE) player.x = level.width*TILESIZE - player.width;
    if(player.x < 0) player.x = 0;
}

//Check & apply collision of static objects
//похорошему вместо 4ёх разных направлений надо проверять будет ли коллизия по вектору движения(добавить вектор скорости)
void static_object_collision_by_speed(Player &player, Game_Level level) {
    //@Refactor - in collison funcs there should be y_width, y_height etc instead of TILESIZE (int y_width = TILESIZE)
    player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + player.height, player.width, player.height}, // Check if player is one tile away from collision or closer
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, // If player is NOT stuck in collision
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_y *delta_time > y*TILESIZE - (player.y + player.height)) { //Apply collision - if player wants to move into collision, move him right next to it
                            player.speed_y = (y * TILESIZE - (player.y + player.height)) /delta_time;
                            //Check if player is standing on the ground
                            player.is_standing = true;
                        }
                    }
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - player.height, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_y *delta_time > player.y - (y*TILESIZE + TILESIZE))
                            player.speed_y = -(player.y - (y*TILESIZE + TILESIZE)) /delta_time;
                    }
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x + player.width, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_x *delta_time> x*TILESIZE - (player.x + player.width))
                            player.speed_x = (x*TILESIZE - (player.x + player.width)) /delta_time;
                    }
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - player.width, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_x *delta_time > player.x - (x*TILESIZE + TILESIZE))
                            player.speed_x = -(player.x - (x*TILESIZE + TILESIZE)) /delta_time;
                    }
                }
            }
        }
    }
}

//If player is stuck inside collision push him out
void static_object_collision_by_coordinates(Player &player, Game_Level level){ // VERSION 2
    //player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, // Check if player collides with block
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if (abs(y*TILESIZE - player.y) < abs(x*TILESIZE - player.x)){ //vertical or horizontal
                        if (x*TILESIZE > player.x) {
                            player.x = x*TILESIZE - player.width;  //RIGHT COLLISION
                        }else { player.x = x*TILESIZE + TILESIZE ; } //LEFT COLLISION

                    }else {
                        if (y*TILESIZE > player.y){  // DOWN COLLISION
                            player.y = y * TILESIZE - player.height;  //Apply collision - if player wants to move into collision, move him right next to it
                            //Confirm that player is standing on the ground
                            player.is_standing = true;
                        } else { player.y = y * TILESIZE + TILESIZE;  } // UP COLLISION
                    }
                }
            }
        }
    }
}