#include "shader_loader.hpp"
#include "environment.hpp"

// Initialize static variables
std::map<std::string, cgp::opengl_shader_structure> ShaderLoader::shaders;
std::map<std::string, std::string> ShaderLoader::shader_paths;

/**
Adds a shader to the shader map
*/
void ShaderLoader::addShader(const std::string &name, const std::string &path)
{
    shader_paths[name] = path;
    shaders[name] = cgp::opengl_shader_structure(); // Instanciate without loading yet
}

/**
Returns a shader from its name
*/
cgp::opengl_shader_structure &ShaderLoader::getShader(const std::string &name)
{
    return shaders[name];
}

/**
Initialize shaders and add them to the shader map
Throws an error if a shader is not found
*/
void ShaderLoader::initialise()
{
    // Load all shaders
    for (auto &shader : shader_paths)
    {
        shaders[shader.first].load(project::path + "shaders/" + shader.second + ".vert.glsl", project::path + "shaders/" + shader.second + ".frag.glsl");
    }
}