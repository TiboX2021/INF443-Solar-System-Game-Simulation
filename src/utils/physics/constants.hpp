#pragma once

#include <cmath>

#define EARTH_ROTATION_AXIS   \
    {                         \
        0, 0.397949, 0.917408 \
    }
#define SATURN_ROTATION_AXIS          \
    {                                 \
        0.559455, -0.825413, 0.074357 \
    }

constexpr double GRAVITATIONAL_CONSTANT = 6.67408e-11;

// Sun constants
constexpr double SUN_MASS = 1988500e24; // In kg
constexpr double SUN_RADIUS = 696340e3; // In meters

// Earth constants
constexpr double EARTH_RADIUS = 6.371e+6;                         // In meters
constexpr double EARTH_MASS = 5.9742e+24;                         // In kg
constexpr double EARTH_SUN_DISTANCE = 150.53e9;                   // In meters
constexpr double EARTH_ROTATION_SPEED = 2.0 * M_PI / (3600 * 24); // In rad/day

// Saturn constants
constexpr double SATURN_MASS = 568.32e24;                                     // In kg
constexpr double SATURN_RADIUS = 58232e3;                                     // In meters
constexpr double SATURN_SUN_DISTANCE = 1.4652e12;                             // In meters
constexpr double SATURN_ROTATION_SPEED = (2 * M_PI) / (10.67 * 24 * 60 * 60); // In rad/day

// Jupiter constants