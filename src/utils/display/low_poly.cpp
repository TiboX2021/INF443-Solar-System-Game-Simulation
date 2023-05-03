#include "low_poly.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "utils/display/drawable.hpp"

LowPolyDrawable::LowPolyDrawable(double low_poly_radius)
{
    this->low_poly_radius = low_poly_radius;
}

void LowPolyDrawable::initialize()
{
    Drawable::initialize(); // Call base class initialize function

    // Initialize low poly mesh
    low_poly_mesh = cgp::mesh_primitive_disc(low_poly_radius, {0, 0, 0}, {0, 0, 1}, LOW_POLY_RESOLUTION);
    low_poly_drawable.initialize_data_on_gpu(low_poly_mesh);
    low_poly_drawable.material.phong.specular = 0; // No reflection for the low poly display
    low_poly_drawable.material.color = low_poly_color;
}

void LowPolyDrawable::setLowPolyColor(cgp::vec3 color)
{
    low_poly_color = color;                            // In case called before initialization
    low_poly_drawable.material.color = low_poly_color; // In case called after iniitialization
}

// Main draw function
void LowPolyDrawable::draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Check distance in comparison to radius
    if (shouldDrawLowPoly(camera))
    {
        // Draw low poly
        draw_low_poly(environment, camera, show_wireframe);
    }
    else
    {
        // Draw real object
        draw_real(environment, camera, show_wireframe);
    }
}

void LowPolyDrawable::draw_low_poly(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Set disk orientation facing camera
    low_poly_drawable.model.rotation = camera.camera_model.orientation();

    // Draw low poly
    cgp::draw(low_poly_drawable, environment);
}

void LowPolyDrawable::setPosition(cgp::vec3 position)
{
    low_poly_drawable.model.translation = position;
}

cgp::vec3 LowPolyDrawable::getPosition() const
{
    return low_poly_drawable.model.translation;
}
bool LowPolyDrawable::shouldDrawLowPoly(camera_controller_orbit_euler const &camera) const
{
    double distance = cgp::norm(camera.camera_model.position() - getPosition());

    return distance > LOW_POLY_DISTANCE_RATIO * low_poly_radius;
}