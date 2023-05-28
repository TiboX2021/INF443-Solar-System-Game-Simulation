#include "controls.hpp"
#include "utils/camera/custom_camera_controller.hpp"
#include "utils/controls/control_constants.hpp"
#include <iostream>

bool isHeldOrPressed(int key)
{
    return key == KEY_PRESSED || key == KEY_REPEAT;
}

// Handle player key presses
void Controls::handlePlayerKeys()
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
            case KEY_ARROW_UP:
                // Pitch up
                player.moveUp();
                break;
            case KEY_ARROW_LEFT:
                // Turn left
                player.moveLeft();
                break;
            case KEY_ARROW_RIGHT:
                // Turn right
                player.moveRight();
                break;
            case KEY_ARROW_DOWN:
                // Pitch down
                player.moveDown();
                break;
            case KEY_SPACE:
                // Move forward
                player.moveForward();
                break;
            case KEY_Q:
                player.rollLeft();
                break;
            case KEY_S:
                player.rollRight();
                break;
            default:
                break;
            }
        }
    }

    // If space bar is not pressed, slow down
    if (key_states[KEY_SPACE] == KEY_RELEASED)
    {
        player.brake();
    }
}

void Controls::updateCamera(custom_camera_controller &camera)
{
    // Update camera orientation according to the player
    player.updatePlayerCamera(camera.camera_model);
}

void Controls::updatePlayer()
{
    player.step();
}

Navion &Controls::getPlayerShip()
{
    return navion;
}