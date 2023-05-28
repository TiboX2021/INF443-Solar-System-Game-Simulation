#pragma once

#include "cgp/graphics/camera/camera_controller/camera_controller_generic_base/camera_controller_generic_base.hpp"
#include "utils/camera/custom_camera_model.hpp"

struct custom_camera_controller : cgp::camera_controller_generic_base
{
    custom_camera_model camera_model;

    // Copied from camera_controller_first_person_euler
    void action_mouse_move(cgp::mat4 &camera_matrix_view);

    void action_keyboard(cgp::mat4 &camera_matrix_view);

    void idle_frame(cgp::mat4 &camera_matrix_view);

    void update(cgp::mat4 &camera_matrix_view);

private:
    // true = cursor captured (/infinite motion), false = cursor free
    bool is_cursor_trapped = false;
};
