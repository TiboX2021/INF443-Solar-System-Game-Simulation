#include "threads.hpp"
#include "utils/physics/object.hpp"
#include <iostream>

void ThreadsSync::setThreadCount(int thread_count)
{
    this->thread_count = thread_count;
    done_count.store(0); // Reset the ready count
}

void ThreadsSync::markDone()
{
    done_count++;
    cv.notify_all(); // Do not use tthe same condition variable?
}

void ThreadsSync::awaitRestart()
{
    // Wait for all threads to be done, and for the counter to be reset
    // This function gets unlocked when the count is back to 0.
    std::unique_lock<std::mutex> lock(sync_mutex);
    cv.wait(lock, [this]
            { return done_count.load() == 0; });
}

void ThreadsSync::start()
{
    // Reset the done count
    done_count.store(0);

    // Notify all threads that they can start again
    cv.notify_all();
}

// TODO : use a non-blocking, "frame dropping" version of this ?
void ThreadsSync::awaitAll()
{
    // Wait for all threads to be done
    // This function gets unlocked when the done count is equal to the thread count (ie all threasd are done)
    std::unique_lock<std::mutex> lock(sync_mutex);
    cv.wait(lock, [this]
            { return done_count.load() == thread_count; });
}

// Update animation times and delete data that has reached the end
void AsteroidCollisionAnimationBuffer::update()
{
    // Update times
    float dt = Timer::dt;

    // Loop and modify deque values
    std::unique_lock lock(shared_mutex);

    // Loop through the deque and update the times
    for (int i = 0; i < deque.size(); i++)
    {
        // Update time
        deque[i].w += dt;

        // Delete if time is up
        if (deque[i].w > animation_time)
        {
            // Because all animations have the same order, this will always delete the first element (more efficient)
            deque.erase(deque.begin() + i);
            i--;
        }
    }
}

// Convert the content to a float buffer that can be send to the shader
std::vector<cgp::vec4> AsteroidCollisionAnimationBuffer::toFloatBuffer()
{
    std::shared_lock lock(shared_mutex);

    return std::vector<cgp::vec4>(deque.begin(), deque.end());
}

void AsteroidCollisionAnimationBuffer::add(cgp::vec4 element)
{
    std::unique_lock lock(shared_mutex);

    if (deque.size() >= max_size)
    {
        deque.erase(deque.begin());
    }
    deque.push_back(element);
}