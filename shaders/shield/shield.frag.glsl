#version 330 core

#define PI 3.1415926538
// Fragment shader - this code is executed for every pixel/fragment that belongs to a displayed shape
//
// Compute the color using Phong illumination (ambient, diffuse, specular)
//  There is 3 possible input colors:
//    - fragment_data.color: the per-vertex color defined in the mesh
//    - material.color: the uniform color (constant for the whole shape)
//    - image_texture: color coming from the texture image
//  The color considered is the product of: fragment_data.color x material.color x image_texture
//  The alpha (/transparent) channel is obtained as the product of: material.alpha x image_texture.a
//

// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 local_position; // Vertex local normalized position in relation to the model center
    vec3 position;       // position in the world space
    vec3 normal;         // normal in the world space
    vec3 color;          // current color on the fragment
    vec2 uv;             // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location = 0) out vec4 FragColor;

// Uniform values that must be send from the C++ code
// ***************************************************** //

uniform sampler2D image_texture; // Texture image identifiant

uniform mat4 view; // View matrix (rigid transform) of the camera - to compute the camera position

uniform vec3 light; // position of the light

// Coefficients of phong illumination model
struct phong_structure
{
    float ambient;
    float diffuse;
    float specular;
    float specular_exponent;
};

// Settings for texture display
struct texture_settings_structure
{
    bool use_texture;       // Switch the use of texture on/off
    bool texture_inverse_v; // Reverse the texture in the v component (1-v)
    bool two_sided;         // Display a two-sided illuminated surface (doesn't work on Mac)
};

// Material of the mesh (using a Phong model)
struct material_structure
{
    vec3 color;  // Uniform color of the object
    float alpha; // alpha coefficient

    phong_structure phong;                       // Phong coefficients
    texture_settings_structure texture_settings; // Additional settings for the texture
};

uniform material_structure material;

layout(std140) uniform collision_points
{
    vec4 data[10]; // Max array size (max collisions handled). Adapt this both here and in the c++ code !
    int size;
};

uniform float animation_time;
uniform float time;
uniform vec3 ship_direction;

// Smooth gaussian transition between standby and collision shield
float gaussian(float x, float sigma)
{
    return exp(-x * x / (2 * sigma * sigma));
}

void main()
{
    // Colors
    vec4 standby_color = vec4(0.5, 0.5, 1, 0.2);
    vec4 animation_standby_color = vec4(0.6, 0.6, 1, 0.4);
    vec4 collision_color = vec4(1, 0.5, 0.5, 1);

    // Compute standby animation color
    float angle = acos(dot(fragment.local_position, ship_direction)); // in [0, PI]
    float space_freq = 4;                                             // Number of waves visible during the animation
    float time_freq = 4;
    float standby_coeff = max(0, PI / 2 - angle) * pow(sin(angle * space_freq - time * time_freq), 2);

    standby_color = (1 - standby_coeff) * standby_color + standby_coeff * animation_standby_color;

    // Constants
    float collision_coeff = 0;    // "Shockwave" coefficient (0 = standby, 1 = collision) for this pixel
    float offset_angle = PI / 10; // Width of the shockwave

    // Compute the maximum shockwave coefficient from all collisions in the UBO data
    for (int i = 0; i < size; i++)
    {
        vec4 one_data = data[i];
        vec3 collision_direction = one_data.xyz;
        float current_animation_time = one_data.w;

        // Compute angle between the collision direction and the fragment position
        float angle = acos(dot(fragment.local_position, collision_direction)); // in [0, PI]

        // Compute the angle, and offset it by the collision direction
        float new_coeff = gaussian(angle - current_animation_time / animation_time * PI / 2, offset_angle) * (1 - current_animation_time / animation_time);

        if (new_coeff > collision_coeff)
        {
            collision_coeff = new_coeff;
        }
    }

    // Mix standby with collision color
    FragColor = (1 - collision_coeff) * standby_color + collision_coeff * collision_color;
}