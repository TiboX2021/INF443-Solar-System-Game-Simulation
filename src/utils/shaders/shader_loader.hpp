#pragma once

#include "cgp/graphics/opengl/shaders/shaders.hpp"
#include <string>

class ShaderLoader
{

public:
    // This is a "static" class : remove the constructor
    ShaderLoader() = delete;

    // Load shaders on GPU
    static void initialise();

    // Setter
    static void addShader(const std::string &name, const std::string &path);

    // Getter
    static cgp::opengl_shader_structure &getShader(const std::string &name);

private:
    // Map of shaders
    static std::map<std::string, cgp::opengl_shader_structure> shaders;

    // Shader paths before load
    static std::map<std::string, std::string> shader_paths;
};