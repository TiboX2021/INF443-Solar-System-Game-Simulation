#include "perlin.hpp"
#include "cgp/geometry/shape/noise/noise.hpp"

/*
RAPPEL SUR L'ORGANISATION DES POINTS
Direction des axes :

V
^
|
| - - -> U

Ordre des points dans la sphère :
^  ^  ^
\  \  \
\  \  \
\  \  \
--------- >

Ie :
* 1 de plus selon V : c'est le point suivant, +1
* 1 de plus selon U : c'est la colonne suivante, donc +Nv (la hauteur de la colonne)
*/

// Pris directement depuis cgp::mesh_primitive.cpp
// Nu et Nv : nombres de points sur la sphère à connecter
static cgp::numarray<cgp::uint3> connectivity_grid(size_t Nu, size_t Nv)
{
    cgp::numarray<cgp::uint3> connectivity;
    int n_points = Nu * Nv;

    // Modif : la boucle for va jusqu'à Nu et Nv au lieu de s'arrêter à -1 : boucler la boucle
    for (size_t ku = 0; ku < Nu; ++ku)
    {
        for (size_t kv = 0; kv < Nv; ++kv)
        {
            unsigned int k00 = static_cast<unsigned int>((kv + Nv * ku % n_points));
            unsigned int k10 = static_cast<unsigned int>((kv + 1 + Nv * ku) % n_points);
            unsigned int k01 = static_cast<unsigned int>((kv + Nv * (ku + 1)) % n_points);
            unsigned int k11 = static_cast<unsigned int>((kv + 1 + Nv * (ku + 1)) % n_points);

            connectivity.push_back(cgp::uint3{k00, k10, k11});
            connectivity.push_back(cgp::uint3{k00, k11, k01});
        }
    }
    return connectivity;
}

/**
 * cgp::mesh_primitive_sphere with perlin noise added to the surface
 */
cgp::mesh mesh_primitive_perlin_sphere(float radius, cgp::vec3 const &center, int Nu, int Nv, perlin_noise_parameters parameters)
{
    assert_cgp(radius > 0, "Sphere radius should be > 0");
    assert_cgp(Nu > 2 && Nv > 2, "Sphere samples should be > 2");

    cgp::mesh shape;
    for (size_t ku = 0; ku < size_t(Nu); ++ku)
    {
        for (size_t kv = 0; kv < size_t(Nv); ++kv)
        {
            float const u = ku / (Nu - 1.0f);
            float const alpha = kv / (Nv - 1.0f);
            float const v = 1.0f / (Nv + 1.0f) * (1 - alpha) + alpha * Nv / (Nv + 1.0f);

            float const theta = 2.0f * cgp::Pi * (u - 0.5f);
            float const phi = cgp::Pi * (v - 0.5f);

            // spherical coordinates
            cgp::vec3 const n = {
                std::cos(phi) * std::cos(theta),
                std::cos(phi) * std::sin(theta),
                std::sin(phi)};

            // Using 3D perlin noise
            float perlin_noise_value = cgp::noise_perlin(n * parameters.scale, parameters.octave, parameters.persistency, parameters.frequency_gain);

            cgp::vec3 const p = (radius + perlin_noise_value) * n + center;
            cgp::vec2 const uv = {u, v};

            shape.position.push_back(p);
            shape.normal.push_back(n);
            shape.uv.push_back(uv);
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv);

    // poles
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
    {
        shape.position.push_back(center + radius * cgp::vec3{0, 0, -1.0f});
        shape.normal.push_back(cgp::vec3{0, 0, -1.0f});
        shape.uv.push_back({ku / (Nu - 1.0f), 0.0f});
    }
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
        shape.connectivity.push_back({Nu * Nv + ku, Nv * ku, Nv * (ku + 1)});

    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
    {
        shape.position.push_back(center + radius * cgp::vec3{0, 0, 1.0f});
        shape.normal.push_back(cgp::vec3{0, 0, 1.0f});
        shape.uv.push_back({ku / (Nu - 1.0f), 1.0f});
    }
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
        shape.connectivity.push_back({Nu * Nv + Nu - 1 + ku, Nv - 1 + Nv * (ku + 1), Nv - 1 + Nv * ku});

    shape.fill_empty_field();
    shape.flip_connectivity();
    return shape;
}