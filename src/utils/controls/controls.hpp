#pragma once

#include "cgp/graphics/camera/camera_controller/camera_controller_orbit_euler/camera_controller_orbit_euler.hpp"
#include "cgp/graphics/input_devices/input_devices.hpp"
#include "control_constants.hpp"

// TODO : gérer les clics et les mouvements de la souris

struct KeyEvent
{
    int action;
    int key;
};

// Keyboard control

class Controls
{
public:
    void handleKeyEvent(cgp::input_devices *inputs)
    {
        KeyEvent current_event = {inputs->keyboard.last_action.action, inputs->keyboard.last_action.key};

        // TODO : treat this

        // TODO : what if multiple keys are pressed ? Keep their states in a map in order to detect changes

        // Replace old
        last_key_event = current_event;
    }

    void updateCamera(cgp::camera_controller_orbit_euler &camera);

private:
    KeyEvent last_key_event = {0, -1};
};