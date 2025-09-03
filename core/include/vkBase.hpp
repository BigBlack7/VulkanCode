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
            // TODO 1-3
        }

        // after create instance failed
        VkResult CheckInstanceLayers(std::span<const char *> layersToCheck)
        {
            // TODO 1-3
        }
        void SetInstanceLayers(const std::vector<const char *> &layerNames)
        {
            mInstanceLayers = layerNames;
        }
        VkResult CheckInstanceExtensions(std::span<const char *> extensionsToCheck, const char *layerName = nullptr) const
        {
            // TODO 1-3
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
            // TODO 1-3
        }
        // appoint physical device and call GetQueueFamilyIndices(...) to get queue family indices
        VkResult DeterminePhysicalDevice(uint32_t deviceIndex = 0, bool enableGraphicsQueue, bool enableComputeQueue = true)
        {
            // TODO 1-3
        }
        // create logical device
        VkResult CreateDevice(VkDeviceCreateFlags flags = 0)
        {
            // TODO 1-3
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
            // TODO 1-3
        }

        // called by DeterminePhysicalDevice(...), use for check device is satisfied to queue type
        VkResult GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, bool enableGraphicsQueue, bool enableComputeQueue, uint32_t (&queueFamilyIndices)[3])
        {
            // TODO 1-3
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