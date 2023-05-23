#pragma once

#include <iostream>
#include <thread>

// TODO : camera model et camera controller !
// Actuellement :
// camera_controller_orbit_euler camera_control;
// camera_projection_perspective camera_projection;

// Il faudrait remplacer ça par:
// camera_first_person_controller_euler
// Remarque: camera_projection_perspective reste le même, ça change pas
// Le model de la caméra est stocké dans l'objet, donc c'est la seule chose à changer

/**
Etude de la différence entre camera_controller_orbit_euler et camera_first_person_controller_euler
La variable est "camera_control"

*/

// Handle keyboard input in a separate thread
class KeyboardInput
{
public:
    void start(); // Start the keyboard input handler

    void stop(); // Stop the keyboard input handler

private:
    std::thread input_handler;
    bool isRunning;
    char lastInput;

    // Private thread handler function
    void handle_input(); // Handle keyboard input
};
