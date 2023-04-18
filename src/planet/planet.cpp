#include "planet.hpp"
#include <cassert>
#include <iostream>

Planet::Planet()
{
    // Initialize planet data
    radius = 10.0f;
    position = {0, 0, 5};
    texture_path = "assets/planets/mars.jpg";

    // Initialize CGP elements
    planet_mesh = cgp::mesh_primitive_sphere(radius, position);
}

Planet::Planet(float radius, vec3 position, std::string texture_path)
{

    // Initialize planet data
    this->radius = radius;
    this->position = position;
    this->texture_path = texture_path;

    // Initialize CGP elements
    planet_mesh = cgp::mesh_primitive_sphere(radius, position);
    planet_mesh_drawable.initialize_data_on_gpu(planet_mesh);
}

// Initialize
void Planet::initialize()
{
    planet_mesh_drawable.initialize_data_on_gpu(planet_mesh);
    // planet_mesh_drawable.material.color = {1, 0, 0};
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