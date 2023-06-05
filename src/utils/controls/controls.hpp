#pragma once

// Include this one first, else errors
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "navion/navion.hpp"

#include "cgp/graphics/input_devices/input_devices.hpp"
#include "utils/camera/custom_camera_controller.hpp"
#include "utils/controls/control_constants.hpp"
#include "utils/controls/gui_params.hpp"
#include "utils/controls/player_object.hpp"
#include "utils/opengl/shield_ubo.hpp"
#include <iostream>
#include <map>
#include <vector>

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
        key_states[KEY_E] = KEY_RELEASED;
        key_states[KEY_SPACE] = KEY_RELEASED;
        key_states[KEY_ARROW_UP] = KEY_RELEASED;
        key_states[KEY_ARROW_DOWN] = KEY_RELEASED;
        key_states[KEY_ARROW_LEFT] = KEY_RELEASED;
        key_states[KEY_ARROW_RIGHT] = KEY_RELEASED;
    }

    // Setters
    void setCameraClipObjects(std::vector<Object *> objects)
    {
        camera_clip_objects = objects;
    }

    // Handle keypress event
    void handleKeyEvent(cgp::input_devices *inputs)
    {
        // Update key states
        key_states[inputs->keyboard.last_action.key] = inputs->keyboard.last_action.action;

        // Instantly update GUI keys
        if (inputs->keyboard.last_action.action == KEY_PRESSED)
        {
            switch (inputs->keyboard.last_action.key)
            {
            case KEY_A:
                global_gui_params.display_ship = !global_gui_params.display_ship;
                break;
            case KEY_Z:
                global_gui_params.enable_shield = !global_gui_params.enable_shield;
                break;
            case KEY_E:
                global_gui_params.trigger_laser = !global_gui_params.trigger_laser;
                break;
            default:
                break;
            }
        }
    }

    // Update the camera position from the player control object
    void updateCamera(custom_camera_controller &camera, cgp::mat4 &camera_matrix_view);

    // Handle player actions based on current pressed keys
    void handlePlayerKeys();

    // Update the player object (simulate one step)
    void updatePlayer();
    void updateShip(); // Update the spaceship position according to the player object

    // For display and initialization
    Navion &getPlayerShip();
    void initialize_shield_mesh();

    // For drawing
    void draw_shield(environment_structure const &environment);

private:
    std::map<int, int> key_states;
    PlayerObject player;
    Navion navion;                             // Spaceship controlled by the player
    std::vector<Object *> camera_clip_objects; // Objects that will clip the camera

    // Shield mesh drawable
    ShieldUBO shield_ubo; // Shield UBO for opengl custom animation drawing
    cgp::mesh_drawable shield_mesh_drawable;
};