#pragma once

#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "environment.hpp"
#include "simulation_handler/optimized_simulation_handler.hpp"
#include "simulation_handler/simulation_handler.hpp"
#include "utils/controls/controls.hpp"

#include "navion/navion.hpp"
#include "navion/reacteur.hpp"

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
    float angle_aile_vaisseau;
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
    timer_basic timer;                 // Standard timer

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

<<<<<<< HEAD
    SimulationHandler simulation_handler;
    OptimizedSimulationHandler asteroid_field_handler;
=======

    //***********************************************
    // Pour le projet
    //**********************************************
    Navion un_vaisseau;

    mesh_drawable truc_test;

    // DEBUG : to delete later
    Planet planet;
    Planet sun;
    Galaxy galaxy;
    RingPlanet ring_planet;
>>>>>>> 6009f2c (creation de l'aile du falcon (problèmes de sahder à régler))
    SimulationHandler simulation_handler;
    OptimizedSimulationHandler asteroid_field_handler;
    Controls keyboard_control_handler;
};
