#include "object.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include <cmath>

double Object::time_scale = 3600.0 * 24.0; // Default time scale : 1 simulation second = 1 day

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
    this->rotation_axis = rotation_axis;
    // Vitesse de rotation : en radians par seconde.
    // 1 tour par jour = 2pi radians par jour = 2pi / 3600 / 24 radians par seconde
    this->rotation_speed = 2.0 * M_PI / (3600 * 24); // DEBUG : test pour la terre

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
    forces += GRAVITATIONAL_CONSTANT * this->mass * other->mass / cgp::dot(distance, distance) * cgp::normalize(distance);
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

cgp::rotation_transform Object::getPhysicsRotation() const
{
    // Needed to rotate the texture with the object
    return cgp::rotation_transform::from_vector_transform({0, 0, 1}, rotation_axis) * cgp::rotation_transform::from_axis_angle({0, 0, 1}, rotation_angle);
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

double Object::computeOrbitalSpeed(double M, double r)
{
    return std::sqrt(GRAVITATIONAL_CONSTANT * M / r);
}

void Object::setInitialVelocity(cgp::vec3 velocity)
{
    this->velocity = velocity;
}

bool Object::getShouldTranslate() const
{
    return this->should_translate;
}

bool Object::getShouldRotate() const
{
    return this->should_rotate;
}