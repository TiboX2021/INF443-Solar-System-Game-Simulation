#include "scene.hpp"

#include "background/galaxy.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "planet/ring_planet.hpp"
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
    planet.setPosition({30, 30, 0});

    // Initialize background galaxy
    galaxy.initialize();

    // Change depth of field
    camera_projection.depth_max = 10000.0f; // Default : 1000.0f
}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    // DEBUG : draw planet and galaxy
    galaxy.draw(environment, camera_control, gui.display_wireframe);
    planet.draw(environment, camera_control, gui.display_wireframe);
    ring_planet.draw(environment, camera_control, gui.display_wireframe);
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
    ring_planet.draw_ring_billboard(environment, camera_control, gui.display_wireframe);

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}