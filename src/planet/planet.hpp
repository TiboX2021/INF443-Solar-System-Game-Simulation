#pragma once

#include "cgp/geometry/shape/mesh/structure/mesh.hpp"
#include "environment.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/noise/perlin.hpp"
#include <string>

/**
 * Planète avec son apparence visuelle et ses propriétés physiques
 */
class Planet : public LowPolyDrawable
{
public:
    Planet();
    Planet(double radius, vec3 position, std::string texture_path = "assets/planets/mars.jpg");

    // Draw function
    virtual void initialize() override;
    virtual void draw_real(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

    // Setters
    void setRadius(double radius);
    virtual void setPosition(vec3 position) override;

    // Utility physics functions
    double getHeightAt(vec3 position) const;

private:
    // Perlin noise properties
    perlin_noise_parameters parameters;

    // Planet properties
    double radius;
    vec3 position;
    std::string texture_path;

    // CGP elements
    cgp::mesh planet_mesh;
    cgp::mesh_drawable planet_mesh_drawable;
};