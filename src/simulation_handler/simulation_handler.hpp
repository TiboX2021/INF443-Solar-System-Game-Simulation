#pragma once

#include "background/galaxy.hpp"
#include "utils/display/base_drawable.hpp"
#include "utils/display/billboard_drawable.hpp"
#include "utils/display/drawable.hpp"
#include "utils/physics/object.hpp"
#include <memory>

class SimulationHandler
{
public:
    template <typename TExtendsBaseDrawable>
    void addObject(TExtendsBaseDrawable drawable); // Do not use reference, as the object will be copied and moved to a unique_ptr
    void initialize();

    // Draw Functions
    void drawObjects(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);
    void drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);

    // Simulation Functions
    virtual void simulateStep();

    // Default : 1 day / second, with 60 fps. For slider use
    float time_step = 24.0f * 3600 / 60; // Accélération x100
    // Public static generators
    static void generateSolarSystem(SimulationHandler &handler);

protected:
    // Drawable objects
    // Store all drawable instances here
    std::vector<std::unique_ptr<BaseDrawable>> drawables;

    // Store pointers (references) here for efficient access. The deletion is already handled by the unique ptrs
    std::vector<Drawable *> drawable_objects;
    std::vector<BillboardDrawable *> billboard_drawable_objects;
    std::vector<Object *> physical_objects;

    Galaxy galaxy;
};