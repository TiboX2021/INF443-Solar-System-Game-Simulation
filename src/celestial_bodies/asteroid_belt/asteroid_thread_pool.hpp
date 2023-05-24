#pragma once
#include "utils/physics/object.hpp"
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// TEST : a pool of Objects, that send positions and rotations array on each frame.

// Double buffer : one for the last data, one for the new data. Who choses to replace it ? What if the worker thread is late ?

class AsteroidThreadPool
{
public:
    AsteroidThreadPool(int n_threads) : isRunning(false), n_threads(n_threads)
    {
    }
    AsteroidThreadPool(const AsteroidThreadPool &other);

    void start()
    {
        if (isRunning) return;

        // Threads are not running yet

        threads.clear();
        isRunning = true;

        // Launch threads
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
            // Artificial sleep time
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Ending thread with indexes " << start_index << " to " << end_index << std::endl;
    }

private:
    std::atomic<bool> isRunning;

    int n_threads = 4; // Number of threads to be launched

    std::vector<Object> asteroids; // Asteroid physical objects

    // TODO : include a buffer. This object manages the threads, how many asteroids they do, etc

    // TODO : manage this. Each thread function must use only a part of the asteroid vector
    // But all the threads must reconstitute a full array before sending it to the main thread
    // TODO : it must also compute distance to the camera, on each frame. How to pass the camera position to the threads ? It cannot be done in advance
    // Threads
    std::vector<std::thread> threads;
};