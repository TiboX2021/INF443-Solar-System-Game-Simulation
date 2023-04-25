#include "object.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"

double Object::time_scale = 1.0; // Default time scale

Object::Object(double mass, cgp::vec3 position, cgp::vec3 rotation_axis)
{
    // Translations
    this->mass = mass;
    this->physics_positions = position;
    this->velocity = {0, 0, 0};
    this->acceleration = {0, 0, 0};
    this->forces = {0, 0, 0};

    // Rotations
    this->rotation_angle = 0;
    this->rotation_speed = 1.0; // TODO : adapt this value

    // TODO : set rotation axis
}

void Object::resetForces()
{
    this->forces = {0, 0, 0};
}

/**
 * Compute gravitational force from another object
 */
void Object::computeGravitationnalForce(Object *other)
{
    cgp::vec3 distance = other->physics_positions - this->physics_positions;
    forces += GRAVITAIONAL_CONSTANT * this->mass * other->mass / cgp::dot(distance, distance) * cgp::normalize(distance);
}

/** Update position */
void Object::update(double dt)
{
    this->acceleration = this->forces / this->mass;
    this->velocity += this->acceleration * dt;
    this->physics_positions += this->velocity * dt;

    // TODO : rotation. Il faut mettre à jour les modèles
    this->rotation_angle += this->rotation_speed * dt;
}

// Getters
cgp::vec3 Object::getPhysicsPosition() const
{
    return this->physics_positions;
}

double Object::getPhysicsRotationAngle() const
{
    return this->rotation_angle;
}