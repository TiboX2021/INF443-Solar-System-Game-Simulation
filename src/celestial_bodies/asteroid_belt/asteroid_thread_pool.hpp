#pragma once
#include "utils/physics/object.hpp"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr int ASTEROIDS_PER_THREAD = 10000; // TODO : 10 000 would be more reasonable

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

    void start()
    {
        if (isRunning) return;

        // Threads are not running yet

        threads.clear();
        isRunning = true;

        // Launch threads
        // TODO : use ASTEROIDS_PER_THREAD to determine the number of threads instead
        for (int i = 0; i < n_threads; i++)
        {
            threads.push_back(std::thread(&AsteroidThreadPool::worker, this, i, i + 1));
        }
    }
    void stop()
    {
        isRunning = false;

        // Wait for all threads to end
        for (auto &thread : threads)
        {
            thread.join();
        }
    }

    void worker(int start_index, int end_index)
    {
        // TODO : compute things.
        std::cout << "Starting thread with indexes " << start_index << " to " << end_index << std::endl;

        // TODO : compute loop
        while (isRunning)
        {
            // TODO : compute next positions
            // Artificial sleep time
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Ending thread with indexes " << start_index << " to " << end_index << std::endl;
    }

    // Update the camera position in an atomic variable in order for all the threads to be able to access it safely
    void updateCameraPosition(cgp::vec3 camera_position)
    {
        std::lock_guard<std::mutex> lock(camera_mutex);
        this->camera_position = camera_position;
    }

    // Get data to send to the GPU
    std::vector<AsteroidGPUData> &getGPUData()
    {
        // Lock buffer swap mutex just in case. It will release it when the function returns
        std::lock_guard<std::mutex> lock(swap_buffer_mutex);
        return current_gpu_data;
    }

    // TODO : when to call this function ? And how to sync the threads ? When their computation is finished, they must wait for the swap/read to unlock ?
    void swapBuffers()
    {
        std::lock_guard<std::mutex> lock(swap_buffer_mutex);
        std::copy(gpu_data_buffer.begin(), gpu_data_buffer.end(), current_gpu_data.begin());
    }

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
};