#pragma once

#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "utils/physics/object.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Useful tools & classes

constexpr float NORMALIZE_FLOAT_MARGIN = 0.01;

template <class T>
class ObjectBuffer
{
public:
    ObjectBuffer(int size, T default_value = T()) : size(size + 1), current_return_index(0), current_add_index(size)
    {
        buffer = std::vector<T>(size + 1);

        // Fill the buffer with default values
        for (int i = 0; i < size + 1; i++)
        {
            buffer[i] = default_value;
        }
    };

    // Functions
    T getNextOne()
    {
        assert(current_return_index != current_add_index && "Trying to read from empty buffer");

        // Get object
        T current_obj = buffer[current_return_index];

        // Increment current counter to next one
        current_return_index = (current_return_index + 1) % size;

        return current_obj;
    }
    void add(T element)
    {
        assert(current_return_index != current_add_index && "Trying to add a value to a full buffer");

        // Add object
        buffer[current_add_index] = element;

        // Increment current counter to next one
        current_add_index = (current_add_index + 1) % size;
    }

private:
    std::vector<T> buffer;
    int size;
    int current_return_index;
    int current_add_index;
};

// Return a mix of both vectors with the specified ratio for the first one
inline cgp::vec3 vector_mix(const cgp::vec3 &v1, const cgp::vec3 &v2, float ratio)
{
    return cgp::normalize(v1 * ratio + v2 * (1 - ratio));
}

// Render v1 perpendicular to v2 by removing its projection along v2
inline cgp::vec3 perpendicular_projection(const cgp::vec3 &v1, const cgp::vec3 &v2)
{
    return cgp::normalize(v1 - v2 * cgp::dot(v1, v2));
}

// Given an incoming vector and a normal, return the reflected vector
inline cgp::vec3 reflect(const cgp::vec3 &incoming, const cgp::vec3 &normal)
{
    return incoming - 2 * cgp::dot(incoming, normal) * normal;
}

// Normalize, but {0, 0, 0}-proof
inline cgp::vec3 normalize_or_zero(const cgp::vec3 &v)
{
    return cgp::norm(v) > NORMALIZE_FLOAT_MARGIN ? cgp::normalize(v) : cgp::vec3(0, 0, 0);
}

// Returns the intersection point on the circle surface from the segment [p1, p2]
inline cgp::vec3 circle_intersect_point(const cgp::vec3 &center, float radius, const cgp::vec3 &point, const cgp::vec3 &direction)
{
    // Compute polynomial coeffs
    float a = cgp::dot(direction, direction);
    float b = 2 * cgp::dot(direction, point - center);
    float c = cgp::dot(point - center, point - center) - radius * radius;

    // Compute discriminant
    float delta = b * b - 4 * a * c;

    // If there is no solution, return the point
    if (delta < 0)
    {
        return point;
    }

    // Compute solutions
    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);

    // Return the closest point
    return point + direction * std::min(t1, t2);
}

inline bool does_clip_with_one(const std::vector<Object *> &objects, cgp::vec3 position)
{

    for (Object *object : objects)
    {
        if (object->isInside(position))
        {
            return true;
        }
    }

    return false;
}

// Check if the position collides with one of the objects. If it does, push it back outside with an extra radius
inline bool check_clip_and_push_back(const std::vector<Object *> &objects, cgp::vec3 &position, float extra_radius = 0)
{
    for (Object *object : objects)
    {
        if (object->isInside(position, extra_radius))
        {
            // Push back the position
            cgp::vec3 direction = cgp::normalize(position - object->getPhysicsPosition());

            position = object->getPhysicsPosition() + (object->getPhysicsRadius() + extra_radius) * direction;
            return true;
        }
    }

    return false;
}