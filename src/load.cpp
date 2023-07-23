#include <fstream>
#include <iostream>


/* //not yet implemented
void load_level_dimensions(int &x, int &y){
    if(!file_stream.is_open()){
        printf("Failed to open the file!!!\n");
        //throw CustomException(file_stream, "File could not be opened");  ???what is throw
    }
    else{
        y = 0;
        std::string line;
        while (std::getline(file_stream, line)) {
            // Process each line of the file
            y++;
        }
        x = line.length();
        printf("file loaded (tmp message)");
    }
}
 */


// Load level from txt file to char matrix
void load_level(Game_Level &level){
    std::ifstream file_stream("Game_Data/level_0.txt", std::ios::in);
    //load_level_dimensions(level_x, level_y);

    if(!file_stream.is_open()){
        printf("Failed to open the file!!!\n!!!\n");
    }
    else{
        std::string line;
        int y = 0;
        while (std::getline(file_stream, line)) {
            if (line.length() < level.width){ //if level length is smaller than specified, string '[]' operator has undefined behavior
                printf("!!!ERROR: Level loading failed, string length is incorrect\n");
                total_errors++;
                //break; //TODO make this a proper exeption
            }
            for (int x = 0; x < line.length(); ++x) { // mb make it x < line.lenght()
                level.data[x][y] = line[x];
            }
            y++;
        }
    }
    file_stream.close();
}

//Load player spawnpoint in the level if specified
void load_player_spawn(Game_Level &level, Player &player){
    for (int x = 0; x < level.width; ++x) {
        for (int y = 0; y < level.height; ++y) {
            if (level.data[x][y] == 'P') {
                player.x = x * 64;
                player.y = y * 64;
            }
        }
    }
}