#include "simulation_handler/optimized_simulation_handler.hpp"

template <typename TExtendsBaseDrawable>
void OptimizedSimulationHandler::addObject(TExtendsBaseDrawable drawable, bool attractor)
{
    // Create unique_ptr
    std::unique_ptr<BaseDrawable> drawable_unique_ptr = std::make_unique<TExtendsBaseDrawable>(drawable);

    // Move the unique_ptr (its adress changes !)
    drawables.push_back(std::move(drawable_unique_ptr));

    // Get the memory adress from the vector pointer (while still keeping polymorphism !)
    BaseDrawable *ptr = drawables.back().get();

    // Add reference to the object in the corresponding vectors
    if (dynamic_cast<Drawable *>(ptr))
    {
        drawable_objects.push_back(dynamic_cast<Drawable *>(ptr));
    }
    if (dynamic_cast<BillboardDrawable *>(ptr))
    {
        billboard_drawable_objects.push_back(dynamic_cast<BillboardDrawable *>(ptr));
    }
    if (dynamic_cast<Object *>(ptr))
    {
        physical_objects.push_back(dynamic_cast<Object *>(ptr));

        // Add attractor
        if (attractor)
        {
            physical_attractors.push_back(dynamic_cast<Object *>(ptr));
        }
    }
}

void OptimizedSimulationHandler::simulateStep()
{
    // Clear forces
    for (auto &object : physical_objects)
    {
        object->resetForces();
    }

    // Iterate through all pairs of objects to compute forces
    for (auto it = physical_objects.begin(); it != physical_objects.end(); ++it)
    {
        for (auto it2 = it + 1; it2 != physical_attractors.end(); ++it2)
        {
            // Compute forces between both objects
            (*it)->computeGravitationnalForce(*it2);
        }
    }

    // Update objects
    for (auto &object : physical_objects)
    {
        object->update(time_step);
        object->updateModels();
    }
}

void OptimizedSimulationHandler::generateAsteroidField(OptimizedSimulationHandler &handler)
{

    // TODO : add planet saturn without Rings, and some asteroids that gravitate around (big size, rotation, etc)
    // TODO : util functions for generating this
}