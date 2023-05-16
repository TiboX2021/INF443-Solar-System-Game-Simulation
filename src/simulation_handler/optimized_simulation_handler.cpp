#include "simulation_handler/optimized_simulation_handler.hpp"
#include "celestial_bodies/planet/planet.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/constants.hpp"
#include "utils/random/random.hpp"
#include <iostream>

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
        for (auto it2 = physical_attractors.begin(); it2 != physical_attractors.end(); ++it2)
        {
            if (*it != *it2)
            {
                // Compute forces between both objects
                (*it)->computeGravitationnalForce(*it2);
            }
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
    // Add galaxy first (background)
    Galaxy galaxy;
    handler.addObject(galaxy);

    // Add central immobile Saturn
    Planet saturn(SATURN_MASS, SATURN_RADIUS, {0, 0, 0}, "assets/planets/saturn.jpg", NO_PERLIN_NOISE);
    saturn.setLowPolyColor({207.0f / 255, 171.0f / 255, 134.0f / 255});
    saturn.setInitialRotationSpeed(SATURN_ROTATION_SPEED / 100);
    // saturn.setRotationAxis(SATURN_ROTATION_AXIS);
    saturn.setShouldTranslate(false); // Immobile
    handler.addObject(saturn, true);  // Saturn is the only attractor

    // TODO : add random asteroids gravitating and rotating around
    const float DISTANCE = SATURN_RADIUS * 2000; // Orbit distance : 1 billion meters
    const float ASTEROID_MASS = 1e22;

    // Generic perlin noise parameters :
    const float perlin_scale = 0.1;
    const perlin_noise_parameters noise_params{
        0.45f * perlin_scale,
        1.5f * perlin_scale, // Influence of small frequencies
        6,                   // Level of detail
        3.0f * perlin_scale,
        0.4f * perlin_scale, // Global noise scale
    };

    // TODO : generate N random asteroids and add them
    const int N_ASTEROIDS = 20;

    // Planet asteroid1(ASTEROID_MASS, SATURN_RADIUS / 20, {DISTANCE, 0, 0}, "assets/asteroids/grey_asteroid.jpg", NO_PERLIN_NOISE);
    // asteroid1.setInitialRotationSpeed(SATURN_ROTATION_SPEED / 100);
    // // asteroid1.setRotationAxis({0, 1, 0});
    // asteroid1.setRotationAxis(random_normalized_axis());

    // asteroid1.setInitialVelocity({0, Object::computeOrbitalSpeed(SATURN_MASS, DISTANCE), 0});
    // handler.addObject(asteroid1);

    for (int i = 0; i < N_ASTEROIDS; i++)
    {
        const float random_distance = DISTANCE * random_float(0.8, 1.2);
        const cgp::vec3 random_position = random_orbit_position(random_distance);
        const float size = SATURN_RADIUS / 20 * random_float(0.6, 1.5);
        Planet asteroid(ASTEROID_MASS, size, random_position, "assets/asteroids/grey_asteroid.jpg", NO_PERLIN_NOISE);
        asteroid.setInitialRotationSpeed(SATURN_ROTATION_SPEED / 100 * random_float(0.4, 1.5));
        // asteroid.setRotationAxis({0, 1, 0});
        asteroid.setRotationAxis(random_normalized_axis());

        asteroid.setInitialVelocity(Object::computeOrbitalSpeedForPosition(SATURN_MASS, random_position));
        handler.addObject(asteroid);
    }
}