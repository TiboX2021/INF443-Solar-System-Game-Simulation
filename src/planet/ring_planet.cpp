#include "ring_planet.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "cgp/graphics/opengl/shaders/shaders.hpp"
#include "utils/display/low_poly.hpp"

RingPlanet::RingPlanet() : Planet(5.0f, {0, 0, 0}, "assets/planets/saturn.jpg")
{
    // Default ring texture path
    ring_texture_path = "assets/planets/rings/saturn_ring.png";
    ring_radius = 10.0f;
}

// Non default constructor
RingPlanet::RingPlanet(double radius, double ring_radius, vec3 position, std::string texture_path, std::string ring_texture_path, perlin_noise_parameters parameters) : Planet(radius, position, texture_path, parameters)
{
    this->ring_texture_path = ring_texture_path;
    this->ring_radius = ring_radius;
}

void RingPlanet::initialize()
{
    Planet::initialize(); // Call base class initialize function

    // Quadrangle with correct radius centered on 0
    ring_quad_mesh = mesh_primitive_quadrangle({0, 0, 0}, {2 * ring_radius, 0, 0}, {2 * ring_radius, 2 * ring_radius, 0}, {0, 2 * ring_radius, 0});
    ring_mesh_drawable.initialize_data_on_gpu(ring_quad_mesh);
    setPosition({0, 0, 0}); // Set the position for the ring
    ring_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + ring_texture_path);
    ring_mesh_drawable.material.phong.specular = 0; // No reflection for the planet ring
    ring_mesh_drawable.material.texture_settings.two_sided = true;
    setLowPolyColor({207.0f / 255, 171.0f / 255, 134.0f / 255});
}

void RingPlanet::draw_ring_billboard(const environment_structure &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    if (!shouldDrawLowPoly(camera))
    {
        cgp::draw(ring_mesh_drawable, environment);
    }
}

void RingPlanet::setPosition(vec3 position)
{
    Planet::setPosition(position);
    ring_mesh_drawable.model.translation = position - cgp::vec3{ring_radius, ring_radius, 0};
}