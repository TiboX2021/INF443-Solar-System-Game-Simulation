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

// TODO : define functions here, less cluttered

// Instanciate threads, initialize thread sync and launch threads
void AsteroidThreadPool::start()
{
    if (isRunning) return;

    // Threads are not running yet
    threads.clear();
    isRunning = true;

    // Initialize thread sync
    sync_util.setThreadCount(n_threads);

    // Launch threads
    // TODO : use ASTEROIDS_PER_THREAD to determine the number of threads instead
    for (int i = 0; i < n_threads; i++)
    {
        threads.push_back(std::thread(&AsteroidThreadPool::worker, this, i, i + 1));
    }
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
    sync_util.start();    // Restart the threads
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

// Worker thread function
void AsteroidThreadPool::worker(int start_index, int end_index)
{
    // TODO : compute things.
    std::cout << "Starting thread with indexes " << start_index << " to " << end_index << std::endl;

    // TODO : compute loop
    while (isRunning)
    {
        // // Wait for framerate to unlock the next computation
        // std::unique_lock<std::mutex> lock(sync_mutex);
        // sync_condition_variable.wait(lock, [this]
        //                              { return ready; });

        // TODO : compute next positions. TODO : remove this, or test lag
        // Artificial sleep time
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Wait for the launch signal to be given for the next iteration.
        sync_util.markDone();
        sync_util.awaitRestart();
    }

    std::cout << "Ending thread with indexes " << start_index << " to " << end_index << std::endl;
}