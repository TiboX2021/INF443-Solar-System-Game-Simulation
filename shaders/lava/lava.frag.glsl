#version 330 core

// Lava fragment shader :
// Display an almost uniform lighting that evolves with time
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
uniform float time;

// Noise function
float mod289(float x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 perm(vec4 x) { return mod289(((x * 34.0) + 1.0) * x); }

// 3D Noise : output between 0 and 1
float noise(vec3 p)
{
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

// Remap a value from [0, 1] to [min_value, max_value]
float remap(float value, float min_value, float max_value)
{
    return value * (max_value - min_value) + min_value;
}

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

    // Saturation noise in [1, 1 + noise_amplitude]. Gives the stars a "burning" lighting effect
    float noise_amplitude = 1.5;

    float noise_multiplier1 = remap(noise(fragment.position + vec3(0, time, time)), 1, 1 + noise_amplitude);
    float noise_multiplier2 = remap(noise(fragment.position + vec3(time, time, 0)), 1, 1 + noise_amplitude);

    // Sinuso"idal combination of 2 noises going in different directions, in order to make it feel more realistic and less procedural
    float f = 0.3;
    float noise_multiplier = cos(time * f) * cos(time * f) * noise_multiplier1 + sin(time * f) * sin(time * f) * noise_multiplier2;

    // Compute the base color of the object based on: vertex color, uniform color, and texture
    vec3 color_object = fragment.color * material.color * color_image_texture.rgb * noise_multiplier;

    // Output color, with the alpha component
    FragColor = vec4(color_object, material.alpha * color_image_texture.a);
}