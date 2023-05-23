#pragma once

#include "planet.hpp"

// Planet with a ring image billboard (Saturn, etc)
class RingPlanet : public Planet
{
public:
    RingPlanet();
    RingPlanet(double radius, double ring_radius, vec3 position, std::string texture_path = "assets/planets/saturn.jpg", std::string ring_texture_path = "assets/planets/rings/saturn_ring.png", perlin_noise_parameters parameters = {1.0f, 1.0f, 1, 0.0f, 1.0f});

    // Draw functions
    virtual void initialize() override;
    void draw_ring_billboard(const environment_structure &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);
    void draw_ring_billboard(const environment_structure &environment, camera_controller_first_person_euler const &camera, bool show_wireframe = true);

    virtual void setPosition(vec3 position) override;

private:
    // Ring data
    std::string ring_texture_path;
    double ring_radius;

    // CGP elements
    cgp::mesh ring_quad_mesh;
    cgp::mesh_drawable ring_mesh_drawable;
};