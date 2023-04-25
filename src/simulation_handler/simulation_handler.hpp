#pragma once

// TODO : class with arrays of drawables and arrays of objects, that handles everything
// TODO: faire une méthode statique qui génère le système solaire

#include "utils/display/base_drawable.hpp"
#include "utils/display/billboard_drawable.hpp"
#include "utils/display/drawable.hpp"
#include "utils/physics/object.hpp"

class SimulationHandler
{
public:
    void addObject(BaseDrawable &drawable);

    // Draw Functions
    void drawObjects(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);
    void drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true);

    // Simulation Functions
    void simulateStep();

    // For slider use
    float time_step = 0.1f;

private:
    // Drawable objects
    // Store all drawable instances here
    std::vector<BaseDrawable> drawables;

    // Store pointers (references) here for efficient access
    std::vector<Drawable *> drawable_objects;
    std::vector<BillboardDrawable *> billboard_drawable_objects;
    std::vector<Object *> physical_objects;
};