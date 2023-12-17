
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
    if(player.y+player.height > level.height*TILESIZE) player.y  = level.height*TILESIZE - player.height;
    if(player.y < 0) player.y = 0;
    if(player.x+player.width > level.width*TILESIZE) player.x = level.width*TILESIZE - player.width;
    if(player.x < 0) player.x = 0;
}

//Check & apply collision of static objects - @!!!коллизии говно, они нифига не оптимизируют, всё равно пробегается по каждому блоку а в коде насрано
void static_object_collision_by_speed(Player &player, Game_Level level) {
    player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                bool stuck_up = 0, stuck_down = 0, stuck_left = 0, stuck_right = 0;
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + player.height, player.width, player.height}, // Check if player is one tile away from collision or closer
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, // If player is NOT stuck in collision
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_y *delta_time > y*TILESIZE - (player.y + player.height)) { //Apply collision - if player wants to move into collision, move him right next to it
                            player.speed_y = (y * TILESIZE - (player.y + player.height)) /delta_time;
                            //Check if player is standing on the ground
                            player.is_standing = true;
                            player.falling_time = INITIAL_FALLING_TIME;
                        }
                    } else
                        stuck_down = 1; //to push out if stuck (actually this is suppoused to be an error cause player shouldn't be stuck in collided objects)
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - player.height, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_y *delta_time > player.y - (y*TILESIZE + TILESIZE))
                            player.speed_y = -(player.y - (y*TILESIZE + TILESIZE)) /delta_time;
                    } else
                        stuck_up = 1;
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x + player.width, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(player.speed_x *delta_time> x*TILESIZE - (player.x + player.width))
                            player.speed_x = (x*TILESIZE - (player.x + player.width)) /delta_time;
                    } else
                        stuck_right = 1;
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - player.width, player.y, player.width, player.height},
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                        if(-player.speed_x *delta_time > player.x - (x*TILESIZE + TILESIZE))
                            player.speed_x = -(player.x - (x*TILESIZE + TILESIZE)) /delta_time;
                    } else
                        stuck_left = 1; //to push out if stuck
                }
                //If player is stuck inside collision
                if(stuck_up || stuck_down || stuck_left || stuck_right){ //@Broken - should not be pushing diagonally
                    player.speed_y = (stuck_up - stuck_down) * delta_time * ACCELERATION;
                    player.speed_x = (stuck_left - stuck_right) * delta_time * ACCELERATION;
                }

//                if(player.y + player.height == y*TILESIZE){  //@ DELETE THIS
//                    player.is_standing = true;
//                    player.falling_time = INITIAL_FALLING_TIME;
//                }
            }
        }
    }
}




void static_object_collision_by_coordinates(Player &player, Game_Level level){ // VERSION 2
    player.is_standing = false;
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height}, // Check if player collides with block
                                       (Rectangle){(float)x*TILESIZE, (float)y*TILESIZE, TILESIZE, TILESIZE})){
                    if (abs(y*TILESIZE - player.y) < abs(x*TILESIZE - player.x)){ //vertical or horizontal
                        if (x*TILESIZE > player.x) {
                            player.x = x*TILESIZE - player.width;  player.speed_x = 0;  //RIGHT COLLISION
                        }else { player.x = x*TILESIZE + TILESIZE ; player.speed_x = 0; } //LEFT COLLISION

                    }else {
                        if (y*TILESIZE > player.y){  // DOWN COLLISION
                            player.y = y * TILESIZE - player.height;  //Apply collision - if player wants to move into collision, move him right next to it
                            //Confirm that player is standing on the ground
                            player.is_standing = true;
                            player.speed_y = 0;
                            player.falling_time = INITIAL_FALLING_TIME;
                        }
                        else { player.y = y * TILESIZE + TILESIZE; player.speed_y = 0; } // UP COLLISION
                    }
                }
            }
        }
    }
}