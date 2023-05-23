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

// Function to generate a random number between -1.0 and 1.0
float random(vec3 st)
{
    return fract(sin(dot(st, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

float mod289(float x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 perm(vec4 x) { return mod289(((x * 34.0) + 1.0) * x); }

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

void main()
{
    // Idée : combiner 2 mouvements oscillatoires, pour couvrir la régularité
    // TODO : move the vertices perpendicular to the normal direction in order to create a fake animation
    // TODO : pass the width of the star as a vbo argument in order to have correct amplitude ?
    // TODO : j'aimerais faire une sorte de bruit de perlin sur la luminosité

    // Generate a random offset for each vertex based on its position
    vec3 randomOffset = vec3(random(vertex_position + time));

    float speed = 1;
    float amplitude = 0.05;

    vec3 animationOffset = vec3(
        sin(time * speed + randomOffset.x) * amplitude,
        cos(time * speed + randomOffset.y) * amplitude,
        sin(time * speed + randomOffset.z) * amplitude);

    // The position of the vertex in the world space
    vec4 position = model * vec4(vertex_position, 1.0);

    float space_multiplier = 4;

    // TODO : find the best vector to create the movement that I want for this
    // TODO : create a similar thing for the galaxy fragment shader ?
    vec3 circularOffset = vec3(sin(time + position.y * space_multiplier), cos(time + position.x * space_multiplier), sin(time + position.z * space_multiplier)) * amplitude;

    // Faire en sorte que chaque point ait une variation circulaire autour de son centre
    // XXX : c'est ici qu'on ajoute l'offset
    // position.xyz += normalize(cross(circularOffset, vertex_normal)) * amplitude;

    // The projected position of the vertex in the normalized device coordinates:
    vec4 position_projected = projection * view * position;

    // TODO : calcul d'unmultiplicateur de luminosité via un bruit de perlin

    // Fill the parameters sent to the fragment shader
    fragment.position = position.xyz;
    fragment.color = vertex_color; // * noise(position.xyz);
    fragment.uv = vertex_uv;

    // gl_Position is a built-in variable which is the expected output of the vertex shader
    gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)
}
