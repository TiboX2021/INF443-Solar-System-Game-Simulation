#version 330 core

// Lava fragment shader :
// Display a texture that is slightly moving
//

// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

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

void main()
{
    // *************************************** //
    // Texture
    // *************************************** //

    // Current uv coordinates
    vec2 uv_image = vec2(fragment.uv.x, fragment.uv.y);
    if (material.texture_settings.texture_inverse_v)
    {
        uv_image.y = 1.0 - uv_image.y;
    }

    // Get the current texture color
    vec4 color_image_texture = texture(image_texture, uv_image);
    if (material.texture_settings.use_texture == false)
    {
        color_image_texture = vec4(1.0, 1.0, 1.0, 1.0);
    }

    // Compute the base color of the object based on: vertex color, uniform color, and texture
    vec3 color_object = fragment.color * material.color * color_image_texture.rgb;

    // Output color, with the alpha component
    FragColor = vec4(color_object, material.alpha * color_image_texture.a);
}