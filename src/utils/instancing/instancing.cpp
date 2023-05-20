#include "instancing.hpp"
#include "cgp/core/containers/matrix_stack/special_types/definition/special_types.hpp"
#include <iostream>

namespace cgp
{
    // TODO : create an updated shader accordingly & test it with the debug star
    // TODO : how to change the shader and send the correct vbos for instancing ?
    // DEBUG : voir la page suivante pour compléter mon code et faire en sorte que ça marche
    // https://learnopengl.com/Advanced-OpenGL/Instancing
    // Faire du array instancing. On pourra même y mettre des scale !

    void draw_instanced(mesh_drawable const &drawable, environment_generic_structure const &environment, const std::vector<vec3> &positions, const std::vector<mat3> &orientations, int n_instances, uniform_generic_structure const &additional_uniforms, GLenum draw_mode)
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
        // Note : uniform values are heavy, do not send asteroid position with this means.

        // Final model matrix in the shader is: hierarchy_transform_model * model
        mat4 const model_shader = drawable.hierarchy_transform_model.matrix() * drawable.model.matrix();

        // The normal matrix is transpose( (hierarchy_transform_model * model)^{-1} )
        mat4 const model_normal_shader = transpose(inverse(drawable.model).matrix() * inverse(drawable.hierarchy_transform_model).matrix());

        // set the Model matrix
        // Note : with instancing, we will not use the default position set in those matrices, only the model meshes
        opengl_uniform(drawable.shader, "model", model_shader, true);
        opengl_uniform(drawable.shader, "modelNormal", model_normal_shader, true);

        // set the material
        drawable.material.send_opengl_uniform(drawable.shader);

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
        // Bind predefined vao
        glBindVertexArray(drawable.vao);
        opengl_check;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.ebo_connectivity.id);
        opengl_check;

        // ********************************** //
        //     Custom instancing OpenGL code  //
        // ********************************** //
        GLuint custom_vbo = 0;                                                                            // vbo (contains vertex information, like a vec3)
        glGenBuffers(1, &custom_vbo);                                                                     // Generate buffer
        glBindBuffer(GL_ARRAY_BUFFER, custom_vbo);                                                        // Bind buffer to configure it right now
        glBufferData(GL_ARRAY_BUFFER, sizeof(cgp::vec3) * n_instances, positions.data(), GL_STATIC_DRAW); // Send data + format to buffer
        glEnableVertexAttribArray(4);                                                                     // This positions array will be in shader layout position 4
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);                    // Buffer of position 4 is composed of 3 non normalized floats (vec3)
        glVertexAttribDivisor(4, 1);                                                                      // 1 instead of 0 : this position attribute will change for every INSTANCE, not every VERTEX
        glBindBuffer(GL_ARRAY_BUFFER, 0);                                                                 // Unbind buffer (bind to 0 again)

        GLuint rotations_vbo = 0;
        glGenBuffers(1, &rotations_vbo);                                                                     // Create VBO buffer for the rotation matrix array
        glBindBuffer(GL_ARRAY_BUFFER, rotations_vbo);                                                        // Bind VBO buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(cgp::mat3) * n_instances, orientations.data(), GL_STATIC_DRAW); // Define VBO size, data, and type

        // Reserve the 3 following locations for the 3 rows of the matrix mat3
        glEnableVertexAttribArray(5); // Use location 5 for the first row of the rotation matrix
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
        glVertexAttribDivisor(5, 1);

        glEnableVertexAttribArray(6); // Use location 6 for the second row of the rotation matrix
        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
        glVertexAttribDivisor(6, 1);

        glEnableVertexAttribArray(7); // Use location 7 for the third row of the rotation matrix
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
        glVertexAttribDivisor(7, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer (bind to 0 again)

        // Draw call
        // ********************************** //
        // glDrawElements(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr);
        glDrawElementsInstanced(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr, GLsizei(n_instances));

        opengl_check;

        // Clean state
        // ********************************** //
        glBindVertexArray(0);
        drawable.texture.unbind();
        glUseProgram(0);
    }

}