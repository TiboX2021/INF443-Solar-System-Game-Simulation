#pragma once

#include "cgp/geometry/shape/mesh/structure/mesh.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "environment.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/object.hpp"
#include <string>

/**
 * Planète avec son apparence visuelle et ses propriétés physiques
 */
class Planet : public LowPolyDrawable, public Object
{
public:
    Planet();
    // Default perlin noise is no noise for the detailed constructor
    Planet(double mass, double radius, vec3 position, std::string texture_path = "assets/planets/mars.jpg", perlin_noise_parameters parameters = {0.0f, 0.0f, 0, 0.0f, 0.0f}, int Nu = 50, int Nv = 25);

    // Draw function
    virtual void initialize() override;
    virtual void draw_real(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true) override;

    // Setters
    virtual void setPosition(vec3 position) override;

    // Getters
    virtual cgp::mesh_drawable getMeshDrawable() const { return planet_mesh_drawable; };

    // Utility physics functions
    double getHeightAt(vec3 position) const; // TODO for collisions
    virtual void updateModels() override;

private:
    // Perlin noise properties
    perlin_noise_parameters parameters;

    // Planet properties
    double radius;
    vec3 position;
    std::string texture_path;

    int Nu = 50;
    int Nv = 25;

protected:
    // CGP elements
    cgp::mesh planet_mesh;
    cgp::mesh_drawable planet_mesh_drawable;
};