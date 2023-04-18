#pragma once

#include "cgp/cgp.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "environment.hpp"
#include "terrain.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

// Variables associated to the GUI
struct gui_parameters
{
    bool display_frame = false;
    bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic
{

    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //
    camera_controller_orbit_euler camera_control;
    camera_projection_perspective camera_projection;
    window_structure window;

    mesh_drawable global_frame;        // The standard global frame
    environment_structure environment; // Standard environment controler
    input_devices inputs;              // Storage for inputs status (mouse, keyboard, window dimension)
    gui_parameters gui;                // Standard GUI element storage

    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //

    cgp::mesh_drawable terrain;
    cgp::mesh terrain_mesh;
    // cgp::mesh_drawable test_tree;
    // cgp::mesh_drawable test_cone;
    cgp::mesh_drawable tree;
    cgp::mesh_drawable mushroom;

    cgp::mesh_drawable grass; // Grass billboard that will be repeatedly displayed

    // Positions des arbres
    std::vector<cgp::vec3> tree_positions;
    std::vector<cgp::vec3> mushroom_positions;
    std::vector<cgp::vec3> grass_positions;

    perlin_noise_parameters parameters;

    // ****************************** //
    // Functions
    // ****************************** //

    void initialize();    // Standard initialization to be called before the animation loop
    void display_frame(); // The frame display to be called within the animation loop
    void display_gui();   // The display of the GUI, also called within the animation loop

    void mouse_move_event();
    void mouse_click_event();
    void keyboard_event();
    void idle_frame();

    // Display function for semi-transparent shapes
    void display_semiTransparent();
};