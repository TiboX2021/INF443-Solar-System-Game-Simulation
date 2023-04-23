#include "planet.hpp"
#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/geometry/shape/noise/noise.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/noise/perlin.hpp"
#include <cassert>
#include <iostream>

// TODO : modèle physique ? On pourrait hériter d'une classe qui définit les fonctions
// Si jamais on fait beaucoup d'objets différents, ça pourrait être intéressant
// TODO : faire en sorte que la texture se répète plusieurs fois, pour l'adapter à la taille de la planète
// TODO : génération plus réaliste que juste du bruit ? Genre crevasses, etc à la minecraft

Planet::Planet() : LowPolyDrawable(5.0f)
{
    // Initialize planet data
    radius = 5.0f;
    position = {0, 0, 0};
    texture_path = "assets/planets/mars.jpg";
}

Planet::Planet(double radius, vec3 position, std::string texture_path) : LowPolyDrawable(radius)
{

    // Initialize planet data
    this->radius = radius;
    this->position = position;
    this->texture_path = texture_path;
}

// Initialize
void Planet::initialize()
{
    LowPolyDrawable::initialize(); // Call base class initialize function
    // Initialize CGP elements
    planet_mesh = mesh_primitive_perlin_sphere(radius, position, 200, 100, parameters);
    planet_mesh_drawable.initialize_data_on_gpu(planet_mesh);

    // Add texture
    planet_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + texture_path,
                                                                       GL_REPEAT,
                                                                       GL_REPEAT);

    setLowPolyColor({0, 1, 0}); // DEBUG : set visibel color
}

/**
 * Draw the planet in the given environment
 */
void Planet::draw_real(const environment_structure &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    cgp::draw(planet_mesh_drawable, environment);

    if (show_wireframe)
        cgp::draw_wireframe(planet_mesh_drawable, environment);
}

/**
 * Set the planet radius
 */
void Planet::setRadius(double radius)
{
    assert(radius >= 0 && "Radius must be positive");
    this->radius = radius;
}

/**
 * Set the planet position
 */
void Planet::setPosition(vec3 position)
{
    LowPolyDrawable::setPosition(position);
    this->position = position;
}

/**
 * Get the height of the planet at the given position
 */
double Planet::getHeightAt(vec3 position) const
{
    // Call noise function with normalized position
    return cgp::noise_perlin(cgp::normalize(position) * parameters.scale, parameters.octave, parameters.persistency, parameters.frequency_gain);
}