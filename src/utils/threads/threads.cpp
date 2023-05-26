#include "threads.hpp"

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