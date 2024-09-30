#include <cassert>

int keyboard_key_int[] = {
        KEY_NULL            ,        // Key: NULL, used for no key pressed
        // Alphanumeric keys
        KEY_APOSTROPHE      ,       // Key: '
        KEY_COMMA           ,       // Key: ,
        KEY_MINUS           ,       // Key: -
        KEY_PERIOD          ,       // Key: .
        KEY_SLASH           ,       // Key: /
        KEY_ZERO            ,       // Key: 0
        KEY_ONE             ,       // Key: 1
        KEY_TWO             ,       // Key: 2
        KEY_THREE           ,       // Key: 3
        KEY_FOUR            ,       // Key: 4
        KEY_FIVE            ,       // Key: 5
        KEY_SIX             ,       // Key: 6
        KEY_SEVEN           ,       // Key: 7
        KEY_EIGHT           ,       // Key: 8
        KEY_NINE            ,       // Key: 9
        KEY_SEMICOLON       ,       // Key: ;
        KEY_EQUAL           ,       // Key: =
        KEY_A               ,       // Key: A | a
        KEY_B               ,       // Key: B | b
        KEY_C               ,       // Key: C | c
        KEY_D               ,       // Key: D | d
        KEY_E               ,       // Key: E | e
        KEY_F               ,       // Key: F | f
        KEY_G               ,       // Key: G | g
        KEY_H               ,       // Key: H | h
        KEY_I               ,       // Key: I | i
        KEY_J               ,       // Key: J | j
        KEY_K               ,       // Key: K | k
        KEY_L               ,       // Key: L | l
        KEY_M               ,       // Key: M | m
        KEY_N               ,       // Key: N | n
        KEY_O               ,       // Key: O | o
        KEY_P               ,       // Key: P | p
        KEY_Q               ,       // Key: Q | q
        KEY_R               ,       // Key: R | r
        KEY_S               ,       // Key: S | s
        KEY_T               ,       // Key: T | t
        KEY_U               ,       // Key: U | u
        KEY_V               ,       // Key: V | v
        KEY_W               ,       // Key: W | w
        KEY_X               ,       // Key: X | x
        KEY_Y               ,       // Key: Y | y
        KEY_Z               ,       // Key: Z | z
        KEY_LEFT_BRACKET    ,       // Key: [
        KEY_BACKSLASH       ,       // Key: '\'
        KEY_RIGHT_BRACKET   ,       // Key: ]
        KEY_GRAVE           ,       // Key: `
        // Function keys
        KEY_SPACE           ,       // Key: Space
        KEY_ESCAPE          ,      // Key: Esc
        KEY_ENTER           ,      // Key: Enter
        KEY_TAB             ,      // Key: Tab
        KEY_BACKSPACE       ,      // Key: Backspace
        KEY_INSERT          ,      // Key: Ins
        KEY_DELETE          ,      // Key: Del
        KEY_RIGHT           ,      // Key: Cursor right
        KEY_LEFT            ,      // Key: Cursor left
        KEY_DOWN            ,      // Key: Cursor down
        KEY_UP              ,      // Key: Cursor up
        KEY_PAGE_UP         ,      // Key: Page up
        KEY_PAGE_DOWN       ,      // Key: Page down
        KEY_HOME            ,      // Key: Home
        KEY_END             ,      // Key: End
        KEY_CAPS_LOCK       ,      // Key: Caps lock
        KEY_SCROLL_LOCK     ,      // Key: Scroll down
        KEY_NUM_LOCK        ,      // Key: Num lock
        KEY_PRINT_SCREEN    ,      // Key: Print screen
        KEY_PAUSE           ,      // Key: Pause
        KEY_F1              ,      // Key: F1
        KEY_F2              ,      // Key: F2
        KEY_F3              ,      // Key: F3
        KEY_F4              ,      // Key: F4
        KEY_F5              ,      // Key: F5
        KEY_F6              ,      // Key: F6
        KEY_F7              ,      // Key: F7
        KEY_F8              ,      // Key: F8
        KEY_F9              ,      // Key: F9
        KEY_F10             ,      // Key: F10
        KEY_F11             ,      // Key: F11
        KEY_F12             ,      // Key: F12
        KEY_LEFT_SHIFT      ,      // Key: Shift left
        KEY_LEFT_CONTROL    ,      // Key: Control left
        KEY_LEFT_ALT        ,      // Key: Alt left
        KEY_LEFT_SUPER      ,      // Key: Super left
        KEY_RIGHT_SHIFT     ,      // Key: Shift right
        KEY_RIGHT_CONTROL   ,      // Key: Control right
        KEY_RIGHT_ALT       ,      // Key: Alt right
        KEY_RIGHT_SUPER     ,      // Key: Super right
        KEY_KB_MENU         ,      // Key: KB menu
        // Keypad keys
        KEY_KP_0            ,      // Key: Keypad 0
        KEY_KP_1            ,      // Key: Keypad 1
        KEY_KP_2            ,      // Key: Keypad 2
        KEY_KP_3            ,      // Key: Keypad 3
        KEY_KP_4            ,      // Key: Keypad 4
        KEY_KP_5            ,      // Key: Keypad 5
        KEY_KP_6            ,      // Key: Keypad 6
        KEY_KP_7            ,      // Key: Keypad 7
        KEY_KP_8            ,      // Key: Keypad 8
        KEY_KP_9            ,      // Key: Keypad 9
        KEY_KP_DECIMAL      ,      // Key: Keypad .
        KEY_KP_DIVIDE       ,      // Key: Keypad /
        KEY_KP_MULTIPLY     ,      // Key: Keypad *
        KEY_KP_SUBTRACT     ,      // Key: Keypad -
        KEY_KP_ADD          ,      // Key: Keypad +
        KEY_KP_ENTER        ,      // Key: Keypad Enter
        KEY_KP_EQUAL        ,      // Key: Keypad =
        // Android key buttons
        KEY_BACK            ,        // Key: Android back button
        KEY_MENU            ,       // Key: Android menu button
        KEY_VOLUME_UP       ,       // Key: Android volume up button
        KEY_VOLUME_DOWN             // Key: Android volume down button
};


