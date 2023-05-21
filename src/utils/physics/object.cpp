#include "object.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "utils/physics/constants.hpp"
#include <cmath>
#include <iostream>

Object::Object(double mass, cgp::vec3 position, cgp::vec3 rotation_axis, bool should_translate, bool should_rotate)
{
    // std::cout << "Generated object with physics position: " << position << std::endl; // BUG : dès le constructeur, c'est nan

    // Translations
    this->mass = mass;
    this->physics_position = position;
    this->velocity = {0, 0, 0};
    this->acceleration = {0, 0, 0};
    this->forces = {0, 0, 0};

    // Rotations
    this->rotation_angle = 0;
    this->rotation_axis = rotation_axis;
    this->rotation_speed = 0;

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
        this->velocity += this->acceleration * dt * ORBIT_FACTOR;
        this->physics_position += this->velocity * dt * ORBIT_FACTOR;
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

cgp::vec3 Object::computeOrbitalSpeedForPosition(double M, cgp::vec3 position, cgp::vec3 rotation_axis)
{
    const double orbitalSpeed = Object::computeOrbitalSpeed(M, cgp::norm(position));

    return orbitalSpeed * cgp::normalize(cgp::cross(rotation_axis, position));
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

void Object::setInitialRotationSpeed(double rotation_speed)
{
    this->rotation_speed = rotation_speed;
}

void Object::setRotationAxis(cgp::vec3 rotation_axis)
{
    this->rotation_axis = rotation_axis;
}
double Object::getMass() const
{
    return this->mass;
}

cgp::vec3 Object::getPhysicsVelocity() const
{
    return this->velocity;
}