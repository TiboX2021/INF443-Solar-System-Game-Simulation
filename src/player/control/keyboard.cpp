#include "keyboard.hpp"

// TODO
void KeyboardInput::start()
{
    isRunning = true;
    input_handler = std::thread(&KeyboardInput::handle_input, this);
}

// Stop the Keyboard input thread
void KeyboardInput::stop()
{
    isRunning = false;
    if (input_handler.joinable())
    {

        input_handler.join();
    }
}

void KeyboardInput::handle_input()
{
    std::cout << "Keyboard input handler is ready !\n";

    while (isRunning)
    {
        if (std::cin.peek() != EOF)
        {
            std::cin >> lastInput;

            // Process the input
            // Example: Check if the 'q' key was pressed to stop the input handling
            if (lastInput == 'q')
            {
                stop();
            }
            std::cout << "Input: " << lastInput << "\n"; // DEBUG : display input
        }

        // Sleep for a short duration to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}