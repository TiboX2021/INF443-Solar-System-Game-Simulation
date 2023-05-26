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
    vec3 color;    // vertex color
    vec2 uv;       // vertex uv
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model;      // Model affine transform matrix associated to the current shape
uniform mat4 view;       // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera
uniform float time;      // Time in seconds since the start of the program

vec3 computePerpendicularVector(vec3 v)
{

    float threshold = 0.577; // Arbitrary
    vec3 result;

    if (abs(v.x) < threshold)
    {
        result = vec3(1, 0, 0);
    }
    else if (abs(v.y) < threshold)
    {
        result = vec3(0, 1, 0);
    }
    else if (abs(v.z) < threshold)
    {
        result = vec3(0, 0, 1);
    }
    else
    {
        result = vec3(1, 0, 0);
    }

    return normalize(cross(v, result));
}

void main()
{
    // The position of the vertex in the world space
    vec4 position = model * vec4(vertex_position, 1.0);

    float space_multiplier = 4;
    float amplitude = 0.05;

    // Create orthonormal base for the normal vector
    vec3 u = computePerpendicularVector(vertex_normal);
    vec3 v = normalize(cross(vertex_normal, u));

    // Create a sinusoidal rotation, etc
    vec3 offset = u * sin(time + position.x * space_multiplier) + v * cos(time + position.y * space_multiplier);

    // Add tangent offset to the vertex position
    position.xyz += offset * amplitude;

    // The projected position of the vertex in the normalized device coordinates:
    vec4 position_projected = projection * view * position;

    // Fill the parameters sent to the fragment shader
    fragment.position = position.xyz;
    fragment.color = vertex_color;
    fragment.uv = vertex_uv;

    // gl_Position is a built-in variable which is the expected output of the vertex shader
    gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)
}
