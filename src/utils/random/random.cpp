#include "random.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include <cmath>

float random_float(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(float(RAND_MAX) / (max - min)));
}

cgp::vec3 random_axis()
{
    float theta = random_float(0, M_PI);
    float phi = random_float(0, 2 * M_PI);

    return cgp::vec3{sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
}