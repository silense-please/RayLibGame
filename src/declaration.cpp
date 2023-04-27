// Global variables


bool _is_borderless = false; // this better be global value (use 'extern')
int window_width = 1280; // maybe '_' (internal) too
int window_height = 720;

//initial "source" screen resolution - maybe move to global
const float initial_window_width = window_width;
const float initial_window_height = window_height;

float scale_x = 1.0f; // scales should be global i guess
float scale_y = 1.0f;
