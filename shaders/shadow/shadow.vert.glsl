// TODO : simple vertex shader for shadow mapping ?
#version 330 core
layout(location = 0) in vec3 vertex_position;

uniform mat4 model;      // Model affine transform matrix associated to the current shape
uniform mat4 view;       // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera

void main()
{
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}