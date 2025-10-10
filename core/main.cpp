#include "glfwGeneral.hpp"
#include "vkCreate.hpp"

int main()
{

    if (!InitializeWindow({1280, 720}))
        return -1;

    const auto &[renderPass, framebuffers] = vkc::CreateRPWF();

    vkb::Fence fence; // create fence by not signaled
    vkb::Semaphore semaphore_imageIsAvailable;
    vkb::Semaphore semaphore_renderIsOver;

    vkb::CommandBuffer commandBuffer;
    vkb::CommandPool commandPool(vkb::GraphicsBase::Base().GetQueueFamilyIdxGraphics(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    commandPool.AllocateBuffers(commandBuffer);

    VkClearValue clearColor = {.color = {0.537f, 0.812f, 0.941f, 1.f}};

    while (!glfwWindowShouldClose(pWindow))
    {
        while (glfwGetWindowAttrib(pWindow, GLFW_ICONIFIED))
        {
            // block loop when window is iconified
            glfwWaitEvents();
        }

        // get swapchain image index
        vkb::GraphicsBase::Base().SwapImage(semaphore_imageIsAvailable);
        auto i = vkb::GraphicsBase::Base().GetCurrentImageIndex();

        commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        renderPass.CmdBegin(commandBuffer, framebuffers[i], {{}, windowSize}, clearColor);
        // TODO:render instructions
        renderPass.CmdEnd(commandBuffer);

        commandBuffer.End();

        vkb::GraphicsBase::Base().SubmitCommandBufferGraphics(commandBuffer, semaphore_imageIsAvailable, semaphore_renderIsOver, fence);
        vkb::GraphicsBase::Base().PresentImage(semaphore_renderIsOver);

        glfwPollEvents();
        FPSDisplay();

        // wait and reset fence
        fence.WaitAndReset();
    }
    TerminateWindow();

    std::cout << "Hello, Vulkan!" << std::endl;
    return 0;
}