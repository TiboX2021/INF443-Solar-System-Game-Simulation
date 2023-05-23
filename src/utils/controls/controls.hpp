#pragma once

#include "cgp/graphics/camera/camera_controller/camera_controller_orbit_euler/camera_controller_orbit_euler.hpp"
#include "cgp/graphics/input_devices/input_devices.hpp"

// TODO : gérer les clics et les mouvements de la souris
/**
Philosophie du contrôle du joueur:

  Z
Q   S : pour orienter la caméra
  W

ESPACE : pour avancer vers l'avant

Attention : comment gérer les keys qui sont pressed en continu, sans être relâchées ? Faire une map / un truc itérable qui contient les keys pressed ?

// TODO : Gérer plusieurs keys en même temps
// TODO : Définir les rotations de la caméra
// TODO : pour le déplacement avant, utiliser l'objet du joueur et sa vitesse, scale dans le référentiel OpenGL

// BUG : on va être obligés d'utiliser une caméra fps. Il faudra passer en arg la position de la caméra et son orientation pour que ça soit "cross camera"
*/

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