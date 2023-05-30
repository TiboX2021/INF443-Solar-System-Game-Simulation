#pragma once

#include <atomic>
#include <condition_variable>
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
        std::shared_lock lock(shared_mutex);
        return value;
    }

    void write(T new_value)
    {
        std::unique_lock lock(shared_mutex);
        value = new_value;
    }

private:
    std::shared_mutex shared_mutex;
    T value;
};
