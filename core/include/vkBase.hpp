#pragma once
#include "vkCore.hpp"

namespace vk
{
    class GraphicsBase
    {
    public:
        static GraphicsBase &Base()
        {
            return mSingleton;
        }

        // api version
        uint32_t GetApiVersion() const { return mApiVersion; }
        VkResult UseLatestApiVersion()
        {
            // TODO 1-3
            if (vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion"))
            {
                return vkEnumerateInstanceVersion(&mApiVersion);
            }
            return VK_SUCCESS;
        }

        // Getters
        VkInstance GetInstance() const { return mInstance; }
        const std::vector<const char *> &GetInstanceLayers() const { return mInstanceLayers; }
        const std::vector<const char *> &GetInstanceExtensions() const { return mInstanceExtensions; }
        VkSurfaceKHR GetSurface() const { return mSurface; }

        VkPhysicalDevice GetPhysicalDevice() const { return mPhysicalDevice; }
        const VkPhysicalDeviceProperties &GetPhysicalDeviceProperties() const { return mPhysicalDeviceProperties; }
        const VkPhysicalDeviceMemoryProperties &GetPhysicalDeviceMemoryProperties() const { return mPhysicalDeviceMemoryProperties; }
        VkPhysicalDevice GetAvailablePhysicalDevice(uint32_t index) const { return mAvailablePhysicalDevices[index]; }
        uint32_t GetAvailablePhysicalDeviceCount() const { return uint32_t(mAvailablePhysicalDevices.size()); }

        VkDevice GetDevice() const { return mDevice; }
        uint32_t GetQueueFamilyIdxGraphics() const { return mQueueFamilyIdxGraphics; }
        uint32_t GetQueueFamilyIdxPresentation() const { return mQueueFamilyIdxPresentation; }
        uint32_t GetQueueFamilyIdxCompute() const { return mQueueFamilyIdxCompute; }
        VkQueue GetQueueGraphics() const { return mQueueGraphics; }
        VkQueue GetQueuePresentation() const { return mQueuePresentation; }
        VkQueue GetQueueCompute() const { return mQueueCompute; }
        const std::vector<const char *> &GetDeviceExtensions() const { return mDeviceExtensions; }

        const VkFormat &AvailableSurfaceFormat(uint32_t index) const { return mAvailableSurfaceFormats[index].format; }
        const VkColorSpaceKHR &AvailableSurfaceColorSpace(uint32_t index) const { return mAvailableSurfaceFormats[index].colorSpace; }
        uint32_t AvailableSurfaceFormatCount() const { return uint32_t(mAvailableSurfaceFormats.size()); }
        VkSwapchainKHR GetSwapChain() const { return mSwapChain; }
        VkImage GetSwapChainImage(uint32_t index) const { return mSwapChainImages[index]; }
        VkImageView GetSwapChainImageView(uint32_t index) const { return mSwapChainImageViews[index]; }
        uint32_t GetSwapChainImageCount() const { return uint32_t(mSwapChainImages.size()); }
        const VkSwapchainCreateInfoKHR &GetSwapChainCreateInfo() const { return mSwapChainCreateInfo; }

        // before create instance
        void AddInstanceLayer(const char *layerName)
        {
            AddLayerOrExtension(mInstanceLayers, layerName);
        }
        void AddInstanceExtension(const char *extensionName)
        {
            AddLayerOrExtension(mInstanceExtensions, extensionName);
        }

        // create instance
        VkResult CreateInstance(VkInstanceCreateFlags flags = 0)
        {
#ifndef NDEBUG
            AddInstanceLayer("VK_LAYER_KHRONOS_validation");
            AddInstanceExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
            VkApplicationInfo appInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .apiVersion = mApiVersion};
            VkInstanceCreateInfo instanceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .flags = flags,
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = uint32_t(mInstanceLayers.size()),
                .ppEnabledLayerNames = mInstanceLayers.data(),
                .enabledExtensionCount = uint32_t(mInstanceExtensions.size()),
                .ppEnabledExtensionNames = mInstanceExtensions.data()};
            if (VkResult res = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance))
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to create a vulkan instance!\nError code: {}\n", int32_t(res));
                return res;
            }

            std::cout << std::format("Vulkan API Version: {}.{}.{}\n", VK_VERSION_MAJOR(mApiVersion), VK_VERSION_MINOR(mApiVersion), VK_VERSION_PATCH(mApiVersion));

