#include "scene.hpp"

#include "background/galaxy.hpp"
#include "celestial_bodies/planet/planet.hpp"
#include "celestial_bodies/ring_planet/ring_planet.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "third_party/src/imgui/imgui.h"
#include "utils/physics/object.hpp"
#include <iostream>
using namespace cgp;

void scene_structure::initialize()
{
    camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
    camera_control.set_rotation_axis_z();
    camera_control.look_at({15.0f, 6.0f, 6.0f}, {0, 0, 0});
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());

    // Initialize planet
    planet.initialize();
    ring_planet.initialize();

    // Initialize background galaxy
    galaxy.initialize();

    double sun_mass = 1988500e24; // In kg
    double sun_radius = 696340e3; // In meters

    // Initialize sun
    sun = Planet(sun_mass, sun_radius, {0, 0, 0}, "assets/planets/sun.jpg");
    sun.initialize();

    // Distance terre soleil : 150.48 million km
    double terre_soleil = 150.48e9; // en m
    planet.setPosition({terre_soleil, 0, 0});
    planet.setLowPolyColor({32.0f / 255, 60.0f / 255, 74.0f / 255});
    planet.setInitialVelocity({0, Object::computeOrbitalSpeed(sun_mass, terre_soleil), 0});
    planet.setInitialRotationSpeed(2.0 * M_PI / (3600 * 24));

    // Initialize Saturn
    // Saturne soleil : 1.4652 billion km
    double saturne_soleil = 1.4652e12; // en m
    ring_planet.setPosition({saturne_soleil, 0, 0});
    ring_planet.setInitialVelocity({0, Object::computeOrbitalSpeed(sun_mass, saturne_soleil), 0});

    // Change depth of field
    camera_projection.depth_max = 10000.0f; // Default : 1000.0f
}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    // Update physics for planet
    planet.resetForces();
    planet.computeGravitationnalForce(&sun);
    planet.update(1.0 / 60.0 * Object::getTimeScale());
    planet.updateModels();

    ring_planet.resetForces();
    ring_planet.computeGravitationnalForce(&sun);
    ring_planet.update(1.0 / 60.0 * Object::getTimeScale());
    ring_planet.updateModels();

    // DEBUG : draw planet and galaxy
    galaxy.draw(environment, camera_control, gui.display_wireframe);
    planet.draw(environment, camera_control, gui.display_wireframe);
    ring_planet.draw(environment, camera_control, gui.display_wireframe);
    sun.draw(environment, camera_control, gui.display_wireframe);
    display_semiTransparent();
}

void scene_structure::display_gui()
{
    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
    if (!inputs.keyboard.shift)
        camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
    camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
    camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
    camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_semiTransparent()
{
    // Enable use of alpha component as color blending for transparent elements
    //  alpha = current_color.alpha
    //  new color = previous_color * alpha + current_color * (1-alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);

    // Draw ring planet billboard
    ring_planet.drawBillboards(environment, camera_control, gui.display_wireframe);

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}