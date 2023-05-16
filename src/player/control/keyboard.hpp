#pragma once

#include <iostream>
#include <thread>

// Handle keyboard input in a separate thread
class KeyboardInput
{
public:
    void start(); // Start the keyboard input handler

    void stop(); // Stop the keyboard input handler

private:
    std::thread input_handler;
    bool isRunning;
    char lastInput;

    // Private thread handler function
    void handle_input(); // Handle keyboard input
};
