#pragma once
#include "utils/physics/object.hpp"
#include "utils/threads/threads.hpp"
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr int ASTEROIDS_PER_THREAD = 40000;
const float ASTEROID_DISPLAY_RADIUS = Object::scaleRadiusForDisplay(58232e3 / 40);

// Data that is computed by the worker threads, and then directly passed on to the GPU using instancing
struct AsteroidGPUData
{
    cgp::vec3 position;
    cgp::mat3 rotation;
    int mesh_index;
    float scale;
};

// Data for an asteroid. Used for initialization, fed into the thread pools at start and then deleted
struct Asteroid
{
    Object object;
    int mesh_index;
    float scale = 1.0f;
    cgp::vec3 asteroid_offset;
};

struct DistanceMeshHandler
{
    int high_poly;
    int low_poly;
    int low_poly_disk;
};

// Asteroid configuration data (does never change !)
struct AsteroidConfigData
{
    float scale;
    int mesh_handler_index;
};

class AsteroidThreadPool
{
public:
    AsteroidThreadPool(const std::vector<DistanceMeshHandler> &distance_mesh_handlers) : isRunning(false), distance_mesh_handlers(distance_mesh_handlers)
    {
    }
    AsteroidThreadPool(const AsteroidThreadPool &other);

    // Setters to call during initialization
    void setAttractor(Object *attractor)
    {
        this->attractor = attractor; // Also initialize attractor position to avoid undefined behavior
        current_attractor_position = Object::scaleDownDistanceForDisplay(this->attractor.load()->getPhysicsPosition());
        last_attractor_position = current_attractor_position;
    };

    // Load asteroid data before launching the simulation
    void loadAsteroids(const std::vector<Asteroid> &asteroids);
    void setDistanceMeshHandlers(const std::vector<DistanceMeshHandler> &distance_mesh_handlers) { this->distance_mesh_handlers = distance_mesh_handlers; };
    void setOrbitFactor(float orbitFactor) { this->orbitFactor = orbitFactor; };
    void allocateBuffers()
    {
        gpu_data_buffer.resize(asteroids.size());
        current_gpu_data.resize(asteroids.size());
    };
    void setTimeStep(float time_step) { this->time_step = time_step; };

    // Base functions
    void start(); // Create threads, initialize thread sync and un threads
    void stop();  // Stop threads

    // Worker thread utility functions
    void simulateStepForIndexes(float step, int start, int end);
    void computeGPUDataForIndexes(int start, int end);

    void worker(int start_index, int end_index); // Worker thread function

    // Utility functions
    void updateCameraPosition(cgp::vec3 camera_position); // Update the camera position in an atomic variable in order for all the threads to be able to access it safely
    cgp::vec3 getCameraPosition();                        // Get the camera position in a thread safe way
    std::vector<AsteroidGPUData> &getGPUData();           // Get data to send to the GPU
    void swapBuffers();
    void awaitAndLaunchNextFrameComputation(); // Unlock the sync mutex to enable the next computation for all threads

private:
    // Atomic variables shared between threads
    std::atomic<bool> isRunning;
    std::atomic<float> orbitFactor;
    std::atomic<Object *> attractor;
    cgp::vec3 last_attractor_position; // Semi-realistic physics simulation : always center the asteroids on the attractor
    cgp::vec3 current_attractor_position;
    std::atomic<float> time_step;

    std::mutex camera_mutex; // Atomic is not defined for vec3
    cgp::vec3 camera_position;

    // Mutex for buffer swapping
    std::mutex swap_buffer_mutex;

    // Note : we do not need mutexes for the gpu_data_buffer, as each thread only writes to a specific section of it, so it is thread safe
    std::vector<AsteroidGPUData> gpu_data_buffer;
    std::vector<AsteroidGPUData> current_gpu_data;

    std::vector<Object> asteroids; // Asteroid physical objects
    std::vector<float> collision_timeout;
    std::vector<bool> deactivated_asteroids; // Keep track of deactivated asteroids to avoid unnecessary computations
    std::vector<cgp::vec3> asteroid_offsets; // Asteroid offsets for gravity computation (display a "fluffy" belt while all asteroids are in theory on the same circular orbit)

    // Configuration data for asteroids and meshes. They are initialized and then never changed (read only operations by worke threads)
    std::vector<DistanceMeshHandler> distance_mesh_handlers;
    std::vector<AsteroidConfigData> asteroid_config_data;

    // Threads
    std::vector<std::thread> threads;
    ThreadsSync sync_util;
};