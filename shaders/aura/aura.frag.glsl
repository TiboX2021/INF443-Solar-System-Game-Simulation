#version 330 core

// Aura fragment shader - this code is executed for every pixel/fragment that belongs to a displayed shape
//
// Compute the color with an alpha value based on the angle between the fragment's normal and the camera direction
// It gives the impression of an atmosphere around the planet (Aura object that englobes a planet instance)
//

// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
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
    // Alpha max constant, TODO pass it as arg?
    float max_alpha = 0.5;

    // -------------------------------------------- //
    //         Preliminary calculations             //
    // -------------------------------------------- //

    // Fragment normal
    vec3 N = normalize(fragment.normal);

    // Fragment to camera angle
    mat3 O = transpose(mat3(view));                        // get the orientation matrix
    vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
    vec3 camera_position = -O * last_col;                  // get the camera position
    vec3 V = normalize(camera_position - fragment.position);

    // Unit direction toward the light
    vec3 L = normalize(light - fragment.position);

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

    // -------------------------------------------- //
    //               Aura calculations              //
    // -------------------------------------------- //

    // Compute the alpha coefficient
    float alpha = min(abs(dot(N, V)), max_alpha);

    // Compute diffusion lighting coefficient (Phong model)
    float diffuse_component = abs(dot(N, L));
    float Ka = material.phong.ambient;
    float Kd = material.phong.diffuse;

    FragColor = vec4((Ka + Kd * diffuse_component) * color_object, alpha);
}