#include "ring_planet.hpp"
#include "celestial_bodies/planet/planet.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "cgp/graphics/opengl/shaders/shaders.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/physics/constants.hpp"
#include <iostream>
#include <iterator>

RingPlanet::RingPlanet() : Planet(SATURN_MASS, SATURN_RADIUS, {0, 0, 0}, "assets/planets/saturn.jpg")
{
    // Default ring texture path
    ring_texture_path = "assets/planets/rings/saturn_ring.png";
    ring_radius = scaleRadiusForDisplay(SATURN_RADIUS * 2);
}

// Non default constructor
RingPlanet::RingPlanet(double mass, double radius, double ring_radius, vec3 position, std::string texture_path, std::string ring_texture_path, perlin_noise_parameters parameters) : Planet(mass, radius, position, texture_path, parameters)
{
    this->ring_texture_path = ring_texture_path;
    this->ring_radius = scaleRadiusForDisplay(ring_radius);
}

void RingPlanet::initialize()
{
    Planet::initialize(); // Call base class initialize function

    // Quadrangle with correct radius centered on 0
    ring_quad_mesh = mesh_primitive_quadrangle({-ring_radius, -ring_radius, 0}, {ring_radius, -ring_radius, 0}, {ring_radius, ring_radius, 0}, {-ring_radius, ring_radius, 0});
    ring_mesh_drawable.initialize_data_on_gpu(ring_quad_mesh);
    setPosition((getPhysicsPosition())); // Set the position for the ring
    ring_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + ring_texture_path);
    ring_mesh_drawable.material.phong.specular = 0; // No reflection for the planet ring
    ring_mesh_drawable.material.texture_settings.two_sided = true;
}

void RingPlanet::drawBillboards(const environment_structure &environment, cgp::vec3 &position, cgp::rotation_transform &, bool)
{
    if (!shouldDrawLowPoly(position))
    {
        cgp::draw(ring_mesh_drawable, environment);
    }
}

void RingPlanet::setPosition(vec3 position)
{
    Planet::setPosition(position);

    ring_mesh_drawable.model.translation = scaleDownDistanceForDisplay(position);
}

void RingPlanet::updateModels()
{
    Planet::updateModels();

    if (getShouldRotate())
    {
        // Update rotation
        ring_mesh_drawable.model.rotation = getPhysicsRotation();
    }
}