#include "simulation_handler.hpp"

void SimulationHandler::addObject(BaseDrawable &drawable)
{
    drawables.push_back(drawable);

    // Add reference to the object in the corresponding vectors
    if (dynamic_cast<Drawable *>(&drawable))
    {
        drawable_objects.push_back(dynamic_cast<Drawable *>(&drawable));
    }
    if (dynamic_cast<BillboardDrawable *>(&drawable))
    {
        billboard_drawable_objects.push_back(dynamic_cast<BillboardDrawable *>(&drawable));
    }
    if (dynamic_cast<Object *>(&drawable))
    {
        physical_objects.push_back(dynamic_cast<Object *>(&drawable));
    }
}

void SimulationHandler::drawObjects(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    for (auto &drawable : drawable_objects)
    {
        drawable->draw(environment, camera, show_wireframe);
    }
}

void SimulationHandler::drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    for (auto &drawable : billboard_drawable_objects)
    {
        drawable->drawBillboards(environment, camera, show_wireframe);
    }
}

void SimulationHandler::simulateStep()
{
    // Clear forces
    for (auto &object : physical_objects)
    {
        object->resetForces();
    }

    // Iterate through all pairs of objects to compute forces
    for (auto it = physical_objects.begin(); it != physical_objects.end(); ++it)
    {
        for (auto it2 = it + 1; it2 != physical_objects.end(); ++it2)
        {
            // Compute forces between both objects
            (*it)->computeGravitationnalForce(*it2);
            (*it2)->computeGravitationnalForce(*it);
        }
    }

    // Update objects
    for (auto &object : physical_objects)
    {
        object->update(time_step);
        object->updateModels();
    }
}