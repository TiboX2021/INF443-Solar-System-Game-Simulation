#version 330 core

// Vertex shader - this code is executed for every vertex of the shape

// Inputs coming from VBOs
layout(location = 0) in vec3 vertex_position;          // vertex position in local space (x,y,z)
layout(location = 1) in vec3 vertex_normal;            // vertex normal in local space   (nx,ny,nz)
layout(location = 2) in vec3 vertex_color;             // vertex color      (r,g,b)
layout(location = 3) in vec2 vertex_uv;                // vertex uv-texture (u,v)
layout(location = 4) in vec3 instanced_model_position; // instanced model position in world space (x,y,z) TODO translation
layout(location = 5) in mat3 instanced_model_rotation; // instanced model rotation
layout(location = 8) in float instanced_model_scale;   // instanced model scale

// Output variables sent to the fragment shader
out struct fragment_data
{
    vec3 position; // vertex position in world space
    vec3 normal;   // normal position in world space
    vec3 tangent;  // Tangent and bitangent for bump mapping
    vec3 bitangent;
    vec3 color; // vertex color
    vec2 uv;    // vertex uv
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model;      // Model affine transform matrix associated to the current shape
uniform mat4 view;       // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera

uniform mat4 modelNormal; // Model without scaling used for the normal. modelNormal = transpose(inverse(model))

uniform bool do_bump_mapping; // True if bump mapping is enabled

void main()
{
    // The position of the vertex in the world space
    vec4 position = model * vec4(vertex_position * instanced_model_scale, 1.0); // Scale the vertex position

    // Apply rotation before translation
    position = position * mat4(instanced_model_rotation);

    position.xyz += instanced_model_position; // Add instanced model position to the vertex position

    // The normal of the vertex in the world space
    vec4 normal = modelNormal * vec4(vertex_normal, 0.0) * mat4(instanced_model_rotation);

    // Compute tangent and bitangent
    vec3 tangent;
    vec3 bitangent;

    if (do_bump_mapping)
    {
        if (vertex_normal.y == 1.0)
        {
            tangent = vec3(1.0, 0.0, 0.0);
            bitangent = vec3(0.0, 0.0, 1.0);
        }
        else
        {
            tangent = normalize(cross(normal.xyz, vec3(0.0, 1.0, 0.0)));
            bitangent = normalize(cross(normal.xyz, tangent));
        }
    }

    // The projected position of the vertex in the normalized device coordinates:
    vec4 position_projected = projection * view * position;

    // Fill the parameters sent to the fragment shader
    fragment.position = position.xyz;
    fragment.normal = normal.xyz;
    fragment.color = vertex_color;
    fragment.uv = vertex_uv;

    // gl_Position is a built-in variable which is the expected output of the vertex shader
    gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)
}
