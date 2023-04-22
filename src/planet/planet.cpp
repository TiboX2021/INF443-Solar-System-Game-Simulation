#include "planet.hpp"
#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/shape/noise/noise.hpp"
#include "utils/noise/perlin.hpp"
#include <cassert>
#include <iostream>

// TODO : modèle physique ? On pourrait hériter d'une classe qui définit les fonctions
// Si jamais on fait beaucoup d'objets différents, ça pourrait être intéressant
// TODO : faire en sorte que la texture se répète plusieurs fois, pour l'adapter à la taille de la planète
// TODO : génération plus réaliste que juste du bruit ? Genre crevasses, etc à la minecraft

Planet::Planet()
{
    // Initialize planet data
    radius = 5.0f;
    position = {0, 0, 0};
    texture_path = "assets/planets/mars.jpg";
}

Planet::Planet(float radius, vec3 position, std::string texture_path)
{

    // Initialize planet data
    this->radius = radius;
    this->position = position;
    this->texture_path = texture_path;
}

// Initialize
void Planet::initialize()
{
    // Initialize CGP elements
    planet_mesh = mesh_primitive_perlin_sphere(radius, position, 200, 100, parameters);
    planet_mesh_drawable.initialize_data_on_gpu(planet_mesh);

    // Add texture
    planet_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + texture_path,
                                                                       GL_REPEAT,
                                                                       GL_REPEAT);
}

/**
 * Draw the planet in the given environment
 */
void Planet::draw(const environment_structure &environment, bool show_wireframe) const
{
    cgp::draw(planet_mesh_drawable, environment);

    if (show_wireframe)
        cgp::draw_wireframe(planet_mesh_drawable, environment);
}

/**
 * Set the planet radius
 */
void Planet::set_radius(float radius)
{
    assert(radius >= 0 && "Radius must be positive");
    this->radius = radius;
}

/**
 * Set the planet position
 */
void Planet::set_position(vec3 position)
{
    this->position = position;
}

/**
 * Get the height of the planet at the given position
 */
double Planet::get_height_at(vec3 position) const
{
    // Call noise function with normalized position
    return cgp::noise_perlin(cgp::normalize(position) * parameters.scale, parameters.octave, parameters.persistency, parameters.frequency_gain);
}