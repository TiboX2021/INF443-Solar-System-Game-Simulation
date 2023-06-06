#pragma once

#include "background/galaxy.hpp"
#include "celestial_bodies/asteroid_belt/asteroid_belt.hpp"
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
    void addAsteroidBelt(AsteroidBelt asteroid_belt);
    void initialize();

    // Draw Functions
    void drawObjects(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true);
    void drawBillboards(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true);

    // Simulation Functions
    virtual void simulateStep(float time_step);

    // Default : 1 day / second, with 60 fps. For slider use
    float time_step_multiplier = 24.0f * 3600; // Accélération x100
    // Public static generators
    static void generateSolarSystem(SimulationHandler &handler);

    // Get physics object (for camera intersection detection)
    std::vector<Object *> getPhysicalObjects() const;

protected:
    // Drawable objects
    // Store all drawable instances here
    std::vector<std::unique_ptr<BaseDrawable>> drawables;

    // Store pointers (references) here for efficient access. The deletion is already handled by the unique ptrs
    std::vector<Drawable *> drawable_objects;
    std::vector<BillboardDrawable *> billboard_drawable_objects;
    std::vector<Object *> physical_objects;

    std::vector<AsteroidBelt> asteroid_belts; // Asteroid belts

    Galaxy galaxy;
};