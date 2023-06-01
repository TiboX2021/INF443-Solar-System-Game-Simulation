#pragma once

#include "cgp/core/array/numarray_stack/special_types/special_types.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"

constexpr int SHIELD_COLLISION_BUFFER_MAX_SIZE = 10; // Max animations at the same time

// Draw the shield with a custom UBO  (Uniform Buffer Object)
// Because it is a complex data structure, it cannot be sent using regular cgp functions
struct collision_points
{
    cgp::vec4 data[SHIELD_COLLISION_BUFFER_MAX_SIZE];
    int size;
};

// Stores thet UBO object, and draws the shield by using it to send the collision data
class ShieldUBO
{
public:
    // Instanciate ubo
    void initialize();

    // Draw with ubo
    void draw(cgp::mesh_drawable const &drawable, cgp::environment_generic_structure const &environment, const std::vector<cgp::vec4> animation_data = {}, cgp::uniform_generic_structure const &additional_uniforms = cgp::uniform_generic_structure(), GLenum draw_mode = GL_TRIANGLES);

private:
    GLuint ubo;
};