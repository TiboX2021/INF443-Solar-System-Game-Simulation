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

constexpr int ASTEROIDS_PER_THREAD = 10000;

// BUG : attention aux fonctions qui lockent des mutex mais renvoient des références, il y a un risque (mais la copie serait aussi lente, à voir)

// TEST : a pool of Objects, that send positions and rotations array on each frame.

// Double buffer : one for the last data, one for the new data. Who choses to replace it ? What if the worker thread is late ?
// TODO : communication + initialize objects

// Data that is computed by the worker threads, and then directly passed on to the GPU using instancing
struct AsteroidGPUData
{
    cgp::vec3 position;
    cgp::mat3 rotation;
    int mesh_index;
};

struct DistanceMeshHandler
{
    int high_poly;
    int low_poly;
    int low_poly_disk;
};

class AsteroidThreadPool
{
public:
    AsteroidThreadPool(const std::vector<DistanceMeshHandler> &distance_mesh_handlers) : isRunning(false), n_threads(4), distance_mesh_handlers(distance_mesh_handlers)
    {
    }
    AsteroidThreadPool(const AsteroidThreadPool &other);

    // Setters to call during initialization
    void setAttractor(Object *attractor) { this->attractor = attractor; };
    void setAsteroids(std::vector<Object> &asteroids) { this->asteroids = asteroids; };
    void setDistanceMeshHandlers(const std::vector<DistanceMeshHandler> &distance_mesh_handlers) { this->distance_mesh_handlers = distance_mesh_handlers; };

    // Base functions
    void start(); // Create threads, initialize thread sync and un threads
    void stop();  // Stop threads

    void worker(int start_index, int end_index); // Worker thread function

    // Utility functions
    void updateCameraPosition(cgp::vec3 camera_position); // Update the camera position in an atomic variable in order for all the threads to be able to access it safely
    std::vector<AsteroidGPUData> &getGPUData();           // Get data to send to the GPU
    // TODO : when to call this function ? And how to sync the threads ? When their computation is finished, they must wait for the swap/read to unlock ?
    void swapBuffers();
    void awaitAndLaunchNextFrameComputation(); // Unlock the sync mutex to enable the next computation for all threads

private:
    // Atomic variables shared between threads
    std::atomic<bool> isRunning;
    std::atomic<Object *> attractor;

    std::mutex camera_mutex; // Atomic is not defined for vec3
    cgp::vec3 camera_position;

    // TODO : create the objects that must be passed to the shader (position, rotation and correct mesh index)

    // Mutex for buffer swapping
    std::mutex swap_buffer_mutex; // TODO : when should we swap buffer ?

    // Note : we do not need mutexes for the gpu_data_buffer, as each thread only writes to a specific section of it, so it is thread safe
    std::vector<AsteroidGPUData> gpu_data_buffer;  // TODO : use some sort of lock that would prevent reading this when threads are writing. Or swap it ?
    std::vector<AsteroidGPUData> current_gpu_data; // TODO : return this one, and swap it every frame ?

    int n_threads = 4; // Number of threads to be launched

    std::vector<Object> asteroids; // Asteroid physical objects

    // Mesh handlers data. It will only be read, so no need for mutexes or atomics
    std::vector<DistanceMeshHandler> distance_mesh_handlers;

    // Threads
    std::vector<std::thread> threads;
    ThreadsSync sync_util;
};