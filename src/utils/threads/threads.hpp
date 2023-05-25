#pragma once

#include <atomic>
#include <condition_variable>

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