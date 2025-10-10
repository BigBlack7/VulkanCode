#pragma once
#include "vkBase.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

// windows ptr
GLFWwindow *pWindow;
// display info ptr
GLFWmonitor *pMonitor;
// window title
const char *pWindowTitle = "Phoebus";

bool InitializeWindow(VkExtent2D size, bool isFullscreen = false, bool isResizeable = true, bool isLimitFrameRate = true)
{
    if (!glfwInit())
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to initialize GLFW!\n");
        return false;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizeable);

    // get display info ptr
    pMonitor = glfwGetPrimaryMonitor();
    // get display mode, mode may change when program running, so get it when need
    const GLFWvidmode *pMode = glfwGetVideoMode(pMonitor);

    // fourth para appoint what display is fullscreen, if nullptr, use window mode
    // fifth para mean share content with another window
    // accroding to isFullscreen, decide whether to use fullscreen mode
    pWindow = isFullscreen ? glfwCreateWindow(pMode->width, pMode->height, pWindowTitle, pMonitor, nullptr)
                           : glfwCreateWindow(size.width, size.height, pWindowTitle, nullptr, nullptr);
    // if failed, clear glfw
    if (!pWindow)
    {
        std::cout << std::format("[ InitializeWindow ]\nFailed to create a glfw window!\n");
        glfwTerminate();
        return false;
    }
#ifdef _WIN32
    vkb::GraphicsBase::Base().AddInstanceExtension(VK_KHR_SURFACE_EXTENSION_NAME);
    vkb::GraphicsBase::Base().AddInstanceExtension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
    // get instance extensions
    uint32_t extensionCount = 0;
    const char **extensionNames;
    extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount); // get platform required extensions
    if (!extensionNames)
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nVulkan is not available on this machine!\n");
        glfwTerminate();
        return false;
    }
    for (size_t i = 0; i < extensionCount; i++)
    {
        vk::GraphicsBase::Base().AddInstanceExtension(extensionNames[i]);
    }
#endif
    vkb::GraphicsBase::Base().AddDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    // create instance before create window surface
    vkb::GraphicsBase::Base().UseLatestApiVersion();
    if (vkb::GraphicsBase::Base().CreateInstance())
    {
        return false;
    }
    // create window surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (VkResult res = glfwCreateWindowSurface(vkb::GraphicsBase::Base().GetInstance(), pWindow, nullptr, &surface))
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to create window surface!\nError code: {}\n", int32_t(res));
        glfwTerminate();
        return false;
    }
    vkb::GraphicsBase::Base().SetSurface(surface);

    // by using || operator to short-circuit execution to save a few lines
    if ( // get physical device, and use first one, don't consider exchange physical device after any func below fail
        vkb::GraphicsBase::Base().GetPhysicalDevices() ||
        vkb::GraphicsBase::Base().DeterminePhysicalDevice(0, true, false) ||
        vkb::GraphicsBase::Base().CreateDevice())
        return false;
    //----------------------------------------

    if (vkb::GraphicsBase::Base().CreateSwapChain(isLimitFrameRate))
        return false;
    return true;
}

/// @brief terminate window
void TerminateWindow()
{
    vkb::GraphicsBase::Base().WaitIdle();
    glfwTerminate();
}

void FPSDisplay()
{
    static double t0 = glfwGetTime();
    static double t1;
    static double dt;
    static int dFrame = -1;
    static std::stringstream info;
    t1 = glfwGetTime();
    dFrame++;
    if ((dt = t1 - t0) >= 1)
    {
        info.precision(1);
        info << pWindowTitle << "   " << std::fixed << dFrame / dt << " FPS";
        glfwSetWindowTitle(pWindow, info.str().c_str());
        info.str("");
        t0 = t1;
        dFrame = 0;
    }
}