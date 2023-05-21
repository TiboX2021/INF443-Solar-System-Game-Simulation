#include "billboard.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"

void Billboard::initialize()
{
    // Create mesh and mesh drawable

    billboard_mesh = cgp::mesh_primitive_quadrangle({-radius, -radius, 0}, {radius, -radius, 0}, {radius, radius, 0}, {-radius, radius, 0});
    billboard_mesh_drawable.initialize_data_on_gpu(billboard_mesh);

    // Set texture
    billboard_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + texture_path);

    // Set billboard parameters
    billboard_mesh_drawable.material.texture_settings.two_sided = true;
    billboard_mesh_drawable.material.phong.specular = 0;
}

void Billboard::drawBillboards(const environment_structure &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Set billboard orientation to the camera
    billboard_mesh_drawable.model.rotation = camera.camera_model.orientation();

    // Draw the billboard
    cgp::draw(billboard_mesh_drawable, environment);
}