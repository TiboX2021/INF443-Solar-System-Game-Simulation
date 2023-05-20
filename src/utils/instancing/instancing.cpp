#include "instancing.hpp"

namespace cgp
{
    // TODO : create an updated shader accordingly & test it with the debug star
    // TODO : how to change the shader and send the correct vbos for instancing ?
    // DEBUG : voir la page suivante pour compléter mon code et faire en sorte que ça marche
    // https://learnopengl.com/Advanced-OpenGL/Instancing
    // Faire du array instancing. On pourra même y mettre des scale !

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

        // TODO : send positions lists as uniforms to the shader. What type ?
        // TODO : il faudra recalc les positions pour chaque vertex ?
        // opengl_uniform(drawable.shader, "instance_positions", positions, true);
        // opengl_uniform(drawable.shader, "instance_orientations", instance_orientations, true);

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
        // TODO : ici, on ajoute tous les trucs des différents machins (etc). C'est là que je rajoute mes trucs à moi
        glBindVertexArray(drawable.vao); // TODO : mettre les arrays de vao dedans ?
        opengl_check;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.ebo_connectivity.id);
        opengl_check;

        // ********************************** //
        //     Custom instancing OpenGL code  //
        // ********************************** //
        GLuint custom_vao = 0;
        glGenBuffers(1, &custom_vao);              // Generate buffer
        glBindBuffer(GL_ARRAY_BUFFER, custom_vao); // Bind buffer
        // translations : array de glm::vec2 (si on a un vector, get l'array) et donner le ptr en argument
        // DEBUG : expérience, on veut passer en arg des vec3 qui représentent la position.
        // TODO : passer directement des matrices ? On peut faire ça dans le shader sinon
        const int n_instances = positions.size();
        glBufferData(GL_ARRAY_BUFFER, sizeof(cgp::vec3) * n_instances, positions.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer (bind to 0 again)

        glEnableVertexAttribArray(4);                                                  // This positions array will be in shader layout position 4
        glBindBuffer(GL_ARRAY_BUFFER, custom_vao);                                     // Bind buffer
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // Buffer of position 4 is composed of 3 non normalized floats
        glBindBuffer(GL_ARRAY_BUFFER, 0);                                              // Unbind buffer (bind to 0 again)
        glVertexAttribDivisor(4, 1);                                                   // 1 instead of 0 : this position attribute will change for every INSTANCE, not every VERTEX

        // Draw call
        // ********************************** //
        // glDrawElements(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr);
        // TODO : glDrawelementsInstanced.
        // En gros, on passe en dernier paramètre le nombre de fois qu'on veut dessiner le truc (en l'occurence, le nombre de positions qu'on a suffit)
        // Ça veut dire qu'il faut bind les trucs corrects avant. Objectif : faire en sorte que ça display les 2 soleils, avec de l'instancing !
        glDrawElementsInstanced(draw_mode, GLsizei(drawable.ebo_connectivity.size * 3), GL_UNSIGNED_INT, nullptr, GLsizei(positions.size()));

        opengl_check;

        // Clean state
        // ********************************** //
        glBindVertexArray(0);
        drawable.texture.unbind();
        glUseProgram(0);
    }

}