#pragma once

#include "cgp/core/array/numarray_stack/special_types/special_types.hpp"
#include "utils/opengl/shield_ubo.hpp"
#include <atomic>
#include <condition_variable>
#include <deque>
#include <shared_mutex>

// Multithreading utils. Used in the asteroid simulation

// Class to synchronize threads
class ThreadsSync
{
public:
    // Initialize functions
    void setThreadCount(int thread_count);

    // Worker threads tools
    void markDone();     // Add one to the done count
    void awaitRestart(); // Wait for the restart signal

    // Controller thread tools
    void awaitAll(); // Wait for all threads to be done
    void start();    // Start the threads again

private:
    int thread_count = 0;
    std::atomic<int> done_count;

    std::condition_variable cv;
    std::mutex sync_mutex;
};

// Class to lock a variable for reading or writing, but not for both at the same time
// Added support for c++17 in CMakelists.txt for shared_mutex
template <class T>
class ReadWriteLock
{
public:
    T read()
    {
        std::shared_lock<std::shared_mutex> lock(shared_mutex);
        return value;
    }

    void write(T new_value)
    {
        std::unique_lock<std::shared_mutex> lock(shared_mutex);
        value = new_value;
    }

private:
    std::shared_mutex shared_mutex;
    T value;
};

// Used to keep track of asteroid collision shield animation
template <class T>
class ThreadSafeDeque
{
public:
    ThreadSafeDeque() = default;

    // Add one element
    virtual void add(T element)
    {
        std::unique_lock<std::shared_mutex> lock(shared_mutex);

        deque.push_back(element);
    }

    // Remove the first element
    void remove()
    {
        std::unique_lock<std::shared_mutex> lock(shared_mutex);
        deque.pop_front();
    }

    // Not used directly,
    std::deque<T> read_all()
    {
        std::shared_lock<std::shared_mutex> lock(shared_mutex);
        return deque;
    }

protected:
    std::deque<T> deque;
    std::shared_mutex shared_mutex;
};

class AsteroidCollisionAnimationBuffer : public ThreadSafeDeque<cgp::vec4>
{
public:
    AsteroidCollisionAnimationBuffer() = default;
    AsteroidCollisionAnimationBuffer(int max_size, float animation_time) : ThreadSafeDeque(), animation_time(animation_time), max_size(max_size) {}

    // Convert the content to a float buffer that can be send to the shader
    collision_points toCollisionPoints(); // Build the data to send to the GPU

    // Add element, but remove one if size grows too big
    void add(cgp::vec4 element) override;

    // Update animation times and delete data that has reached the end
    void update();

    std::atomic<float> animation_time;

private:
    int max_size;
};