#ifndef NDEBUG
            CreateDebugMessenger();
#endif
            return VK_SUCCESS;
        }

        // after create instance failed
        VkResult CheckInstanceLayers(std::span<const char *> layersToCheck)
        {
            uint32_t layerCount = 0;
            std::vector<VkLayerProperties> availableLayers;
            if (VkResult res = vkEnumerateInstanceLayerProperties(&layerCount, nullptr)) // get usable layer count
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to get the count of instance layers!\n");
                return res;
            }

            if (layerCount)
            {
                // find layer name, if not, set to nullptr in layersToCheck
                availableLayers.resize(layerCount);
                if (VkResult res = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()))
                {
                    std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to enumerate instance layer properties!\nError code: {}\n", int32_t(res));
                    return res;
                }
                for (auto &i : layersToCheck)
                {
                    bool found = false;
                    for (auto &j : availableLayers)
                    {
                        if (!strcmp(i, j.layerName))
                        {
                            found = true;
                            break;
                        }
                        if (!found)
                        {
                            i = nullptr;
                        }
                    }
                }
            }
            else
            {
                for (auto &i : layersToCheck)
                {
                    i = nullptr;
                }
            }
            return VK_SUCCESS;
        }
        void SetInstanceLayers(const std::vector<const char *> &layerNames)
        {
            mInstanceLayers = layerNames;
        }
        VkResult CheckInstanceExtensions(std::span<const char *> extensionsToCheck, const char *layerName = nullptr) const
        {
            uint32_t extensionCount = 0;
            std::vector<VkExtensionProperties> availableExtensions;
            if (VkResult res = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr))
            {
                layerName ? std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to get the count of instance extensions!\nLayer name:{}\n", layerName)
                          : std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to get the count of instance extensions!\n");
                return res;
            }
            if (extensionCount)
            {
                availableExtensions.resize(extensionCount);
                if (VkResult res = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, availableExtensions.data()))
                {
                    std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to enumerate instance extension properties!\nError code: {}\n", int32_t(res));
                    return res;
                }
                for (auto &i : extensionsToCheck)
                {
                    bool found = false;
                    for (auto &j : availableExtensions)
                    {
                        if (!strcmp(i, j.extensionName))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        i = nullptr;
                    }
                }
            }
            else
            {
                for (auto &i : extensionsToCheck)
                {
                    i = nullptr;
                }
            }
            return VK_SUCCESS;
        }
        void SetInstanceExtensions(const std::vector<const char *> &extensionNames)
        {
            mInstanceExtensions = extensionNames;
        }

        // before choose physical device
        void SetSurface(VkSurfaceKHR surface)
        {
            if (!this->mSurface)
            {
                this->mSurface = surface;
            }
        }

        // before create logical device
        void AddDeviceExtension(const char *extensionName)
        {
            AddLayerOrExtension(mDeviceExtensions, extensionName);
        }
        // get physical device
        VkResult GetPhysicalDevices()
        {
            uint32_t deviceCount;
            if (VkResult res = vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr))
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to get the count of physical devices!\nError code: {}\n", int32_t(res));
                return res;
            }
            if (!deviceCount)
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to find any physical device supports vulkan!\n"), abort();
            }
            mAvailablePhysicalDevices.resize(deviceCount);
            VkResult res = vkEnumeratePhysicalDevices(mInstance, &deviceCount, mAvailablePhysicalDevices.data());
            if (res)
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to enumerate physical devices!\nError code: {}\n", int32_t(res));
            }
            return res;
        }
        // appoint physical device and call GetQueueFamilyIndices(...) to get queue family indices
        VkResult DeterminePhysicalDevice(uint32_t deviceIndex = 0, bool enableGraphicsQueue, bool enableComputeQueue = true)
        {
            // define a special value to mark a queue family index has been found but not found
            static constexpr uint32_t notFound = INT32_MAX; //== VK_QUEUE_FAMILY_IGNORED & INT32_MAX
            // define a struct to store queue family indices
            struct queueFamilyIndexCombination
            {
                uint32_t graphics = VK_QUEUE_FAMILY_IGNORED;
                uint32_t presentation = VK_QUEUE_FAMILY_IGNORED;
                uint32_t compute = VK_QUEUE_FAMILY_IGNORED;
            };
            // queueFamilyIndexCombinations use for store queue family indices of each physical device
            static std::vector<queueFamilyIndexCombination> queueFamilyIndexCombinations(mAvailablePhysicalDevices.size());
            auto &[ig, ip, ic] = queueFamilyIndexCombinations[deviceIndex];

            // if any queue family index has been found but not found, return VK_RESULT_MAX_ENUM
            if (ig == notFound && enableGraphicsQueue ||
                ip == notFound && mSurface ||
                ic == notFound && enableComputeQueue)
                return VK_RESULT_MAX_ENUM;

            // if any queue family index has not been found, get queue family indices
            if (ig == VK_QUEUE_FAMILY_IGNORED && enableGraphicsQueue ||
                ip == VK_QUEUE_FAMILY_IGNORED && mSurface ||
                ic == VK_QUEUE_FAMILY_IGNORED && enableComputeQueue)
            {
                uint32_t indices[3];
                VkResult res = GetQueueFamilyIndices(mAvailablePhysicalDevices[deviceIndex], enableGraphicsQueue, enableComputeQueue, indices);
                // if GetQueueFamilyIndices(...) return VK_SUCCESS or VK_RESULT_MAX_ENUM(vkGetPhysicalDeviceSurfaceSupportKHR(...) execute success but not find queue family need),
                // means we have a conclusion about queue family index, store res to queueFamilyIndexCombinations[deviceIndex]
                // if idx gotten still is VK_QUEUE_FAMILY_IGNORED, means we not find queue family need, (VK_QUEUE_FAMILY_IGNORED（~0u） & INT32_MAX) = notFound
                if (res == VK_SUCCESS ||
                    res == VK_RESULT_MAX_ENUM)
                {
                    if (enableGraphicsQueue)
                        ig = indices[0] & INT32_MAX;
                    if (mSurface)
                        ip = indices[1] & INT32_MAX;
                    if (enableComputeQueue)
                        ic = indices[2] & INT32_MAX;
                }
                // if GetQueueFamilyIndices(...) execute failed, return
                if (res)
                    return res;
            }

            // if two of the branches above are not executed, means queue need have been get, get idx from queueFamilyIndexCombinations[deviceIndex]
            else
            {
                mQueueFamilyIdxGraphics = enableGraphicsQueue ? ig : VK_QUEUE_FAMILY_IGNORED;
                mQueueFamilyIdxPresentation = mSurface ? ip : VK_QUEUE_FAMILY_IGNORED;
                mQueueFamilyIdxCompute = enableComputeQueue ? ic : VK_QUEUE_FAMILY_IGNORED;
            }
            mPhysicalDevice = mAvailablePhysicalDevices[deviceIndex];
            return VK_SUCCESS;
        }
        // create logical device
        VkResult CreateDevice(VkDeviceCreateFlags flags = 0)
        {
            float queuePriority = 1.f;
            VkDeviceQueueCreateInfo queueCreateInfos[3] = {
                {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                 .queueCount = 1,
                 .pQueuePriorities = &queuePriority},
                {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                 .queueCount = 1,
                 .pQueuePriorities = &queuePriority},
                {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                 .queueCount = 1,
                 .pQueuePriorities = &queuePriority}};
            uint32_t queueCreateInfoCount = 0;
            if (mQueueFamilyIdxGraphics != VK_QUEUE_FAMILY_IGNORED)
                queueCreateInfos[queueCreateInfoCount++].queueFamilyIndex = mQueueFamilyIdxGraphics;
            if (mQueueFamilyIdxPresentation != VK_QUEUE_FAMILY_IGNORED &&
                mQueueFamilyIdxPresentation != mQueueFamilyIdxGraphics)
                queueCreateInfos[queueCreateInfoCount++].queueFamilyIndex = mQueueFamilyIdxPresentation;
            if (mQueueFamilyIdxCompute != VK_QUEUE_FAMILY_IGNORED &&
                mQueueFamilyIdxCompute != mQueueFamilyIdxGraphics &&
                mQueueFamilyIdxCompute != mQueueFamilyIdxPresentation)
                queueCreateInfos[queueCreateInfoCount++].queueFamilyIndex = mQueueFamilyIdxCompute;

            VkPhysicalDeviceFeatures physicalDeviceFeatures;
            vkGetPhysicalDeviceFeatures(mPhysicalDevice, &physicalDeviceFeatures);

            VkDeviceCreateInfo deviceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .flags = flags,
                .queueCreateInfoCount = queueCreateInfoCount,
                .pQueueCreateInfos = queueCreateInfos,
                .enabledExtensionCount = uint32_t(mDeviceExtensions.size()),
                .ppEnabledExtensionNames = mDeviceExtensions.data(),
                .pEnabledFeatures = &physicalDeviceFeatures};

            if (VkResult res = vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice))
            {
                std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to create a vulkan logical device!\nError code: {}\n", int32_t(res));
                return res;
            }

            if (mQueueFamilyIdxGraphics != VK_QUEUE_FAMILY_IGNORED)
                vkGetDeviceQueue(mDevice, mQueueFamilyIdxGraphics, 0, &mQueueGraphics);
            if (mQueueFamilyIdxPresentation != VK_QUEUE_FAMILY_IGNORED)
                vkGetDeviceQueue(mDevice, mQueueFamilyIdxPresentation, 0, &mQueuePresentation);
            if (mQueueFamilyIdxCompute != VK_QUEUE_FAMILY_IGNORED)
                vkGetDeviceQueue(mDevice, mQueueFamilyIdxCompute, 0, &mQueueCompute);
            vkGetPhysicalDeviceProperties(mPhysicalDevice, &mPhysicalDeviceProperties);
            vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mPhysicalDeviceMemoryProperties);
            // output device info
            std::cout << std::format("Renderer: {}\n", mPhysicalDeviceProperties.deviceName);
            // TODO 1-4
            return VK_SUCCESS;
        }
        // after create logical device failed
        VkResult CheckDeviceExtensions(std::span<const char *> extensionsToCheck, const char *layerName = nullptr) const
        {
            // TODO 1-3
        }
        void SetDeviceExtensions(const std::vector<const char *> &extensionNames)
        {
            mDeviceExtensions = extensionNames;
        }

        VkResult GetSurfaceFormats()
        {
            // TODO 1-4
        }
        VkResult SetSurfaceFormat(VkSurfaceFormatKHR surfaceFormat)
        {
            // TODO 1-4
        }
        // create swap chain
        VkResult CreateSwapChain(bool limitFrameRate = true, VkSwapchainCreateFlagsKHR flags = 0)
        {
            // TODO 1-4
        }
        // recreate swap chain
        VkResult RecreateSwapChain()
        {
            // TODO 1-4
        }

    private:
        GraphicsBase() = default;
        GraphicsBase(GraphicsBase &&) = delete;
        ~GraphicsBase()
        {
            // TODO 1-4
        }

        // push str ptr to layers or extensions and make sure it's not repeated
        static void AddLayerOrExtension(std::vector<const char *> &container, const char *name)
        {
            for (auto &item : container)
            {
                if (!strcmp(name, item)) // if str is matched, strcmp return 0
                {
                    return; // if layer or extension is repeated, return
                }
            }
            container.push_back(name);
        }

        VkResult CreateDebugMessenger()
        {
            static PFN_vkDebugUtilsMessengerCallbackEXT DebugUtilsMessengerCallback = [](
                                                                                          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                                          VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                                                          const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                                                          void *pUserData) -> VkBool32
            {
                std::cout << std::format("{}\n\n", pCallbackData->pMessage);
                return VK_FALSE;
            };

            // means to get all warnings and errors
            VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = DebugUtilsMessengerCallback};

            PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessenger =
                reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
            if (vkCreateDebugUtilsMessenger)
            {
                VkResult res = vkCreateDebugUtilsMessenger(mInstance, &debugUtilsMessengerCreateInfo, nullptr, &mDebugMessenger);
                if (res)
                    std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to create a debug messenger!\nError code: {}\n", int32_t(res));
                return res;
            }
            std::cout << std::format("[ graphicsBase ] ERROR\nFailed to get the function pointer of vkCreateDebugUtilsMessengerEXT!\n");
            return VK_RESULT_MAX_ENUM;
        }

        // called by DeterminePhysicalDevice(...), use for check device is satisfied to queue type
        VkResult GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, bool enableGraphicsQueue, bool enableComputeQueue, uint32_t (&queueFamilyIndices)[3])
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
            if (!queueFamilyCount)
            {
                return VK_RESULT_MAX_ENUM;
            }
            std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

            auto &[ig, ip, ic] = queueFamilyIndices;
            ig = ip = ic = VK_QUEUE_FAMILY_IGNORED;
            for (uint32_t i = 0; i < queueFamilyCount; i++)
            {
                // indicate whether queue can be got
                VkBool32
                    // only when enableGraphicsQueue is true, get queue family index that support graphics
                    supportGraphics = enableGraphicsQueue && queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT,
                    supportPresentation = false,
                    // only when enableComputeQueue is true, get queue family index that support compute
                    supportCompute = enableComputeQueue && queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT;
                // only when create window surface, get queue family index that support presentation
                if (mSurface)
                    if (VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, mSurface, &supportPresentation))
                    {
                        std::cout << std::format("[ GraphicsBase ] ERROR\nFailed to determine if the queue family supports presentation!\nError code: {}\n", int32_t(res));
                        return res;
                    }
                // only when enableGraphicsQueue and enableComputeQueue are true, get queue family index that support graphics and compute
                if (supportGraphics && supportCompute)
                {
                    // if supportPresentation is true, ig, ip, ic must be same
                    if (supportPresentation)
                    {
                        ig = ip = ic = i;
                        break;
                    }
                    // unless ig and ic are both got and same, otherwise overwrite their values with i, to ensure two queue family indices are same
                    if (ig != ic || ig == VK_QUEUE_FAMILY_IGNORED)
                        ig = ic = i;
                    // if no need to present, then can break
                    if (!mSurface)
                        break;
                }
                // if any queue family index is ignored, overwrite its value with i7
                if (supportGraphics && ig == VK_QUEUE_FAMILY_IGNORED)
                    ig = i;
                if (supportPresentation && ip == VK_QUEUE_FAMILY_IGNORED)
                    ip = i;
                if (supportCompute && ic == VK_QUEUE_FAMILY_IGNORED)
                    ic = i;
            }
            // if any queue family index is ignored, return error
            if (ig == VK_QUEUE_FAMILY_IGNORED && enableGraphicsQueue ||
                ip == VK_QUEUE_FAMILY_IGNORED && mSurface ||
                ic == VK_QUEUE_FAMILY_IGNORED && enableComputeQueue)
            {
                return VK_RESULT_MAX_ENUM;
            }
            // function success, write queue family index to member variable
            mQueueFamilyIdxGraphics = ig;
            mQueueFamilyIdxPresentation = ip;
            mQueueFamilyIdxCompute = ic;
            return VK_SUCCESS;
        }

        // called by CreateSwapChain(...) and RecreateSwapChain()
        VkResult CreateSwapChainInternal()
        {
            // TODO 1-4
        }

    private:
        // api
        uint32_t mApiVersion = VK_API_VERSION_1_0;

        static GraphicsBase mSingleton;

        VkInstance mInstance;
        std::vector<const char *> mInstanceLayers;
        std::vector<const char *> mInstanceExtensions;

        VkDebugUtilsMessengerEXT mDebugMessenger;

        VkSurfaceKHR mSurface;

        VkPhysicalDevice mPhysicalDevice;
        VkPhysicalDeviceProperties mPhysicalDeviceProperties;
        VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
        std::vector<VkPhysicalDevice> mAvailablePhysicalDevices;

        VkDevice mDevice;
        // valid idx from 0, use uint32_max as queue idx default
        uint32_t mQueueFamilyIdxGraphics = VK_QUEUE_FAMILY_IGNORED;
        uint32_t mQueueFamilyIdxPresentation = VK_QUEUE_FAMILY_IGNORED;
        uint32_t mQueueFamilyIdxCompute = VK_QUEUE_FAMILY_IGNORED;
        VkQueue mQueueGraphics;
        VkQueue mQueuePresentation;
        VkQueue mQueueCompute;

        std::vector<const char *> mDeviceExtensions;

        std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
        VkSwapchainKHR mSwapChain;
        std::vector<VkImage> mSwapChainImages;
        std::vector<VkImageView> mSwapChainImageViews;
        // store swap chain creation info
        VkSwapchainCreateInfoKHR mSwapChainCreateInfo = {};
    };
    inline GraphicsBase GraphicsBase::mSingleton;
}