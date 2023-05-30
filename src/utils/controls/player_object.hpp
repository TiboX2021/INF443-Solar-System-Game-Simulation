#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "navion/navion.hpp"
#include "utils/camera/custom_camera_model.hpp"
#include "utils/physics/object.hpp"

// Max player speed
constexpr float PLAYER_MAX_TRANSLATION_SPEED = 0.03 * 10e10 * 60 / (3600 * 24);

// Takes 3 second for full translation speed. (The last coefficient is the number of frames at 60 fps)
constexpr float PLAYER_TRANSLATION_ACCELERATION = PLAYER_MAX_TRANSLATION_SPEED * 60 / (3600 * 24) / 180; // Player acceleration per frame

// Max player rotation speed along any axis
constexpr float PLAYER_MAX_ROTATION_SPEED = 0.01 * 60 / (3600 * 24); // radians
constexpr float PLAYER_MAX_ROLL_SPEED = 0.03 * 60 / (3600 * 24);     // radians. This is higher, as rolling does not change the tajectory

// Roll acceleration (takes 0.25 seconds for full roll speed)
constexpr float PLAYER_ROLL_ACCELERATION = PLAYER_MAX_ROLL_SPEED * 60 / (3600 * 24) / 15; // Player acceleration per frame

// Rotation acceleration (takes 0.5 seconds for full rotation speed)
constexpr float PLAYER_ROTATION_ACCELERATION = PLAYER_MAX_ROTATION_SPEED * 60 / (3600 * 24) / 15;

// Player orientation default (for the playe spaceship)
const cgp::vec3 PLAYER_BASE_DIRECTION = {1, 0, 0};
const cgp::vec3 PLAYER_BASE_TOP = {0, 0, 1};

// Strruct to handle gradual otation and translation speeds
struct GradualCoeff
{
    float value = 0;
    float max_value = 0;
    float min_value = 0;

    float acceleration = 0;

    void one_step_up()
    {
        value += acceleration * Timer::getSimulStep();

        // Cap value
        if (value > max_value)
            value = max_value;
    }
    void one_step_down()
    {
        value -= acceleration * Timer::getSimulStep();

        // Cap value
        if (value < min_value)
            value = min_value;
    }

    void one_step_decelerate()
    {
        if (value > 0)
        {
            value -= acceleration * Timer::getSimulStep();

            // Cap value
            if (value < 0)
                value = 0;
        }
        else if (value < 0)
        {
            value += acceleration * Timer::getSimulStep();

            // Cap value
            if (value > 0)
                value = 0;
        }
    }
};

class PlayerObject
{
public:
    // Default constructor : the player object is modified using setters.
    PlayerObject() : direction({1, 0, 0}),
                     directionTop({0, 0, 1}),
                     speed({0, PLAYER_MAX_TRANSLATION_SPEED, 0, PLAYER_TRANSLATION_ACCELERATION}),
                     roll_speed({0, PLAYER_MAX_ROLL_SPEED, -PLAYER_MAX_ROLL_SPEED, PLAYER_ROLL_ACCELERATION}),
                     vertical_rotation_speed({0, PLAYER_MAX_ROTATION_SPEED, -PLAYER_MAX_ROTATION_SPEED, PLAYER_ROTATION_ACCELERATION}),
                     horizontal_rotation_speed({0, PLAYER_MAX_ROTATION_SPEED, -PLAYER_MAX_ROTATION_SPEED, PLAYER_ROTATION_ACCELERATION}){};

    void step(); // Simulate one step for the player

    // Player ship rotation commands (also do animation)
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void rollLeft();
    void rollRight();

    // Stop rotations when no key is pressed
    void decelerateRoll();
    void decelerateVerticalRotation();
    void decelerateHorizontalRotation();

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

    // Player speed coeffs
    GradualCoeff speed;
    GradualCoeff roll_speed;
    GradualCoeff vertical_rotation_speed;
    GradualCoeff horizontal_rotation_speed;
};