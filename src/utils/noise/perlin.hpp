#pragma once

#include "cgp/geometry/shape/mesh/structure/mesh.hpp"

struct perlin_noise_parameters
{
    float persistency = 0.35f;
    float frequency_gain = 2.0f;
    int octave = 6;
    float terrain_height = 0.5f;
    float scale = 1.0f; // Scale for the perlin coordinates
};

constexpr perlin_noise_parameters NO_PERLIN_NOISE = {0, 0, 0, 0, 0};

cgp::mesh mesh_primitive_perlin_sphere(float radius = 1.0f, cgp::vec3 const &center = {0, 0, 0}, int Nu = 40, int Nv = 20, perlin_noise_parameters parameters = {0.05f, 2, 3, 0.5f, 1.0f});
