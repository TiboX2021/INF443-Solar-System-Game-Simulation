#include "asteroid_thread_pool.hpp"

// Define the copy constructor
AsteroidThreadPool::AsteroidThreadPool(const AsteroidThreadPool &other)
{
    isRunning.store(other.isRunning.load());
    // camera_position.store(other.camera_position.load());
    attractor.store(other.attractor.load());

    // Copy the data
    asteroids = other.asteroids;
    distance_mesh_handlers = other.distance_mesh_handlers;
}