#include "object.hpp"

double Object::time_scale = 1.0; // Default time scale

Object::Object(double mass, cgp::vec3 position)
{
    this->mass = mass;
    this->position = position;
    this->velocity = {0, 0, 0};
    this->acceleration = {0, 0, 0};
    this->forces = {0, 0, 0};
}

void Object::reset_forces()
{
    this->forces = {0, 0, 0};
}

/**
 * Compute gravitational force from another object
 */
void Object::compute_force(Object *other)
{
    cgp::vec3 distance = other->position - this->position;
    forces += GRAVITAIONAL_CONSTANT * this->mass * other->mass / cgp::dot(distance, distance) * cgp::normalize(distance);
}

/** Update position */
void Object::update(double dt)
{
    this->acceleration = this->forces / this->mass;
    this->velocity += this->acceleration * dt;

    this->setPosition(this->velocity * dt);
}

// : voir si ça call bien la fonction override dans les sous classes. Ça risque de pas être hyper facile à gérer si je peux update les positions des mesh en même temps
void Object::setPosition(cgp::vec3 position)
{
    this->position = position;
}
