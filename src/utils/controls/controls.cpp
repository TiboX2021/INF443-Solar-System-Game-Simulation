#include "controls.hpp"
#include "utils/controls/control_constants.hpp"
#include <iostream>

bool isHeldOrPressed(int key)
{
    return key == KEY_PRESSED || key == KEY_REPEAT;
}

void Controls::updateCamera(cgp::camera_controller_orbit_euler &camera)
{

    //    std::cout << "Last action : " << last_key_event.action << std::endl;

    // Exemple : déplacer la caméra vers le côté
    if (isHeldOrPressed(last_key_event.action) && last_key_event.key == KEY_Z)
    {
        // TODO faire une rotation vers le haut

        // TODO : il faudra garder ça en tête dans le player object
        // C'est de la vitesse de rotation, ok
        camera.camera_model.manipulator_rotate_roll_pitch_yaw(0, 0.01, 0);
    }

    if (isHeldOrPressed(last_key_event.action) && last_key_event.key == KEY_SPACE)
    {
        camera.camera_model.manipulator_translate_front(-0.1);
    }
}