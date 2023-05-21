#include "scene.hpp"

#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "simulation_handler/optimized_simulation_handler.hpp"
#include "simulation_handler/simulation_handler.hpp"
#include "third_party/src/imgui/imgui.h"
#include "utils/shaders/shader_loader.hpp"
#include <cmath>
#include <iostream>
#include <math.h>
using namespace cgp;

void scene_structure::initialize()
{
    camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
    camera_control.set_rotation_axis_z();
    camera_control.look_at({15.0f, 6.0f, 6.0f}, {0, 0, 0});
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());

    // Change depth of field
    camera_projection.depth_max = 10000.0f; // Default : 1000.0f
    // BUG : à très longue distance, pour les objets qui ne disparaissent pas, bug d'affichage. Diminuer la scale ?
    // Il faut réduire l'échelle globale du projet, cf object.hpp

    // Load shaders
    ShaderLoader::addShader("custom", "custom_shaders/custom");
    ShaderLoader::addShader("aura", "aura/aura");
    ShaderLoader::addShader("bumpy", "bumpy/bumpy");
    ShaderLoader::addShader("uniform", "uniform/uniform");
    ShaderLoader::addShader("lava", "lava/lava");
    ShaderLoader::addShader("instanced", "instanced/instanced");

    ShaderLoader::initialise();

    // Initialize simulation handler
    // SimulationHandler::generateSolarSystem(simulation_handler);
    // simulation_handler.initialize();

    // Initialise asteroid field simulation handler
    OptimizedSimulationHandler::generateAsteroidField(asteroid_field_handler);
    asteroid_field_handler.initialize();
}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = vec3{1000, 0, 0}; // camera_control.camera_model.position();

    // simulation_handler.simulateStep();
    // simulation_handler.drawObjects(environment, camera_control, false);

    asteroid_field_handler.simulateStep();
    asteroid_field_handler.drawObjects(environment, camera_control, false);

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

    // simulation_handler.drawBillboards(environment, camera_control, false);
    asteroid_field_handler.drawBillboards(environment, camera_control, false);

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}