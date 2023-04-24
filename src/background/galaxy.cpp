#include "galaxy.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "environment.hpp"

// TODO : no source lighting. Only uniform lighting (custom shader ?). Not a problem if we are inside ?

Galaxy::Galaxy()
{
    this->radius = 100;
}

Galaxy::Galaxy(double radius)
{
    this->radius = radius;
}

void Galaxy::initialize()
{

    // Initialize CGP elements
    galaxy_mesh = cgp::mesh_primitive_sphere(radius, position, 20, 10);
    galaxy_mesh_drawable.initialize_data_on_gpu(galaxy_mesh);

    // Add texture
    galaxy_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/galaxy/voie_lactee.jpg",
                                                                       GL_CLAMP_TO_EDGE,
                                                                       GL_CLAMP_TO_EDGE);
    galaxy_mesh_drawable.material.texture_settings.two_sided = true;
    galaxy_mesh_drawable.material.phong.specular = 0; // No reflection for the planet display
}

void Galaxy::draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Remarque : pour la profondeur, jouer sur scene.camera_projection.depth_max = 10_000.0f;

    // Set position to camera position
    setPosition(camera.camera_model.position());

    cgp::draw(galaxy_mesh_drawable, environment);

    if (show_wireframe)
        cgp::draw_wireframe(galaxy_mesh_drawable, environment);
    glClear(GL_DEPTH_BUFFER_BIT); // Reset depth buffer in order to draw every next object in front of the galaxy
}

void Galaxy::setPosition(cgp::vec3 position)
{
    galaxy_mesh_drawable.model.translation = position;
}