string keyboard_key_str[] ={
        "KEY_NULL"            ,        // Key: NULL, used for no key pressed
        // Alphanumeric keys
        "KEY_APOSTROPHE"      ,       // Key: '
        "KEY_COMMA"           ,       // Key: ,
        "KEY_MINUS"           ,       // Key: -
        "KEY_PERIOD"          ,       // Key: .
        "KEY_SLASH"           ,       // Key: /
        "KEY_ZERO"            ,       // Key: 0
        "KEY_ONE"             ,       // Key: 1
        "KEY_TWO"             ,       // Key: 2
        "KEY_THREE"           ,       // Key: 3
        "KEY_FOUR"            ,       // Key: 4
        "KEY_FIVE"            ,       // Key: 5
        "KEY_SIX"             ,       // Key: 6
        "KEY_SEVEN"           ,       // Key: 7
        "KEY_EIGHT"           ,       // Key: 8
        "KEY_NINE"            ,       // Key: 9
        "KEY_SEMICOLON"       ,       // Key: ;
        "KEY_EQUAL"           ,       // Key: =
        "KEY_A"               ,       // Key: A | a
        "KEY_B"               ,       // Key: B | b
        "KEY_C"               ,       // Key: C | c
        "KEY_D"               ,       // Key: D | d
        "KEY_E"               ,       // Key: E | e
        "KEY_F"               ,       // Key: F | f
        "KEY_G"               ,       // Key: G | g
        "KEY_H"               ,       // Key: H | h
        "KEY_I"               ,       // Key: I | i
        "KEY_J"               ,       // Key: J | j
        "KEY_K"               ,       // Key: K | k
        "KEY_L"               ,       // Key: L | l
        "KEY_M"               ,       // Key: M | m
        "KEY_N"               ,       // Key: N | n
        "KEY_O"               ,       // Key: O | o
        "KEY_P"               ,       // Key: P | p
        "KEY_Q"               ,       // Key: Q | q
        "KEY_R"               ,       // Key: R | r
        "KEY_S"               ,       // Key: S | s
        "KEY_T"               ,       // Key: T | t
        "KEY_U"               ,       // Key: U | u
        "KEY_V"               ,       // Key: V | v
        "KEY_W"               ,       // Key: W | w
        "KEY_X"               ,       // Key: X | x
        "KEY_Y"               ,       // Key: Y | y
        "KEY_Z"               ,       // Key: Z | z
        "KEY_LEFT_BRACKET"    ,       // Key: [
        "KEY_BACKSLASH"       ,       // Key: '\'
        "KEY_RIGHT_BRACKET"   ,       // Key: ]
        "KEY_GRAVE"           ,       // Key: `
        // Function keys
        "KEY_SPACE"           ,       // Key: Space
        "KEY_ESCAPE"          ,      // Key: Esc
        "KEY_ENTER"           ,      // Key: Enter
        "KEY_TAB"             ,      // Key: Tab
        "KEY_BACKSPACE"       ,      // Key: Backspace
        "KEY_INSERT"          ,      // Key: Ins
        "KEY_DELETE"          ,      // Key: Del
        "KEY_RIGHT"           ,      // Key: Cursor right
        "KEY_LEFT"            ,      // Key: Cursor left
        "KEY_DOWN"            ,      // Key: Cursor down
        "KEY_UP"              ,      // Key: Cursor up
        "KEY_PAGE_UP"         ,      // Key: Page up
        "KEY_PAGE_DOWN"       ,      // Key: Page down
        "KEY_HOME"            ,      // Key: Home
        "KEY_END"             ,      // Key: End
        "KEY_CAPS_LOCK"       ,      // Key: Caps lock
        "KEY_SCROLL_LOCK"     ,      // Key: Scroll down
        "KEY_NUM_LOCK"        ,      // Key: Num lock
        "KEY_PRINT_SCREEN"    ,      // Key: Print screen
        "KEY_PAUSE"           ,      // Key: Pause
        "KEY_F1"              ,      // Key: F1
        "KEY_F2"              ,      // Key: F2
        "KEY_F3"              ,      // Key: F3
        "KEY_F4"              ,      // Key: F4
        "KEY_F5"              ,      // Key: F5
        "KEY_F6"              ,      // Key: F6
        "KEY_F7"              ,      // Key: F7
        "KEY_F8"              ,      // Key: F8
        "KEY_F9"              ,      // Key: F9
        "KEY_F10"             ,      // Key: F10
        "KEY_F11"             ,      // Key: F11
        "KEY_F12"             ,      // Key: F12
        "KEY_LEFT_SHIFT"      ,      // Key: Shift left
        "KEY_LEFT_CONTROL"    ,      // Key: Control left
        "KEY_LEFT_ALT"        ,      // Key: Alt left
        "KEY_LEFT_SUPER"      ,      // Key: Super left
        "KEY_RIGHT_SHIFT"     ,      // Key: Shift right
        "KEY_RIGHT_CONTROL"   ,      // Key: Control right
        "KEY_RIGHT_ALT"       ,      // Key: Alt right
        "KEY_RIGHT_SUPER"     ,      // Key: Super right
        "KEY_KB_MENU"         ,      // Key: KB menu
        // Keypad keys
        "KEY_KP_0"            ,      // Key: Keypad 0
        "KEY_KP_1"            ,      // Key: Keypad 1
        "KEY_KP_2"            ,      // Key: Keypad 2
        "KEY_KP_3"            ,      // Key: Keypad 3
        "KEY_KP_4"            ,      // Key: Keypad 4
        "KEY_KP_5"            ,      // Key: Keypad 5
        "KEY_KP_6"            ,      // Key: Keypad 6
        "KEY_KP_7"            ,      // Key: Keypad 7
        "KEY_KP_8"            ,      // Key: Keypad 8
        "KEY_KP_9"            ,      // Key: Keypad 9
        "KEY_KP_DECIMAL"      ,      // Key: Keypad .
        "KEY_KP_DIVIDE"       ,      // Key: Keypad /
        "KEY_KP_MULTIPLY"     ,      // Key: Keypad *
        "KEY_KP_SUBTRACT"     ,      // Key: Keypad -
        "KEY_KP_ADD"          ,      // Key: Keypad +
        "KEY_KP_ENTER"        ,      // Key: Keypad Enter
        "KEY_KP_EQUAL"        ,      // Key: Keypad =
        // Android key buttons
        "KEY_BACK"            ,        // Key: Android back button
        "KEY_MENU"            ,       // Key: Android menu button
        "KEY_VOLUME_UP"       ,       // Key: Android volume up button
        "KEY_VOLUME_DOWN"             // Key: Android volume down button
};


