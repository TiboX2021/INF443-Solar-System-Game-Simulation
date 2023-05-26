#include "asteroid_thread_pool.hpp"
#include "cgp/core/array/numarray_stack/implementation/numarray_stack.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "utils/physics/object.hpp"
#include <cmath>
#include <iostream>

// Define the copy constructor
AsteroidThreadPool::AsteroidThreadPool(const AsteroidThreadPool &other)
{
    isRunning.store(other.isRunning.load());
    // camera_position.store(other.camera_position.load());
    attractor.store(other.attractor.load());
    orbitFactor.store(other.orbitFactor.load());

    // Copy the data
    asteroids = other.asteroids;
    distance_mesh_handlers = other.distance_mesh_handlers;
}

// Instanciate threads, initialize thread sync and launch threads
void AsteroidThreadPool::start()
{
    if (isRunning) return;

    // Threads are not running yet
    threads.clear();
    isRunning = true;

    // Compute number of threads to use according to the number of asteroids to simulate
    int n_threads = std::ceil((float)asteroids.size() / ASTEROIDS_PER_THREAD);

    // Initialize thread sync
    sync_util.setThreadCount(n_threads);

    // Launch threads
    for (int i = 0; i < n_threads; i++)
    {
        threads.push_back(std::thread(&AsteroidThreadPool::worker, this, i * ASTEROIDS_PER_THREAD, std::min(((i + 1) * ASTEROIDS_PER_THREAD), (int)asteroids.size())));
    }

    if (threads.size() > 0)
        std::cout << "Started " << threads.size() << " simulation worker threads" << std::endl;
}

// Stop all threads gracefully
void AsteroidThreadPool::stop()
{
    isRunning = false;

    // Forcefully start/unlock threads in order to avoid deadlock
    sync_util.start();

    // Wait for all threads to end
    for (auto &thread : threads)
    {
        thread.join();
    }

    if (threads.size() > 0)
        std::cout << "Gracefully stopped " << threads.size() << " worker threads" << std::endl;
}

void AsteroidThreadPool::swapBuffers()
{
    std::lock_guard<std::mutex> lock(swap_buffer_mutex);
    std::copy(gpu_data_buffer.begin(), gpu_data_buffer.end(), current_gpu_data.begin());
}

// Unlock the sync mutex to enable the next computation for all threads
void AsteroidThreadPool::awaitAndLaunchNextFrameComputation()
{
    sync_util.awaitAll(); // Wait for all threads to finish their computation

    // Updae the attractor position while the threads are waiting
    // WARNING : this is technically not thread safe, but it should be fine
    last_attractor_position = current_attractor_position;
    current_attractor_position = Object::scaleDownDistanceForDisplay(attractor.load()->getPhysicsPosition());

    sync_util.start(); // Restart the threads
}

// Get data to send to the GPU
std::vector<AsteroidGPUData> &AsteroidThreadPool::getGPUData()
{
    // Lock buffer swap mutex just in case. It will release it when the function returns
    std::lock_guard<std::mutex> lock(swap_buffer_mutex);
    return current_gpu_data;
}

// Update the camera position in an atomic variable in order for all the threads to be able to access it safely
void AsteroidThreadPool::updateCameraPosition(cgp::vec3 camera_position)
{
    std::lock_guard<std::mutex> lock(camera_mutex);
    this->camera_position = camera_position;
}

cgp::vec3 AsteroidThreadPool::getCameraPosition()
{
    std::lock_guard<std::mutex> lock(camera_mutex);
    return camera_position;
}

// Worker thread function
void AsteroidThreadPool::worker(int start_index, int end_index)
{
    while (isRunning)
    {
        // Update physics positions
        simulateStepForIndexes(Timer::dt * 24.0f * 3600, start_index, end_index);

        // Compute & add the mesh index data to the buffers to be sent to the GPU
        computeGPUDataForIndexes(start_index, end_index);

        // Wait for the launch signal to be given for the next iteration.
        sync_util.markDone();
        sync_util.awaitRestart();
    }
}

// Simulate a step for asteroids ranging from start to end indexes.
// Helper for the worker thread function
void AsteroidThreadPool::simulateStepForIndexes(float step, int start, int end)
{
    cgp::vec3 delta_attractor_position = (current_attractor_position - last_attractor_position) / PHYSICS_SCALE;

    // Clear forces + update positions to match the main attractor
    for (int i = start; i < end; i++)
    {
        asteroids[i].resetForces();
        asteroids[i].setPhysicsPosition(asteroids[i].getPhysicsPosition() + delta_attractor_position);
    }

    // Compute gravitationnal force to the attractor
    for (int i = start; i < end; i++)
    {
        asteroids[i].computeGravitationnalForce(attractor, orbitFactor * orbitFactor);
    }

    // Simulate step
    for (int i = start; i < end; i++)
    {
        asteroids[i].update(step);
    }
}

void AsteroidThreadPool::computeGPUDataForIndexes(int start, int end)
{

    // Get camera position for distance computation
    cgp::vec3 camera_position = getCameraPosition();
    cgp::mat3 rotation;

    for (int i = start; i < end; i++)
    {
        // Compute the asteroid size to camera distance ratio. The higher, the lesser poly count is required
        float ratio = cgp::norm(Object::scaleDownDistanceForDisplay(asteroids[i].getPhysicsPosition()) - camera_position) / (asteroid_config_data[i].scale * ASTEROID_DISPLAY_RADIUS);

        int mesh_index;
        bool is_low_poly_disk = false;
        if (ratio < 100) // Maybe lower
        {
            mesh_index = distance_mesh_handlers[asteroid_config_data[i].mesh_handler_index].high_poly;
        }
        else if (ratio < 200) // Maybe higher
        {
            mesh_index = distance_mesh_handlers[asteroid_config_data[i].mesh_handler_index].low_poly;
        }
        else
        {
            mesh_index = distance_mesh_handlers[asteroid_config_data[i].mesh_handler_index].low_poly_disk;
            is_low_poly_disk = true;
        }

        // If this is the low poly disk, compute the rotation to face the camera
        rotation = is_low_poly_disk ? cgp::rotation_transform::from_vector_transform({0, 0, 1}, cgp::normalize(camera_position - Object::scaleDownDistanceForDisplay(asteroids[i].getPhysicsPosition()))).matrix() : asteroids[i].getPhysicsRotation().matrix();

        //  Add data to the GPU buffer
        gpu_data_buffer[i] = {Object::scaleDownDistanceForDisplay(asteroids[i].getPhysicsPosition()), rotation, mesh_index};
    }
}
