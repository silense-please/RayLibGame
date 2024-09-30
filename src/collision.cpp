
//Display collision of static objects
void show_collision(Player player, Game_Level level) {
    float width = TILESIZE;
    float height = TILESIZE;
    for (int index_x = 0; index_x < level.width; ++index_x) {
        for (int index_y = 0; index_y < level.height; ++index_y) {
            float x = index_x * TILESIZE;
            float y = index_y * TILESIZE;
            if (level.data[index_x][index_y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                       (Rectangle){x, y, width, height}))
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
    player.is_standing = false;
    float width = TILESIZE;
    float height = TILESIZE;
    for (int index_x = 0; index_x < level.width; ++index_x) {
        for (int index_y = 0; index_y < level.height; ++index_y) {
            float x = index_x * TILESIZE;
            float y = index_y * TILESIZE;
            if (level.data[index_x][index_y] == 'G') {
                // Down collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y + player.height, player.width, player.height},                    (Rectangle){x, y, width, height})){ // Check if player is one tile away from collision or closer
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                        (Rectangle){x, y, width, height})){  // If player is NOT stuck in collision
                        if(player.speed.y * delta_time > y - (player.y + player.height)) { //Apply collision - if player wants to move into collision, move him right next to it
                            player.speed.y = (y - (player.y + player.height)) / delta_time;
                        }
                    }
                    //Check if player is standing on the ground
                    const float COLLISION_EPSILON = 0.01f; // there is a FloatEquals() func in raymath.h
                    if (abs(player.y + player.height - y) <= COLLISION_EPSILON && !player.started_jumping){
                        player.is_standing = true;
                    }
                }
                //Up collision
                if (CheckCollisionRecs((Rectangle){player.x, player.y - player.height, player.width, player.height},
                                       (Rectangle){x, y, width, height})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){x, y, width, height})){
                        if(-player.speed.y *delta_time > player.y - (y + height))
                            player.speed.y = -(player.y - (y + height)) /delta_time;
                    }
                }
                //Right collision
                if (CheckCollisionRecs((Rectangle){player.x + player.width, player.y, player.width, player.height},
                                       (Rectangle){x, y, width, height})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){x, y, width, height})){
                        if(player.speed.x *delta_time> x - (player.x + player.width))
                            player.speed.x = (x - (player.x + player.width)) /delta_time;
                    }
                }
                //Left collision
                if (CheckCollisionRecs((Rectangle){player.x - player.width, player.y, player.width, player.height},
                                       (Rectangle){x, y, width, height})){
                    if(!CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                                           (Rectangle){x, y, width, height})){
                        if(-player.speed.x *delta_time > player.x - (x + width))
                            player.speed.x = -(player.x - (x + width)) /delta_time;
                    }
                }
            }
        }
    }
}

//If player is stuck inside collision push him out
void prevent_collision_stuck(Player &player, Game_Level level){
    //player.is_standing = false;
    float width = TILESIZE;
    float height = TILESIZE;
    for (int index_x = 0; index_x < level.width; ++index_x) {
        for (int index_y = 0; index_y < level.height; ++index_y) {
            float x = index_x * TILESIZE;
            float y = index_y * TILESIZE;
            if (level.data[index_x][index_y] == 'G') {
                if (CheckCollisionRecs((Rectangle){player.x, player.y, player.width, player.height},
                    (Rectangle){x, y, width, height})){ // Check if player collides with block
                    if (abs(y - player.y) < abs(x - player.x)){ //vertical or horizontal
                        if (x > player.x) {
                            player.x = x - player.width;  //RIGHT COLLISION
                        }else { player.x = x + width ; } //LEFT COLLISION

                    }else {
                        if (y > player.y){  // DOWN COLLISION
                            player.y = y - player.height;  //Apply collision - if player wants to move into collision, move him right next to it
                            //Confirm that player is standing on the ground
                            player.is_standing = true;
                        } else { player.y = y + height;  } // UP COLLISION
                    }
                }
            }
        }
    }
}