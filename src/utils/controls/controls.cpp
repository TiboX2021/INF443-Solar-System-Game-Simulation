#include "controls.hpp"
#include "cgp/graphics/camera/camera_controller/camera_controller_first_person_euler/camera_controller_first_person_euler.hpp"
#include "utils/controls/control_constants.hpp"
#include <iostream>

bool isHeldOrPressed(int key)
{
    return key == KEY_PRESSED || key == KEY_REPEAT;
}

void Controls::updateCamera(cgp::camera_controller_first_person_euler &camera)
{

    // Scan key states
    for (auto &key_state : key_states)
    {
        // If the key is pressed or held
        if (isHeldOrPressed(key_state.second))
        {
            // Handle actions
            switch (key_state.first)
            {
            case KEY_Z:
                // Pitch up
                camera.camera_model.manipulator_rotate_roll_pitch_yaw(0, 0.01, 0);
                break;
            case KEY_Q:
                // Turn left
                camera.camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, 0.01);
                break;
            case kEY_S:
                // Turn right
                camera.camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -0.01);
                break;
            case KEY_W:
                // Pitch down
                camera.camera_model.manipulator_rotate_roll_pitch_yaw(0, -0.01, 0);

                break;
            case KEY_SPACE:
                // Move forward
                camera.camera_model.manipulator_translate_front(-0.1);
                break;
            default:
                break;
            }
        }
    }
}