#include "object.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"

double Object::time_scale = 1.0; // Default time scale

Object::Object(double mass, cgp::vec3 position, cgp::vec3 rotation_axis, bool should_translate, bool should_rotate)
{
    // Translations
    this->mass = mass;
    this->physics_position = position;
    this->velocity = {0, 0, 0};
    this->acceleration = {0, 0, 0};
    this->forces = {0, 0, 0};

    // Rotations
    this->rotation_angle = 0;
    this->rotation_speed = 1.0; // TODO : adapt this value

    // TODO : set rotation axis

    // Movement
    this->should_translate = should_translate;
    this->should_rotate = should_rotate;
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
    cgp::vec3 distance = other->physics_position - this->physics_position;
    forces += GRAVITAIONAL_CONSTANT * this->mass * other->mass / cgp::dot(distance, distance) * cgp::normalize(distance);
}

/** Update position */
void Object::update(double dt)
{
    if (should_translate)
    {
        this->acceleration = this->forces / this->mass;
        this->velocity += this->acceleration * dt;
        this->physics_position += this->velocity * dt;
    }

    // TODO : rotation. Il faut mettre à jour les modèles
    if (should_rotate)
    {
        this->rotation_angle += this->rotation_speed * dt;
    }
}

// Getters
cgp::vec3 Object::getPhysicsPosition() const
{
    return this->physics_position;
}

double Object::getPhysicsRotationAngle() const
{
    return this->rotation_angle;
}

void Object::setShouldTranslate(bool should_translate)
{
    this->should_translate = should_translate;
}

void Object::setShouldRotate(bool should_rotate)
{
    this->should_rotate = should_rotate;
}

void Object::setPhysicsPosition(cgp::vec3 position)
{
    this->physics_position = position;
}