#pragma once

#include "celestial_bodies/planet/planet.hpp"
#include "utils/display/billboard_drawable.hpp"

// Planet with a ring image billboard (Saturn, etc)
class RingPlanet : public Planet, public BillboardDrawable
{
public:
    RingPlanet();
    RingPlanet(double mass, double radius, double ring_radius, vec3 position, std::string texture_path = "assets/planets/saturn.jpg", std::string ring_texture_path = "assets/planets/rings/saturn_ring.png", perlin_noise_parameters parameters = {1.0f, 1.0f, 1, 0.0f, 1.0f});

    // Draw functions
    virtual void initialize() override;
    void drawBillboards(const environment_structure &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true) override;

    virtual void setPosition(vec3 position) override;
    virtual void updateModels() override;

private:
    // Ring data
    std::string ring_texture_path;
    double ring_radius;

    // CGP elements
    cgp::mesh ring_quad_mesh;
    cgp::mesh_drawable ring_mesh_drawable;
};