// Global variables (use 'extern' if breaks)

bool _is_borderless = false;
bool _fps_lock = true;
int window_width = 1280; // maybe '_' (internal) too
int window_height = 720;
//initial "source" screen resolution
const int initial_window_width = window_width;
const int initial_window_height = window_height;
Vector2 window_position;  //Position of game window on monitor in pixels

float scale_x = 1.0f;
float scale_y = 1.0f;

int active_gamepad = 0;
const int max_gamepads = 4;// defined in raylib config - not hooking for some reason

struct Player{
    float x = 0;
    float y = 0;
    float speed = 0;
    const float acceleration = 800.0f;
};

//TODO declare all functions here
//void process_input(Player& player);
//void switch_active_gamepad();