#include <iostream>

// Global variables (use 'extern' if breaks)
#define TARGET_FPS 120
#define TILESIZE 64

//Pre-underscore-d values are internal config flags(states) - modify them cautiously
bool _is_borderless = false;
bool _fps_lock = true;
bool _draw_debug_info = false;
int total_errors = 0;
int window_width = 1280; // maybe '_' (internal) too
int window_height = 720;
//initial "source" screen resolution
const int initial_window_width = window_width;
const int initial_window_height = window_height;
Vector2 window_position;  //Position of game window on monitor in pixels

float scale_x = 1.0f;
float scale_y = 1.0f;

int active_gamepad = 0;
const int max_gamepads = 4;//@Unresolved - defined in raylib config - not hooking for some reason



#define ACCELERATION 600.0
#define GRAVITATION 2000.0
#define INITIAL_FALLING_TIME 0.001 // initial acceleration time of free fall - for faster falling


//#define BUTTON_TOGGLE_BORDERLESS KEY_F || GAMEPAD_BUTTON_LEFT_FACE_LEFT || MOUSE_BUTTON_MIDDLE
#define MAX_BUTTON_BINDINGS 5
std::string BUTTON_TOGGLE_BORDERLESS[MAX_BUTTON_BINDINGS]  {"KEY_F", "MOUSE_BUTTON_MIDDLE", "KEY_A","GAMEPAD_BUTTON_LEFT_FACE_LEFT", "GAMEPAD_BUTTON_RIGHT_FACE_LEFT"};

struct Player{
    float x = 0;
    float y = 0;
    float width = 64;
    float height = 64;
    float speed_x = 0; //speed is the distance(step) player steps every frame
    float speed_y = 0;
    float acceleration_left = 0;
    float acceleration_right = 0;
    float acceleration_up = 0;
    float acceleration_down = 0;
    bool is_standing = false;
    bool is_levitating = false; //for now only happens when dragging player with mouse "in debug mode"

    float falling_time = 0;
};

struct Game_Level{
    static const int width = 20; // Level dimensions in 64x64 grid @Unfinished - Needs to be loaded by load level dimensions func
    static const int height = 11;
    char data[width][height] = {}; // matrix containing rectangular level grid (mb need to make this array dynamic later)
};

//TODO declare all functions here
//void process_input(Player& player);
//void switch_active_gamepad();