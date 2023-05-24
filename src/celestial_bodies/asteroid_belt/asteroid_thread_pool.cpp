#include "asteroid_thread_pool.hpp"

// Define the copy constructor
AsteroidThreadPool::AsteroidThreadPool(const AsteroidThreadPool &other)
{
    isRunning.store(other.isRunning.load());
}