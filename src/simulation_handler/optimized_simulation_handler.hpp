#pragma once

#include "simulation_handler.hpp"

// Simulation handler, but not all objects do attract
class OptimizedSimulationHandler : public SimulationHandler
{
public:
    template <typename TExtendsBaseDrawable>
    void addObject(TExtendsBaseDrawable drawable, bool attractor = false); // Do not use reference, as the object will be copied and moved to a unique_ptr

    virtual void simulateStep() override;

    // Public static generators
    static void generateAsteroidField(OptimizedSimulationHandler &handler);

protected:
    std::vector<Object *> physical_attractors;
};