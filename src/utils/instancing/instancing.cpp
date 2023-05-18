#include "instancing.hpp"

namespace cgp
{

    void draw_instanced(mesh_drawable const &drawable, environment_generic_structure const &environment, const std::vector<vec3> &positions, const std::vector<rotation_transform> &orientations, uniform_generic_structure const &additional_uniforms, GLenum draw_mode)
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
            opengl_texture_image_structure const &additional_texture = element.second;

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

}