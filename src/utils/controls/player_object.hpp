#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "navion/navion.hpp"
#include "utils/camera/custom_camera_model.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include "utils/threads/threads.hpp"
#include "utils/tools/tools.hpp"

// Max player speed
constexpr float PLAYER_MAX_TRANSLATION_SPEED = 0.03 * 10e10 * 60 / (3600 * 24);

// Takes 3 second for full translation speed. (The last coefficient is the number of frames at 60 fps)
constexpr float PLAYER_TRANSLATION_ACCELERATION = PLAYER_MAX_TRANSLATION_SPEED * 60 / (3600 * 24) / 180; // Player acceleration per frame

// Max player rotation speed along any axis
constexpr float PLAYER_MAX_ROTATION_SPEED = 0.015 * 60 / (3600 * 24); // radians
constexpr float PLAYER_MAX_ROLL_SPEED = 0.03 * 60 / (3600 * 24);      // radians. This is higher, as rolling does not change the tajectory

// Roll acceleration (takes 0.12 seconds for full roll speed)
constexpr float PLAYER_ROLL_ACCELERATION = PLAYER_MAX_ROLL_SPEED * 60 / (3600 * 24) / 7; // Player acceleration per frame

// Rotation acceleration (takes 0.25 seconds for full rotation speed)
constexpr float PLAYER_ROTATION_ACCELERATION = PLAYER_MAX_ROTATION_SPEED * 60 / (3600 * 24) / 15;

// Player orientation default (for the playe spaceship)
const cgp::vec3 PLAYER_BASE_DIRECTION = {1, 0, 0};
const cgp::vec3 PLAYER_BASE_TOP = {0, 0, 1};

// Both parameters have an influence (sweet spot for 60 fps)
constexpr int DELAY_FRAMES = 12;   // Delay frames for the camera. 1 second
constexpr float DELAY_RATIO = 0.7; // Ratio of the delayed buffer direction

constexpr float COLLISION_TIMEOUT = 3; // Time (in seconds) before an asteroid can collide again with the player

constexpr float SHIELD_COLLISION_ANIMATION_TIME = 0.3f; // Animation time for the shield, in seconds

constexpr float MAX_DESTRUCTION_DISTANCE = SATURN_SUN_DISTANCE; // Max distance from the player to an asteroid to be destroyed

constexpr float LASER_DESTRUCTION_RADIUS = 0.5 / PHYSICS_SCALE; // Half player radius, as it is
constexpr float LASER_VISIBLE_RADIUS = 0.1;

struct PlayerCollisionData
{
    cgp::vec3 position;
    cgp::vec3 velocity;
    cgp::vec3 direction;
    float radius;
};

// Global player position
extern ReadWriteLock<PlayerCollisionData> global_player_collision_data;

// Global player collision animation buffer
extern AsteroidCollisionAnimationBuffer global_player_collision_animation_buffer;

// Struct to handle gradual otation and translation speeds
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
    PlayerObject() : position({-2e12, 0, 4e11}), // Default position
                     direction({1, 0, 0}),
                     directionTop({0, 0, 1}),
                     speed({0, PLAYER_MAX_TRANSLATION_SPEED, 0, PLAYER_TRANSLATION_ACCELERATION}),
                     roll_speed({0, PLAYER_MAX_ROLL_SPEED, -PLAYER_MAX_ROLL_SPEED, PLAYER_ROLL_ACCELERATION}),
                     vertical_rotation_speed({0, PLAYER_MAX_ROTATION_SPEED, -PLAYER_MAX_ROTATION_SPEED, PLAYER_ROTATION_ACCELERATION}),
                     horizontal_rotation_speed({0, PLAYER_MAX_ROTATION_SPEED, -PLAYER_MAX_ROTATION_SPEED, PLAYER_ROTATION_ACCELERATION}),
                     camera_direction_buffer(DELAY_FRAMES, PLAYER_BASE_DIRECTION),
                     camera_direction_top_buffer(DELAY_FRAMES, PLAYER_BASE_TOP){};

    void step(const std::vector<Object *> &objects_with_hitbox = {}); // Simulate one step for the player

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
    void updatePlayerCamera(custom_camera_model &camera_model, const std::vector<Object *> &camera_clip_objects = {}) const;
    void updatePlayerShip(Navion &ship) const;

    // Get player orientation (can be used for the camera)
    cgp::rotation_transform orientation() const;
    cgp::vec3 get_position() const;
    cgp::vec3 get_direction() const;

private:
    cgp::vec3 position;     // Display position, not the physics one
    cgp::vec3 direction;    // Player direction
    cgp::vec3 directionTop; // Vector that points to the top of the player
    cgp::vec3 velocity;

    // Camera vectors
    cgp::vec3 camera_direction;
    cgp::vec3 camera_direction_top;

    // Player rotation
    cgp::rotation_transform rotation;

    // Player speed coeffs
    GradualCoeff speed;
    GradualCoeff roll_speed;
    GradualCoeff vertical_rotation_speed;
    GradualCoeff horizontal_rotation_speed;

    // Camera position buffer
    ObjectBuffer<cgp::vec3> camera_direction_buffer;     // = ObjectBuffer<cgp::vec3>(2);
    ObjectBuffer<cgp::vec3> camera_direction_top_buffer; // = ObjectBuffer<cgp::vec3>(2);
};