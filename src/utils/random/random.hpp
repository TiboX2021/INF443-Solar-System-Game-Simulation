#pragma once

// Generate random rotation axis & random sizes
// Used for asteroids

#include "cgp/geometry/vec/vec3/vec3.hpp"
#include <cstdlib>

// Generate a random floating point number
float random_float(float min = 0.0f, float max = 1.0f);

// Generate a random normalized 3d vector
cgp::vec3 random_axis();