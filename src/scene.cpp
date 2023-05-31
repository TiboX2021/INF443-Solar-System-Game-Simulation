#include "scene.hpp"

#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "simulation_handler/simulation_handler.hpp"
#include "third_party/src/imgui/imgui.h"
#include "utils/controls/gui_params.hpp"
#include "utils/physics/object.hpp"
#include "utils/shaders/shader_loader.hpp"
#include <GLFW/glfw3.h>
#include <bits/types/timer_t.h>
#include <cmath>
#include <iostream>
#include <math.h>
using namespace cgp;

void scene_structure::initialize()
{
    // Initialize custom camera. The default direction is {1, 0, 0}, and the default top is {0, 0, 1} (set in the header)
    custom_camera.initialize(inputs, window);

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
    ShaderLoader::addShader("shield", "shield/shield");

    ShaderLoader::initialise();

    // Initialize simulation handler
    SimulationHandler::generateSolarSystem(simulation_handler);
    simulation_handler.initialize();

    keyboard_control_handler.getPlayerShip().create_millennium_falcon(0.2); // Initialize player spaceship

    keyboard_control_handler.setCameraClipObjects(simulation_handler.getPhysicalObjects());

    keyboard_control_handler.initialize_shield_mesh();
}

void scene_structure::display_frame()
{

    // Update GUI params values
    global_gui_params.update_values();

    /*********************************************/
    /*              TIMER HANDLING               */
    /*********************************************/

    // Update timer (ALWAYS FIRST)
    float dt = timer.update(); // Update timer
    // IMPORTANT : regulate timer : the first frames are slow, and a time step too large can mess up the simulation orbit
    dt = std::min(dt, 1.0f / 30); // Max time step is that of 30 fps

    // Set global timer attributes
    Timer::dt = dt;
    Timer::time = timer.t;

    // Send timer time as uniform to the shader
    environment.uniform_generic.uniform_float["time"] = timer.t;

    /*********************************************/
    /*          INPUTS & PLAYER HANDLING         */
    /*********************************************/

    // Handle keyboard & other controls
    keyboard_control_handler.handlePlayerKeys();
    keyboard_control_handler.updatePlayer();
    keyboard_control_handler.updateShip();
    keyboard_control_handler.updateCamera(custom_camera, environment.camera_view);

    // Set the light to the sun position (center)
    environment.light = vec3{0, 0, 0}; // camera_control.camera_model.position();

    /*********************************************/
    /*            SIMULATION & DRAWING           */
    /*********************************************/

    simulation_handler.simulateStep(dt);

    // Get camera position and rotation to compute custom meshes for distant objects
    cgp::vec3 position = custom_camera.camera_model.position();
    cgp::rotation_transform rotation = custom_camera.camera_model.orientation();

    simulation_handler.drawObjects(environment, position, rotation, false);

    if (global_gui_params.display_ship_atomic)
        keyboard_control_handler.getPlayerShip().draw(environment);

    display_semiTransparent();
}

void scene_structure::display_gui()
{
    ImGui::Checkbox("Show ship", &global_gui_params.display_ship);
    ImGui::Checkbox("Enable shield", &global_gui_params.enable_shield);
    ImGui::SliderFloat("Camera distance", &global_gui_params.camera_distance, 1, 20);
}

void scene_structure::mouse_move_event()
{
    // Does nothing but update the camera matrix
    // Mouse events shall be handled with the control instance
    custom_camera.idle_frame(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
    // Same as mouse_move_event
    custom_camera.idle_frame(environment.camera_view);
}
void scene_structure::keyboard_event()
{
    // Keyboard control is handled by the Control class instance
    keyboard_control_handler.handleKeyEvent(custom_camera.inputs);
    custom_camera.idle_frame(environment.camera_view);
}
void scene_structure::idle_frame()
{
    // Update the camera model on idle frames
    custom_camera.idle_frame(environment.camera_view);
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

    cgp::vec3 position = custom_camera.camera_model.position();
    cgp::rotation_transform rotation = custom_camera.camera_model.orientation();

    simulation_handler.drawBillboards(environment, position, rotation, false);

    if (global_gui_params.enable_shield)
        keyboard_control_handler.draw_shield(environment);

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}