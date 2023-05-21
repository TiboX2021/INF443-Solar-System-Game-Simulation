#pragma once

#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include <vector>

// Define draw function for instanced rendering (multiple asteroids, for instance)
namespace cgp
{

    void draw_instanced(mesh_drawable const &drawable, environment_generic_structure const &environment = environment_generic_structure(), const std::vector<vec3> &positions = {}, const std::vector<mat3> &orientations = {}, const std::vector<float> &scales = {}, int n_instances = 1, uniform_generic_structure const &additional_uniforms = uniform_generic_structure(), GLenum draw_mode = GL_TRIANGLES);
}