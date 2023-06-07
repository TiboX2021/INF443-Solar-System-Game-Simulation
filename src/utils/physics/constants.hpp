#pragma once

#include <cmath>

constexpr float PI = 3.1415926535897932;
#define EARTH_ROTATION_AXIS   \
    {                         \
        0, 0.397949, 0.917408 \
    }

#define MARS_ROTATION_AXIS            \
    {                                 \
        -0.054875, 0.110222, 0.992391 \
    }

#define SATURN_ROTATION_AXIS                        \
    {                                               \
        0.0, 0.8931360029996426, 0.4497867051679301 \
    }

// 3,12° de tilt !
#define JUPITER_ROTATION_AXIS        \
    {                                \
        0.000000, 0.054427, 0.998518 \
    }

#define URANUS_ROTATION_AXIS         \
    {                                \
        0.546671, 0.837098, 0.020437 \
    }

#define NEPTUNE_ROTATION_AXIS          \
    {                                  \
        -0.724734, 0.684821, -0.076032 \
    }

constexpr double GRAVITATIONAL_CONSTANT = 6.67408e-11;

// Sun constants
constexpr double SUN_MASS = 1988500e24; // In kg
constexpr double SUN_RADIUS = 696340e3; // In meters

// Mars constants
constexpr double MARS_MASS = 0.64185e24;                            // In kg
constexpr double MARS_RADIUS = 3389.5e3;                            // In meters
constexpr double MARS_SUN_DISTANCE = 227.9e9;                       // In meters
constexpr double MARS_ROTATION_SPEED = (2 * PI) / (24.6229 * 3600); // In rad/day

// Earth constants
constexpr double EARTH_RADIUS = 6.371e+6;                       // In meters
constexpr double EARTH_MASS = 5.9742e+24;                       // In kg
constexpr double EARTH_SUN_DISTANCE = 150.53e9;                 // In meters
constexpr double EARTH_ROTATION_SPEED = 2.0 * PI / (3600 * 24); // In rad/day

// Saturn constants
constexpr double SATURN_MASS = 568.32e24;                                   // In kg
constexpr double SATURN_RADIUS = 58232e3;                                   // In meters
constexpr double SATURN_SUN_DISTANCE = 1.4652e12;                           // In meters
constexpr double SATURN_ROTATION_SPEED = (2 * PI) / (10.67 * 24 * 60 * 60); // In rad/day

// Jupiter constants
constexpr double JUPITER_MASS = 1898.13e24;                                  // In kg
constexpr double JUPITER_RADIUS = 69911e3;                                   // In meters
constexpr double JUPITER_SUN_DISTANCE = 778.57e9;                            // In meters
constexpr double JUPITER_ROTATION_SPEED = (2 * PI) / (9.925 * 24 * 60 * 60); // In rad/day

// Uranus constants
constexpr double URANUS_MASS = 86.8103e24;                                  // In kg
constexpr double URANUS_RADIUS = 25362e3;                                   // In meters
constexpr double URANUS_SUN_DISTANCE = 1.7832e12;                           // In meters
constexpr double URANUS_ROTATION_SPEED = (2 * PI) / (17.24 * 24 * 60 * 60); // In rad/day

// Neptune constants
constexpr double NEPTUNE_MASS = 102.41e24;                                   // In kg
constexpr double NEPTUNE_RADIUS = 24622e3;                                   // In meters
constexpr double NEPTUNE_SUN_DISTANCE = 2.795e12;                            // In meters
constexpr double NEPTUNE_ROTATION_SPEED = (2 * PI) / (16.11 * 24 * 60 * 60); // In rad/day

// Jupiter constants