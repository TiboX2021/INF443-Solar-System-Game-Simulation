#pragma once

// Include this one first, else errors
#include "navion/navion.hpp"

#include "cgp/graphics/input_devices/input_devices.hpp"
#include "utils/camera/custom_camera_controller.hpp"
#include "utils/controls/control_constants.hpp"
#include "utils/controls/player_object.hpp"
#include <iostream>
#include <map>

struct KeyEvent
{
    int action;
    int key;
};

// Keyboard control

class Controls
{
public:
    Controls()
    {
        // Initialise key states
        key_states[KEY_Z] = KEY_RELEASED;
        key_states[KEY_Q] = KEY_RELEASED;
        key_states[KEY_S] = KEY_RELEASED;
        key_states[KEY_W] = KEY_RELEASED;
        key_states[KEY_SPACE] = KEY_RELEASED;
        key_states[KEY_ARROW_UP] = KEY_RELEASED;
        key_states[KEY_ARROW_DOWN] = KEY_RELEASED;
        key_states[KEY_ARROW_LEFT] = KEY_RELEASED;
        key_states[KEY_ARROW_RIGHT] = KEY_RELEASED;
    }

    // Handle keypress event
    void handleKeyEvent(cgp::input_devices *inputs)
    {
        // Update key states
        key_states[inputs->keyboard.last_action.key] = inputs->keyboard.last_action.action;
    }

    // Update the camera position from the player control object
    void updateCamera(custom_camera_controller &camera);

    // Handle player actions based on current pressed keys
    void handlePlayerKeys();

    // Update the player object (simulate one step)
    void updatePlayer();
    void updateShip(); // Update the spaceship position according to the player object

    // For display and initialization
    Navion &getPlayerShip();

private:
    std::map<int, int> key_states;
    PlayerObject player;
    Navion navion; // Spaceship controlled by the player
};