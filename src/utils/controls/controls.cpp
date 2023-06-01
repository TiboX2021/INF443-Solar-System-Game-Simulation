#include "controls.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "utils/camera/custom_camera_controller.hpp"
#include "utils/controls/control_constants.hpp"
#include "utils/controls/player_object.hpp"
#include "utils/shaders/shader_loader.hpp"
#include <iostream>

bool isHeldOrPressed(int key)
{
    return key == KEY_PRESSED || key == KEY_REPEAT;
}

// Handle player key presses
void Controls::handlePlayerKeys()
{
    // Scan key states
    for (auto &key_state : key_states)
    {
        // If the key is pressed or held
        if (isHeldOrPressed(key_state.second))
        {
            // Handle actions
            switch (key_state.first)
            {
            case KEY_ARROW_UP:
                // Pitch up
                player.moveUp();
                break;
            case KEY_ARROW_LEFT:
                // Turn left
                player.moveLeft();
                break;
            case KEY_ARROW_RIGHT:
                // Turn right
                player.moveRight();
                break;
            case KEY_ARROW_DOWN:
                // Pitch down
                player.moveDown();
                break;
            case KEY_SPACE:
                // Move forward
                player.moveForward();
                break;
            case KEY_Q:
                player.rollLeft();
                break;
            case KEY_S:
                player.rollRight();
                break;
            default:
                break;
            }
        }
    }

    // If space bar is not pressed, slow down
    if (key_states[KEY_SPACE] == KEY_RELEASED)
    {
        player.brake();
    }

    // If roll keys are not pressed, decelerate the roll
    if (key_states[KEY_Q] == KEY_RELEASED && key_states[KEY_S] == KEY_RELEASED)
    {
        player.decelerateRoll();
    }

    // Same for vertical and horizontal rotations
    if (key_states[KEY_ARROW_UP] == KEY_RELEASED && key_states[KEY_ARROW_DOWN] == KEY_RELEASED)
    {
        player.decelerateVerticalRotation();
    }

    if (key_states[KEY_ARROW_LEFT] == KEY_RELEASED && key_states[KEY_ARROW_RIGHT] == KEY_RELEASED)
    {
        player.decelerateHorizontalRotation();
    }
}

void Controls::updateCamera(custom_camera_controller &camera, cgp::mat4 &camera_matrix_view)
{
    // Update camera orientation according to the player
    player.updatePlayerCamera(camera.camera_model, camera_clip_objects);
    camera.idle_frame(camera_matrix_view);
}

void Controls::updatePlayer()
{
    player.step(camera_clip_objects);
}

Navion &Controls::getPlayerShip()
{
    return navion;
}

void Controls::updateShip()
{
    player.updatePlayerShip(navion);
}

void Controls::initialize_shield_mesh()
{

    // TODO : initialize. Use custom shader and all. Use a shader that can change colors ? Pass a color to the fragment shader, it applies it...
    cgp::mesh shield_mesh = cgp::mesh_primitive_sphere(); // Radius = 1 : default for player
    shield_mesh_drawable.initialize_data_on_gpu(shield_mesh);

    // Custom shield shader
    shield_mesh_drawable.shader = ShaderLoader::getShader("shield");

    shield_ubo.initialize(); // Instanciate UBO space on the GPU
}

void Controls::draw_shield(environment_structure const &environment)
{
    // Update mesh position
    shield_mesh_drawable.model.translation = Object::scaleDownDistanceForDisplay(player.get_position());

    // Draw the mesh and send custom array data via UBO
    // cgp::draw(shield_mesh_drawable, environment);
    shield_ubo.draw(shield_mesh_drawable, environment, global_player_collision_animation_buffer.toFloatBuffer());
}
