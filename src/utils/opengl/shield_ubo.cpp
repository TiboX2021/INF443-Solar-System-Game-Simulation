#include "shield_ubo.hpp"

// Initialize OpenGL data
void ShieldUBO::initialize()
{
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    // Set the buffer size (with empty data)
    glBufferData(GL_UNIFORM_BUFFER, sizeof(collision_points), nullptr, GL_DYNAMIC_DRAW);

    // Unbind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Draw the shield with custom animation data sent to the fragment shader as uniforms
void ShieldUBO::draw(cgp::mesh_drawable const &drawable, cgp::environment_generic_structure const &environment, const std::vector<cgp::vec4> animation_data, cgp::uniform_generic_structure const &additional_uniforms, GLenum draw_mode)
{
    opengl_check;
    // Initial clean check
    // ********************************** //
    // If there is not vertices or not triangles, returns
    //  (no error + does not display anything)
    if (drawable.vbo_position.size == 0 || drawable.ebo_connectivity.size == 0)
        return;

    assert_cgp(drawable.shader.id != 0, "Try to draw mesh_drawable without shader ");
    assert_cgp(!glIsShader(drawable.shader.id), "Try to draw mesh_drawable with incorrect shader ");
    assert_cgp(drawable.texture.id != 0, "Try to draw mesh_drawable without texture ");

    // Set the current shader
    // ********************************** //
    glUseProgram(drawable.shader.id);
    opengl_check;

    // Send uniforms for this shader
    // ********************************** //

    // send the uniform values for the model and material of the mesh_drawable
    drawable.send_opengl_uniform();

    // send the uniform values for the environment
    environment.send_opengl_uniform(drawable.shader);

    // [Optionnal] send any additional uniform for this specidic draw call
    additional_uniforms.send_opengl_uniform(drawable.shader);

    // ********************************** //
    //        SEND CUSTOM UBO DATA        //
    // ********************************** //

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(collision_points), animation_data.data()); // Replace all data
    glBindBuffer(GL_UNIFORM_BUFFER, 0);                                                     // Unbind buffer

    // Set textures
    // ********************************** //
    glActiveTexture(GL_TEXTURE0);
    opengl_check;
    drawable.texture.bind();
    opengl_uniform(drawable.shader, "image_texture", 0);
    opengl_check;

    // Set any additional texture
    int texture_count = 1;
    for (auto const &element : drawable.supplementary_texture)
    {
        std::string const &additional_texture_name = element.first;
        cgp::opengl_texture_image_structure const &additional_texture = element.second;

        glActiveTexture(GL_TEXTURE0 + texture_count);
        opengl_check;
        additional_texture.bind();
        opengl_uniform(drawable.shader, additional_texture_name, texture_count);

        texture_count++;
    }

    // Prepare for draw call
    // ********************************** //
    glBindVertexArray(drawable.vao);
    opengl_check;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.ebo_connectivity.id);
    opengl_check;

    // Draw call
    // ********************************** //
    glDrawElements(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr);
    opengl_check;

    // Clean state
    // ********************************** //
    glBindVertexArray(0);
    drawable.texture.unbind();
    glUseProgram(0);
}
