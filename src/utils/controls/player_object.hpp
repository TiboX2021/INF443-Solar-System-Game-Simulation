#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "utils/camera/custom_camera_model.hpp"

// TODO : faire ça pour contrôler la position de la caméra
// TODO : faire en sorte

/*
Organisation du bordel :

=> contrôle du clavier : ajouter manuellement des callbacks ?
=> faire en sorte qu'on puisse ajouter une instance de player_object dans les contrôles, et s'il est présent, on le contrôle

=> mise à jour de la caméra : les contrôles du clavier settent des variables d'accélération pour le player
 * Ensuite, on simule le joueur pour ce time set
 * Enfin : la caméra est placée autour du joueur, avec une orientation particulière ?
 * la caméra regarde toujours vers l'objet, avec une distance fixe à l'objet, mais pas forcément dans la même direction


L'object keyboard control a un player, il le fait en toute situation

// Faire une fonction draw ?


POUR LES LIMITES DE VITESSE

une vitesse basse mais acceptable est 0.1 opengl par frame
Pour convertir depuis physique : fois 10e10

Donc par frame, limite de 10e9 mètres

vitesse correspondante :
une frame c'est 1/60 secondes. Puis multiplicateur : le définir dans le timer
24.0f * 3600 à multiplier : 1 jour par seconde

vitesse : c'est un vecteur : prendre une direction ? Dans toutes les directions, pour éviter que se déplacer dans 1 sens affecte les autres


0.1 opengl per frame

the multiplier will do :

1/60 (frame) * 3600 * 24 * speed in order to compute the physics speed. It must equal 0.1 * the physics scale = 10e10
it will then be scaled down

// TODO : on each frame, add some accel to the ship / add some speed ?
// And right before computing, tone it down to the max speed

ie : it equals 0.1  * 10e10 * 60 / (3600 * 24)
*/

// Max player speed (TODO : add multiplier to go faster)
constexpr float PLAYER_MAX_TRANSLATION_SPEED = 0.1 * 10e10 * 60 / (3600 * 24);

// TODO : divide this in order to have an animation
constexpr float PLAYER_MAX_TRANSLATION_ACCELERATION = PLAYER_MAX_TRANSLATION_SPEED * 60 / (3600 * 24) / 60; // Player acceleration per frame

// Max player rotation speed along any axis
// TODO : animate ? When going left / right, also tilt the ship without tilting the camera ?
constexpr float PLAYER_MAX_ROTATION_SPEED = 0.01 * 60 / (3600 * 24); // radians

// TODO : make a combination for roll, pitch, yaw into up/down, left/right, and ship tilting

class PlayerObject
{
public:
    // Default constructor : the player object is modified using setters.
    PlayerObject() : direction({1, 0, 0}), directionTop({0, 0, 1}){};

    // TODO : player manipulation
    // Player max speed ?
    // Capper l'accélération

    void step(); // Simulate one step for the player

    // Player ship rotation commands (also do animation)
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void rollLeft();
    void rollRight();

    // Player ship translation (with animation)
    void moveForward();
    void brake();

    // Update camera
    void updatePlayerCamera(custom_camera_model &camera_model) const;

private:
    cgp::vec3 position;     // Display position, not the physics one
    cgp::vec3 direction;    // Player direction
    cgp::vec3 directionTop; // Vector that points to the top of the player
    cgp::vec3 velocity;

    // Player rotation
    cgp::rotation_transform rotation;

    // Player speed
    float speed;
};