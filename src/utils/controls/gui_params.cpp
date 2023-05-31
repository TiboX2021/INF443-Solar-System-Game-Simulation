#include "gui_params.hpp"

GUIParams global_gui_params;

void GUIParams::update_values()
{
    display_ship_atomic = display_ship;
    enable_shield_atomic = enable_shield;
    camera_distance_atomic = camera_distance;
}