int gamepad_button_int[] ={
        GAMEPAD_BUTTON_UNKNOWN,         // Unknown button, just for error checking
        GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
        GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Square, Xbox: X)
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
        GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
        GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
        GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
        GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
        GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
};


string gamepad_button_str[] ={
        "GAMEPAD_BUTTON_UNKNOWN",         // Unknown button, just for error checking
        "GAMEPAD_BUTTON_LEFT_FACE_UP",        // Gamepad left DPAD up button
        "GAMEPAD_BUTTON_LEFT_FACE_RIGHT",     // Gamepad left DPAD right button
        "GAMEPAD_BUTTON_LEFT_FACE_DOWN",      // Gamepad left DPAD down button
        "GAMEPAD_BUTTON_LEFT_FACE_LEFT",      // Gamepad left DPAD left button
        "GAMEPAD_BUTTON_RIGHT_FACE_UP",       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
        "GAMEPAD_BUTTON_RIGHT_FACE_RIGHT",    // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
        "GAMEPAD_BUTTON_RIGHT_FACE_DOWN",     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
        "GAMEPAD_BUTTON_RIGHT_FACE_LEFT",     // Gamepad right button left (i.e. PS3: Square, Xbox: X)
        "GAMEPAD_BUTTON_LEFT_TRIGGER_1",      // Gamepad top/back trigger left (first), it could be a trailing button
        "GAMEPAD_BUTTON_LEFT_TRIGGER_2",      // Gamepad top/back trigger left (second), it could be a trailing button
        "GAMEPAD_BUTTON_RIGHT_TRIGGER_1",     // Gamepad top/back trigger right (one), it could be a trailing button
        "GAMEPAD_BUTTON_RIGHT_TRIGGER_2",     // Gamepad top/back trigger right (second), it could be a trailing button
        "GAMEPAD_BUTTON_MIDDLE_LEFT",         // Gamepad center buttons, left one (i.e. PS3: Select)
        "GAMEPAD_BUTTON_MIDDLE",              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
        "GAMEPAD_BUTTON_MIDDLE_RIGHT",        // Gamepad center buttons, right one (i.e. PS3: Start)
        "GAMEPAD_BUTTON_LEFT_THUMB",          // Gamepad joystick pressed button left
        "GAMEPAD_BUTTON_RIGHT_THUMB"          // Gamepad joystick pressed button right
};


