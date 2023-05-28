#include "custom_camera_controller.hpp"

void custom_camera_controller::update(cgp::mat4 &camera_matrix_view)
{
    camera_matrix_view = camera_model.matrix_view();
}

// Does nothing : this function is replaced by the control handler class
void custom_camera_controller::action_mouse_move(cgp::mat4 &camera_matrix_view)
{
    // Preconditions
    assert_cgp_no_msg(inputs != nullptr);
    assert_cgp_no_msg(window != nullptr);
    if (!is_active) return;

    update(camera_matrix_view);
}

void custom_camera_controller::action_keyboard(cgp::mat4 &)
{
    if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_C) && inputs->keyboard.shift)
    {
        is_cursor_trapped = !is_cursor_trapped;
        if (is_cursor_trapped)
            glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    // Escape also gives back the cursor
    if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE))
    {
        is_cursor_trapped = false;
        glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void custom_camera_controller::idle_frame(cgp::mat4 &camera_matrix_view)
{
    // Preconditions
    assert_cgp_no_msg(inputs != nullptr);
    assert_cgp_no_msg(window != nullptr);
    if (!is_active) return;

    // Just update the camera model, no controls handling
    update(camera_matrix_view);
}