#include "glfwGeneral.hpp"

int main()
{

    if (!InitializeWindow({1280, 720}))
    {
        return -1;
    }
    while (!glfwWindowShouldClose(pWindow))
    {
        // TODO render pass
        glfwPollEvents();
        FPSDisplay();
    }
    TerminateWindow();

    std::cout << "Hello, Vulkan!" << std::endl;
    return 0;
}