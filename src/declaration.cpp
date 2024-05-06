// @ sign - tags & reminders to cleanup\change the code later

#include "raylib.h"
#include <fstream>
#include <iostream>
//#include <cmath> //DO I NEED THIS ?
using std::string;
using std::abs;


// Global variables (use 'extern' if breaks)
#define TARGET_FPS 120
#define TILESIZE 64.0f

//@ Pre-Underscore all global variables?
//Pre-underscore-d variables are internal config flags(states) - modify them cautiously
bool _is_borderless = false;
bool _fps_lock = true;
bool _vsync = true;
bool _draw_debug_info = false;
bool _is_paused = false;
bool _is_menu = false;
bool free_cam = false; // free camera for debug mode
int total_errors = 0;
int window_width = 1280; // maybe '_' (internal) too
int window_height = 720;
//initial "source" screen resolution
const int initial_window_width = window_width;
const int initial_window_height = window_height;
Vector2 window_position;  //Position of game window on monitor in pixels
// window resolution scale
float scale_x = 1.0f;
float scale_y = 1.0f;
double delta_time = 0; //in-game physics time delta in seconds (could be paused or reversed or slowed)

float scaled_mouse_x = 0;
float scaled_mouse_y = 0;

int active_gamepad = 0;
const int max_gamepads = 4;//@Unresolved - MAX_GAMEPADS defined in raylib config - not hooking for some reason


#define GAME_SPEED 1 // for slow-mo
#define WALK_SPEED 800.0f
#define GRAVITATION 2000.0f
#define INITIAL_FALLING_TIME 0.00 // initial acceleration time of free fall - for faster falling
#define JUMP_INPUT_BUFFER_TIME 0.07
#define COYOTE_TIME 0.07
#define JUMP_POWER 1000
#define MIN_JUMP_INPUT_HOLD_TIME 0.15
#define MAX_JUMP_INPUT_HOLD_TIME 0.7
#define MAX_FALL_SPEED 1600.0


//Input bindings - be wary of same key duplicating to multiple buttons
#define MAX_BUTTON_BINDINGS 5
string BUTTON_TOGGLE_BORDERLESS[MAX_BUTTON_BINDINGS]  {"KEY_F","GAMEPAD_BUTTON_RIGHT_FACE_LEFT"};
string BUTTON_DEBUG_INFO[MAX_BUTTON_BINDINGS]  {"KEY_I"};
string BUTTON_FRAMELOCK[MAX_BUTTON_BINDINGS]  {"KEY_L"};
string BUTTON_VSYNC[MAX_BUTTON_BINDINGS]  {"KEY_V"};
string BUTTON_FREECAM[MAX_BUTTON_BINDINGS]  {"KEY_R"};
string BUTTON_PAUSE[MAX_BUTTON_BINDINGS]  {"KEY_ESCAPE", "GAMEPAD_BUTTON_MIDDLE_RIGHT"};

string BUTTON_LMB[MAX_BUTTON_BINDINGS]  {"MOUSE_BUTTON_LEFT"}; // You can switch mouse buttons if you're lefty
string BUTTON_RMB[MAX_BUTTON_BINDINGS]  {"MOUSE_BUTTON_RIGHT"};
string BUTTON_MMB[MAX_BUTTON_BINDINGS]  {"MOUSE_BUTTON_MIDDLE"};





string BUTTON_MOVE_RIGHT[MAX_BUTTON_BINDINGS]  {"KEY_D", "KEY_RIGHT", "GAMEPAD_BUTTON_LEFT_FACE_RIGHT"};
string BUTTON_MOVE_LEFT[MAX_BUTTON_BINDINGS]  {"KEY_A", "KEY_LEFT","GAMEPAD_BUTTON_LEFT_FACE_LEFT"};
string BUTTON_JUMP[MAX_BUTTON_BINDINGS]  {"KEY_SPACE", "GAMEPAD_BUTTON_RIGHT_FACE_DOWN"};


struct Player{
    float x = 0;
    float y = 0;
    float width = 32;
    float height = TILESIZE;
    float speed_x = 0; //speed is the distance(step) player steps every frame
    float speed_y = 0; //@Refactor - make it Vector(?) - player.speed.x
    float acceleration_left = 0;
    float acceleration_right = 0;
    float acceleration_up = 0; // maybe just horizontal acceleration
    float acceleration_down = GRAVITATION;
    bool direction = 1; //  horizontal image flip - left = 0, right = 1

    bool is_standing = false; //standing on the ground
    bool is_holding_jump = false;
    bool is_jumping = false;
    bool is_levitating = false; //when player is not falling (noclip, etc) (no use now)

    bool is_running = false;

    double falling_time = 0; //@Rename? to time_of_fall
    double air_time = 0;
    double jump_input_buffer_time = 0;
    double jump_input_hold_time = 0;
};

struct Game_Level{
    static const int width = 60; // Level dimensions in 64x64 grid @Unfinished - Needs to be loaded by load level dimensions func
    static const int height = 18;
    char data[width][height] = {}; // matrix containing rectangular level grid (mb need to make this array dynamic later)
};

struct Menu_Button{
    int id; // menu_button enum
    const char *label; //for some reason DrawText requires a const char* instead of string, but it's fine
    float x;
    float y;
    float width = 250;
    float height = 80;
    bool state = 0; // 0 - steady, 1 - mouse_hover
    Color color = DARKGREEN;
};

enum menu_button{
    BTN_RESUME = 0,
    BTN_QUIT,

};

// Settings functions
void apply_screen_scale();
void toggle_framelock();
void toggle_borderless();
void toggle_fullscreen();

// Load functions
void load_level_dimensions(int &x, int &y);
void load_level(Game_Level &level);
void load_player_spawn(Game_Level &level, Player &player);

// Input functions
bool IsButtonPressed(string buttons[MAX_BUTTON_BINDINGS]);
bool IsButtonReleased(string buttons[MAX_BUTTON_BINDINGS]);
bool IsButtonUp(string buttons[MAX_BUTTON_BINDINGS]);
bool IsButtonDown(string buttons[MAX_BUTTON_BINDINGS]);
void switch_active_gamepad();
void process_input(Player &player, Camera2D &camera, Game_Level current_level);

// Collision functions
void show_collision(Player player, Game_Level level);
void level_borders_collision(Player &player, Game_Level &level);
void static_object_collision_by_speed(Player &player, Game_Level level);
void static_object_collision_by_coordinates(Player &player, Game_Level level);

// Draw functions
void draw_screen_center();
void draw_gamepads();
void gamepad_disconnect_warning();
void draw_debug_info();
void static_objects_draw(Game_Level level, Texture2D texture, char object_symbol);
void draw_target_texture(RenderTexture2D& target);


