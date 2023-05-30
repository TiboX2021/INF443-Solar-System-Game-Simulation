#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "navion/navion.hpp"
#include "utils/camera/custom_camera_model.hpp"

// Max player speed
constexpr float PLAYER_MAX_TRANSLATION_SPEED = 0.03 * 10e10 * 60 / (3600 * 24);

// TODO : divide this in order to have an animation
constexpr float PLAYER_MAX_TRANSLATION_ACCELERATION = PLAYER_MAX_TRANSLATION_SPEED * 60 / (3600 * 24) / 60; // Player acceleration per frame

// Max player rotation speed along any axis
// TODO : animate ? When going left / right, also tilt the ship without tilting the camera ?
constexpr float PLAYER_MAX_ROTATION_SPEED = 0.01 * 60 / (3600 * 24); // radians
constexpr float PLAYER_MAX_ROLL_SPEED = 0.03 * 60 / (3600 * 24);     // radians. This is higher, as rolling does not change the tajectory

// Player orientation default (for the playe spaceship)
const cgp::vec3 PLAYER_BASE_DIRECTION = {1, 0, 0};
const cgp::vec3 PLAYER_BASE_TOP = {0, 0, 1};

class PlayerObject
{
public:
    // Default constructor : the player object is modified using setters.
    PlayerObject() : direction({1, 0, 0}), directionTop({0, 0, 1}){};

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
    void updatePlayerShip(Navion &ship) const;

    // Get player orientation (can be used for the camera)
    cgp::rotation_transform orientation() const;

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