int mouse_button_int[] ={
        MOUSE_BUTTON_LEFT,       // Mouse button left
        MOUSE_BUTTON_RIGHT,       // Mouse button right
        MOUSE_BUTTON_MIDDLE,       // Mouse button middle (pressed wheel)
        MOUSE_BUTTON_SIDE,       // Mouse button side (advanced mouse device)
        MOUSE_BUTTON_EXTRA,       // Mouse button extra (advanced mouse device)
        MOUSE_BUTTON_FORWARD,       // Mouse button forward (advanced mouse device)
        MOUSE_BUTTON_BACK,
};


string mouse_button_str[] ={
        "MOUSE_BUTTON_LEFT",       // Mouse button left
        "MOUSE_BUTTON_RIGHT",       // Mouse button right
        "MOUSE_BUTTON_MIDDLE",       // Mouse button middle (pressed wheel)
        "MOUSE_BUTTON_SIDE",       // Mouse button side (advanced mouse device)
        "MOUSE_BUTTON_EXTRA",       // Mouse button extra (advanced mouse device)
        "MOUSE_BUTTON_FORWARD",       // Mouse button forward (advanced mouse device)
        "MOUSE_BUTTON_BACK",
};

enum {
    PRESSED,
    RELEASED,
    DOWN,
    UP
};

bool process_button_input(string buttons[MAX_BUTTON_BINDINGS], int action){ //@ rename "action" to "state"?
    bool passed = false;
    for (int binding = 0; binding < MAX_BUTTON_BINDINGS; ++binding) {
        if (buttons[binding][0] == 'K') { //KEY_ ...
            for (int index = 0; index < sizeof(keyboard_key_str) / sizeof(*keyboard_key_str); ++index) {
                if (keyboard_key_str[index] == buttons[binding]){
                    if      (action == PRESSED && IsKeyPressed(keyboard_key_int[index]))
                        passed = true;
                    else if (action == RELEASED && IsKeyReleased(keyboard_key_int[index]))
                        passed = true;
                    else if (action == DOWN && IsKeyDown(keyboard_key_int[index]))
                        passed = true;
                    else if (action == UP && IsKeyUp(keyboard_key_int[index]))
                        passed = true;
                }
            }
        }
        if (buttons[binding][0] == 'M') { // MOUSE_BUTTON_ ...
            for (int index = 0; index < sizeof(mouse_button_str) / sizeof(*mouse_button_str); ++index) {
                if (mouse_button_str[index] == buttons[binding]){
                    if      (action == PRESSED && IsMouseButtonPressed(mouse_button_int[index]))
                        passed = true;
                    else if (action == RELEASED && IsMouseButtonReleased(mouse_button_int[index]))
                        passed = true;
                    else if (action == DOWN && IsMouseButtonDown(mouse_button_int[index]))
                        passed = true;
                    else if (action == UP && IsMouseButtonUp(mouse_button_int[index]))
                        passed = true;
                }
            }
        }
        if (buttons[binding][0] == 'G') { // GAMEPAD_BUTTON_ ...
            for (int index = 0; index < sizeof(gamepad_button_str) / sizeof(*gamepad_button_str); ++index) {
                if (gamepad_button_str[index] == buttons[binding]){
                    if      (action == PRESSED && IsGamepadButtonPressed(active_gamepad, gamepad_button_int[index]))
                        passed = true;
                    else if (action == RELEASED && IsGamepadButtonReleased(active_gamepad, gamepad_button_int[index]))
                        passed = true;
                    else if (action == DOWN && IsGamepadButtonDown(active_gamepad, gamepad_button_int[index]))
                        passed = true;
                    else if (action == UP && IsGamepadButtonUp(active_gamepad, gamepad_button_int[index]))
                        passed = true;
                }
            }
        }
    }
    return passed;
}

