
#include "terrain.hpp"
#include "cgp/core/base/rand/rand.hpp"
#include "cgp/core/containers/matrix_stack/matrix_stack.hpp"
#include "cgp/geometry/shape/noise/noise.hpp"
#include <cmath>

using namespace cgp;
#define TERRAIN_OFFSET -0.02
#define CLIPPING_DISTANCE 0.8

// Evaluate 3D position of the terrain for any (x,y)
// float evaluate_terrain_height(float x, float y)
// {
//     vec2 p_0 = {0, 0};    // Position du centre :  en 0, 0
//     float h_0 = 2.0f;     // Coeff de hauteur de la gaussienne
//     float sigma_0 = 3.0f; // Coeff de largeur de la gaussienne

//     float d = norm(vec2(x, y) - p_0) / sigma_0;

//     float z = h_0 * std::exp(-d * d);

//     return z;
// }

// Idem mais avec des paramètres différents (utiliser default arguments)
// TODO: stocker ces paramètres au préalable pour éviter de les instancier à nouveau?
// Remarque : c'est juste pour la génération du terrain, donc c'est pas très grave pour l'instant
float evaluate_terrain_height(float x, float y, perlin_noise_parameters const &parameters, float terrain_length)
{
    int length = 4;
    vec2 p_i[] = {{-10, -10}, {5, 5}, {-3, 4}, {6, 4}};
    float h_i[] = {3, -1.5, 1, 2};
    float sigma_i[] = {10, 3, 4, 4};

    // Computations
    // Sum of exponents
    float z = 0;
    for (int i = 0; i < length; ++i)
    {
        z += h_i[i] * std::exp(-std::pow(norm(vec2(x, y) - p_i[i]) / sigma_i[i], 2));
    }

    // TODO : ici, u et v ne sont probablement pas bons ! Il faudra les scale

    float u = x / terrain_length + 0.5f;
    float v = y / terrain_length + 0.5f;

    z += noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);

    return z;
}

mesh create_terrain_mesh(int N, float terrain_length, perlin_noise_parameters const &parameters)
{

    mesh terrain; // temporary terrain storage (CPU only)

    terrain.uv.resize(N * N); // Application d'une texture
    terrain.position.resize(N * N);

    // Fill terrain geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x, y, parameters, terrain_length);

            // Store vertex coordinates + add perlin noise
            terrain.position[kv + N * ku] = {x, y, z};

            // Store uv coordinates
            terrain.uv[kv + N * ku] = {u * 10, v * 10};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = {idx, idx + 1 + N, idx + 1};
            uint3 triangle_2 = {idx, idx + N, idx + 1 + N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();

    return terrain;
}

// Crée des positions sur le terrain
std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length, perlin_noise_parameters const &parameters)
{
    std::vector<cgp::vec3> positions;
    bool clip = true;
    float x, y, z;

    for (int i = 0; i < N; ++i)
    {
        clip = true;
        while (clip)
        {
            // Generate random coordinates
            x = (rand_interval() - 0.5) * terrain_length;
            y = (rand_interval() - 0.5) * terrain_length;
            z = evaluate_terrain_height(x, y, parameters, terrain_length) + TERRAIN_OFFSET;

            // Check for clipping with other trees
            clip = false;
            for (int j = 0; j < positions.size(); ++j)
            {
                if (norm(positions[j] - vec3{x, y, z}) < CLIPPING_DISTANCE)
                {
                    clip = true;
                }
            }
        }

        positions.push_back(vec3{x, y, z});
    }
    return positions;
}

void update_terrain(mesh &terrain, mesh_drawable &terrain_visual, perlin_noise_parameters const &parameters, float terrain_length)
{
    // Number of samples in each direction (assuming a square grid)
    int const N = std::sqrt(terrain.position.size());

    // Recompute the new vertices
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {

            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            int const idx = ku * N + kv;

            // Update height with noise
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            terrain.position[idx].z = evaluate_terrain_height(x, y, parameters, terrain_length);
        }
    }

    // Update the normal of the mesh structure
    terrain.normal_update();

    // Update step: Allows to update a mesh_drawable without creating a new one
    terrain_visual.vbo_position.update(terrain.position);
    terrain_visual.vbo_normal.update(terrain.normal);
    terrain_visual.vbo_color.update(terrain.color);
}
