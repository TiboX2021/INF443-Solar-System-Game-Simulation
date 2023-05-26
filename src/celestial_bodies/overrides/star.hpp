#pragma once

#include "celestial_bodies/planet/planet.hpp"
#include "utils/shaders/shader_loader.hpp"

// A star class specifically created to override the Planet shader
// A star has more vertices than a default Planet in order to have smoother shader animations
class Star : public Planet
{
public:
    Star(double mass, double radius, vec3 position, std::string texture_path = "assets/planets/mars.jpg", perlin_noise_parameters parameters = {0.0f, 0.0f, 0, 0.0f, 0.0f}) : Planet(mass, radius, position, texture_path, parameters, 200, 100){};

    // Set a custom shader
    void setShader(const std::string &name)
    {
        custom_shader_name = name;
    }

    // Override the initialize method to add the custom shader after initialization
    virtual void initialize() override
    {
        Planet::initialize();
        planet_mesh_drawable.shader = ShaderLoader::getShader(custom_shader_name);
    }

private:
    std::string custom_shader_name;
};