void input_mapping_check(string buttons[MAX_BUTTON_BINDINGS]){
    for (int binding = 0; binding < MAX_BUTTON_BINDINGS; ++binding) {
        bool passed = false;
        if (buttons[binding] == "") {passed = true; break;}
        else if (buttons[binding][0] == 'K') { //KEY_ ...
            for (int index = 0; index < sizeof(keyboard_key_str) / sizeof(*keyboard_key_str); ++index) {
                if (keyboard_key_str[index] == buttons[binding]){
                    passed = true;
                    break;
                }
            }
        }
        else if (buttons[binding][0] == 'M') { // MOUSE_BUTTON_ ...
            for (int index = 0; index < sizeof(mouse_button_str) / sizeof(*mouse_button_str); ++index) {
                if (mouse_button_str[index] == buttons[binding]){
                    passed = true;
                    break;
                }
            }
        }
        else if (buttons[binding][0] == 'G') { // GAMEPAD_BUTTON_ ...
            for (int index = 0; index < sizeof(gamepad_button_str) / sizeof(*gamepad_button_str); ++index) {
                if (gamepad_button_str[index] == buttons[binding]){
                    passed = true;
                    break;
                }
            }
        }
        if (!passed){
            printf("ERROR: \"%s\" button not found \n", buttons[binding].c_str());
            total_errors++;
        }
    }
}

bool IsButtonPressed(string buttons[MAX_BUTTON_BINDINGS]){
    return process_button_input(buttons, PRESSED);
}
bool IsButtonReleased(string buttons[MAX_BUTTON_BINDINGS]){
    return process_button_input(buttons, RELEASED);
}
bool IsButtonDown(string buttons[MAX_BUTTON_BINDINGS]){
    return process_button_input(buttons, DOWN);
}
bool IsButtonUp(string buttons[MAX_BUTTON_BINDINGS]){
    return process_button_input(buttons, UP);
}

// Switch active used gamepad from 1 up to 4
void switch_active_gamepad(){  //@Hardcoded - should be options menu switch
    if (IsKeyPressed(KEY_ONE) && (IsKeyDown(KEY_G))){
        active_gamepad = 0;
    }
    else if (IsKeyPressed(KEY_TWO) && (IsKeyDown(KEY_G))){
        active_gamepad = 1;
    }
    else if (IsKeyPressed(KEY_THREE) && (IsKeyDown(KEY_G))){
        active_gamepad = 2;
    }
    else if (IsKeyPressed(KEY_FOUR) && (IsKeyDown(KEY_G))){
        active_gamepad = 3;
    }
}

