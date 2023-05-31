#pragma once

#include <atomic>

// Class to manage global GUI params that can be accessed anywhere in a thread safe way
struct GUIParams
{
    GUIParams() : display_ship(true), enable_shield(true), camera_distance(10){};

    // Update function
    void update_values();

    // Gui values
    bool display_ship;
    bool enable_shield;
    float camera_distance;

    // Thread safe values
    std::atomic<bool> display_ship_atomic;
    std::atomic<bool> enable_shield_atomic;
    std::atomic<float> camera_distance_atomic;
};

extern GUIParams global_gui_params;
