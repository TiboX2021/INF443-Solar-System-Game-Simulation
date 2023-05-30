#pragma once

#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include <cassert>
#include <string>
#include <vector>

// Useful tools & classes

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