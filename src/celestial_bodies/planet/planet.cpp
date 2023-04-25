#include "planet.hpp"
#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/shape/noise/noise.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/object.hpp"
#include <cassert>
#include <iostream>

// TODO : génération plus réaliste que juste du bruit ? Genre crevasses, etc à la minecraft

Planet::Planet() : LowPolyDrawable(scaleRadiusForDisplay(EARTH_RADIUS)), Object(EARTH_MASS, {0, 0, 0}, EARTH_ROTATION_AXIS)
{
    // Initialize planet data
    radius = scaleRadiusForDisplay(EARTH_RADIUS);
    position = scaleDownDistanceForDisplay({0, 0, 0});
    // texture_path = "assets/planets/mars.png";
    texture_path = "assets/planets/earth.jpg";

    // Reset perlin parameters for earth
    parameters = NO_PERLIN_NOISE;
}

Planet::Planet(double mass, double radius, vec3 position, std::string texture_path, perlin_noise_parameters parameters) : LowPolyDrawable(scaleRadiusForDisplay(radius)), Object(mass, position)
{

    // Initialize display planet data
    this->radius = scaleRadiusForDisplay(radius);
    this->position = scaleDownDistanceForDisplay(position);
    this->texture_path = texture_path;
    this->parameters = parameters;
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
                                                                       GL_CLAMP_TO_EDGE,
                                                                       GL_CLAMP_TO_EDGE);

    setLowPolyColor({221.0f / 255, 108.0f / 255, 75.0f / 255});

    planet_mesh_drawable.material.phong.specular = 0; // No reflection for the planet display
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
 * Set the planet position
 */
void Planet::setPosition(vec3 position)
{
    cgp::vec3 scaled_position = scaleDownDistanceForDisplay(position);
    LowPolyDrawable::setPosition(scaled_position);
    planet_mesh_drawable.model.translation = scaled_position;
    this->position = scaled_position;
    setPhysicsPosition(position);
}

/**
 * Get the height of the planet at the given position
 */
double Planet::getHeightAt(vec3 position) const
{
    // Call noise function with normalized position
    return cgp::noise_perlin(cgp::normalize(position) * parameters.scale, parameters.octave, parameters.persistency, parameters.frequency_gain);
}

// Update models based on physics members
void Planet::updateModels()
{
    if (getShouldTranslate())
    { // Update position
        setPosition(getPhysicsPosition());
    }
    if (getShouldRotate())
    {
        // Update rotation
        planet_mesh_drawable.model.rotation = getPhysicsRotation();
    }
}