// All user input
void process_input(Player &player, Camera2D &camera, Game_Level current_level) { //@Just put func contents in main?
    if (IsButtonPressed(BUTTON_PAUSE)) {
        _is_menu = !_is_menu;
        _is_paused = !_is_paused;
    }

    switch_active_gamepad();

    ingame_mouse_delta = GetMouseDelta();
    if(letterboxing){
        ingame_mouse_delta.x = ingame_mouse_delta.x / screen_scale_min / camera.zoom;
        ingame_mouse_delta.y = ingame_mouse_delta.y / screen_scale_min / camera.zoom;
    } else{
        ingame_mouse_delta.x = ingame_mouse_delta.x / screen_scale.x / camera.zoom;
        ingame_mouse_delta.y = ingame_mouse_delta.y / screen_scale.y / camera.zoom;
    }

    if (IsButtonPressed(BUTTON_DEBUG_INFO)) _draw_debug_info = !_draw_debug_info;
    if (IsButtonPressed(BUTTON_FRAMELOCK)) toggle_framelock();
    if (IsButtonPressed(BUTTON_VSYNC)) toggle_vsync();
    if (
        ( (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))  && IsKeyPressed(KEY_ENTER) )
        || ( (IsButtonPressed(BUTTON_TOGGLE_BORDERLESS)) )
        )
        toggle_borderless();
    if(IsKeyPressed(KEY_F10)) { //maybe F1?
        letterboxing = !letterboxing;
        if(letterboxing) printf("---Letterboxing is ON---\n");
        else printf("---Letterboxing is OFF---\n");
    }
    if(IsKeyPressed(KEY_F9)) { // maybe F2?
        _screen_filtering = !_screen_filtering;
        if(_screen_filtering) printf("---Screen Filtering is ON---\n");
        else printf("---Screen Filtering is OFF---\n");
    }

    toggle_fullscreen(); //@Unfinished (check raylib's fullscreen, in v5.0 they fixed something)

    if (!_is_menu) {

        { /// JUMP
            if (IsButtonPressed(BUTTON_JUMP)) {
                player.jump_input_buffer_time = JUMP_INPUT_BUFFER_TIME;
            }
            if (player.jump_input_buffer_time > 0) {
                player.jump_input_buffer_time -= delta_time;
                // JUMP!
                if ((player.is_standing || player.air_time < COYOTE_TIME) && !player.is_jumping) {
                    player.speed.y = -JUMP_POWER;
                    player.jump_input_buffer_time = 0;
                    player.time_of_fall = INITIAL_TIME_OF_FALL;
                    player.is_standing = false;
                    player.is_holding_jump = true;
                    player.is_jumping = true;

                    player.started_jumping = true;
                }
            }
            player.acceleration_down = GRAVITATION;
            if (IsButtonDown(BUTTON_JUMP) && player.is_holding_jump) {
                player.jump_input_hold_time += delta_time;
            } else if (player.is_jumping)
            { // When jump button is released
                if (player.jump_input_hold_time < MIN_JUMP_INPUT_HOLD_TIME)
                    player.jump_input_hold_time = MIN_JUMP_INPUT_HOLD_TIME;
                player.acceleration_down = GRAVITATION * JUMP_SNAPPING_FACTOR * (MAX_JUMP_INPUT_HOLD_TIME / player.jump_input_hold_time);
                player.is_holding_jump = false;
            }
        }

        float previous_speed_x = player.speed.x;

        /// RUN
        if (IsButtonDown(BUTTON_MOVE_LEFT)) { player.speed.x = -WALK_SPEED; }
        else if (IsButtonDown(BUTTON_MOVE_RIGHT)) { player.speed.x = WALK_SPEED; }
        else { player.speed.x = 0; }

        /// @ change to ->  if walk keys pressed - player+= walk speed, and just cap the speed afted applying inputs: if speed.x is > abs(walk speed) - speed = walk speed * flip    (because keyboard and gamepad movement adds up)
        /// по аналогии с MAX_FALL_SPEED


        player.speed.x += WALK_SPEED* GetGamepadAxisMovement(active_gamepad, 0); // slow down the running animation??

        if ( (player.speed.x > 0 && player.facing_left) || (player.speed.x < 0 && !player.facing_left) ) {
            player.facing_left = !player.facing_left;
        }


        if (IsButtonDown(BUTTON_RMB) /*&& free_cam*/) { //FREE CAMERA DRAG
            free_cam = true;
            camera.target.x -= ingame_mouse_delta.x;
            camera.target.y -= ingame_mouse_delta.y;
            SetMouseCursor(9);
        } else { SetMouseCursor(0); }

        if (IsButtonPressed(BUTTON_LMB)) { //TELEPORT
            player.x = ingame_mouse.x - player.width / 2;
            player.y = ingame_mouse.y - player.height / 2;

            player.time_of_fall = INITIAL_TIME_OF_FALL;
            //player.air_time = 0;
            player.speed.x = 0;
            player.speed.y = 0;
        }
        if (IsButtonPressed(BUTTON_FREECAM)) { //FREE CAMERA TOGGLE
            if (free_cam) camera.zoom = DEFAULT_ZOOM;
            free_cam = !free_cam;
        }
    } // endif !_is_menu


}

