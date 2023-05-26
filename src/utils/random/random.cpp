#include "random.hpp"
#include "cgp/geometry/vec/vec3/vec3.hpp"
#include <cmath>
#include <random>

float random_float(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(float(RAND_MAX) / (max - min)));
}

float random_gaussian(float mean, float std_dev)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, std_dev);
    return distribution(gen);

    return 0;
}

int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

cgp::vec3 random_normalized_axis()
{
    float theta = random_float(0, M_PI);
    float phi = random_float(0, 2 * M_PI);

    return cgp::vec3{sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
}

// TODO : complete this
cgp::vec3 random_orbit_position(float radius, cgp::vec3 rotation_axis)
{

    float angle_position = random_float(0, M_PI * 2);

    return cgp::vec3{radius * cos(angle_position), radius * sin(angle_position), 0};
}