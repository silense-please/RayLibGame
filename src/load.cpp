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
void load_level(char (&level)[level_x][level_y], Player &player){
    std::ifstream file_stream("Game_Data/level_0.txt", std::ios::in);

    //load_level_dimensions(level_x, level_y);

    if(!file_stream.is_open()){
        printf("Failed to open the file!!!\n!!!\n");
    }
    else{
        std::string line;
        int y = 0;
        while (std::getline(file_stream, line)) {
            if (line.length() < level_x){ //if level length is smaller than specified, string '[]' operator has undefined behavior
                printf("ERROR: Level loading failed, level length is incorrect\n");
                break; //TODO make this a proper exeption
            }
            for (int x = 0; x < level_x; ++x) {
                level[x][y] = line[x];
                if (line[x] == 'P') {
                    player.x = x * 64;
                    player.y = y * 64;
                }
            }
            y++;
        }
    }
    file_stream.close();
}
