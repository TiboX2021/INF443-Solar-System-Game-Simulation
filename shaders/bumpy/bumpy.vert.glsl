#version 330 core

// Vertex shader - this code is executed for every vertex of the shape

// Inputs coming from VBOs
layout(location = 0) in vec3 vertex_position; // vertex position in local space (x,y,z)
layout(location = 1) in vec3 vertex_normal;   // vertex normal in local space   (nx,ny,nz)
layout(location = 2) in vec3 vertex_color;    // vertex color      (r,g,b)
layout(location = 3) in vec2 vertex_uv;       // vertex uv-texture (u,v)

// Output variables sent to the fragment shader
out struct fragment_data
{
    vec3 position; // vertex position in world space
    vec3 normal;   // normal position in world space
    vec3 color;    // vertex color
    vec2 uv;       // vertex uv
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model;      // Model affine transform matrix associated to the current shape
uniform mat4 view;       // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera

uniform mat4 modelNormal; // Model without scaling used for the normal. modelNormal = transpose(inverse(model))

void main()
{

    // float theta = 3.1415 / 9;
    // // Matrice de rotation
    // mat4 R = transpose(
    //     mat4(cos(theta), -sin(theta), 0.0, 0.0,
    //          sin(theta), cos(theta), 0.0, 0.0,
    //          0.0, 0.0, 1.0, 0.0,
    //          0.0, 0.0, 0.0, 1.0));
    // TODO : ça pourrait être intéressant pour les étoiles

    // -------------------------------------------- //
    //         Compute the normal vector            //
    // -------------------------------------------- //

    // The normal of the vertex in the world space
    vec4 normal = modelNormal * vec4(vertex_normal, 0.0);
    normal.xyz = normalize(normal.xyz); // Re normalize just in case

    // -------------------------------------------- //
    //         Compute bump translation             //
    // -------------------------------------------- //

    float color_coeff = ((vertex_color[0] + vertex_color[1] + vertex_color[2]) - 2.999) / 1.5; // Between -1 and 1

    float translation_coeff = 10; // TODO : adjust this setting
    // BUG : tout est translaté vers l'extérieur, sans relief...

    float bump_translation = color_coeff * translation_coeff;

    // -------------------------------------------- //
    //       Compute the translation matrix         //
    // -------------------------------------------- //

    mat4 T = transpose(
        mat4(1.0, 0.0, 0.0, normal.x * bump_translation,
             0.0, 1.0, 0.0, normal.y * bump_translation,
             0.0, 0.0, 1.0, normal.z * bump_translation,
             0.0, 0.0, 0.0, 1.0));

    vec4 position = T * model * vec4(vertex_position, 1.0);
    // *****************************************************

    // The projected position of the vertex in the normalized device coordinates:
    vec4 position_projected = projection * view * position;

    // Fill the parameters sent to the fragment shader
    fragment.position = position.xyz;
    fragment.normal = normal.xyz;

    // DEBUG : couleur en fonction du signe de la translation
    if (color_coeff > 0)
    {
        fragment.color = vec3(1.0, 0.0, 0.0);
    }
    else
    {
        fragment.color = vec3(0.0, 0.0, 1.0);
    }

    // fragment.color = vertex_color;
    fragment.uv = vertex_uv;

    // gl_Position is a built-in variable which is the expected output of the vertex shader
    gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)

    // TODO : faire ça dans le fragment pour ne pas avoir à ajouter beaucoup de vertex ?
    // Il faut probablement le faire pour les fragments et pas les vertex...

    // Idée : faire varier les normales en fonction des gradients de couleur. On ne peut pas changer la forme directement comme ça,
    // mais on peut faire varier la manière d'éclairer la surface via la normale.
    // TODO: normal mapping instead (calculer les "gradients" de couleur sur la texture ?) cf vertex shader
}
