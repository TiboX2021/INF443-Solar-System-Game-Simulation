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
        // Get camera facing direction
        auto orientation = camera.camera_model.orientation();

        // Move vector
        cgp::vec3 move_vector = orientation * cgp::vec3(0, 0, -1);

        // Comment mettre à jour la caméra ?
        camera.camera_model.manipulator_translate_in_plane({0.1, 0}); // Ca fait quoi?
        // Ca translate dans le plan, c'est tout.

        // std::cout << "Moving the camera !" << std::endl;
    }
}