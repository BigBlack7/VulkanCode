#include "glfwGeneral.hpp"
#include "vkCreate.hpp"

vk::PipelineLayout pipelineLayout_triangle;
vk::Pipeline pipeline_triangle;

const auto &RenderPassAndFramebuffers()
{
    static const auto &rpwf = vk::CreateRPWF();
    return rpwf;
}

void CreateLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayout_triangle.Create(pipelineLayoutCreateInfo);
}

void CreatePipeline()
{
    static vk::ShaderModule vert("D:\\MyWorkSpace\\VulkanCode\\shader\\FirstTriangle.vert.spv");
    static vk::ShaderModule frag("D:\\MyWorkSpace\\VulkanCode\\shader\\FirstTriangle.frag.spv");
    static VkPipelineShaderStageCreateInfo shaderStageCreateInfos_triangle[2] = {
        vert.StageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT),
        frag.StageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT)};

    auto Create = []
    {
        vk::GraphicsPipelineCreateInfoPack pipelineCiPack;
        pipelineCiPack.__createInfo__.layout = pipelineLayout_triangle;
        pipelineCiPack.__createInfo__.renderPass = RenderPassAndFramebuffers().__renderPass__;
        pipelineCiPack.__inputAssemblyStateCi__.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineCiPack.__viewports__.emplace_back(0.f, 0.f, float(windowSize.width), float(windowSize.height), 0.f, 1.f);
        pipelineCiPack.__scissors__.emplace_back(VkOffset2D{}, windowSize);
        pipelineCiPack.__multisampleStateCi__.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        pipelineCiPack.__colorBlendAttachmentStates__.push_back({.colorWriteMask = 0b1111});
        pipelineCiPack.UpdateAllArrays();
        pipelineCiPack.__createInfo__.stageCount = 2;
        pipelineCiPack.__createInfo__.pStages = shaderStageCreateInfos_triangle;
        pipeline_triangle.Create(pipelineCiPack);
    };
    auto Destroy = []
    {
        pipeline_triangle.~Pipeline();
    };
    vk::GraphicsBase::Base().AddCallbackCreateSwapchain(Create);
    vk::GraphicsBase::Base().AddCallbackDestroySwapchain(Destroy);
    Create();
}

int main()
{

    if (!InitializeWindow({1280, 720}))
        return -1;

    const auto &[renderPass, framebuffers] = RenderPassAndFramebuffers();
    CreateLayout();
    CreatePipeline();

    vk::Fence fence; // create fence by not signaled
    vk::Semaphore semaphore_imageIsAvailable;
    vk::Semaphore semaphore_renderIsOver;

    vk::CommandBuffer commandBuffer;
    vk::CommandPool commandPool(vk::GraphicsBase::Base().GetQueueFamilyIdxGraphics(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
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
        vk::GraphicsBase::Base().SwapImage(semaphore_imageIsAvailable);
        auto i = vk::GraphicsBase::Base().GetCurrentImageIndex();

        commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        renderPass.CmdBegin(commandBuffer, framebuffers[i], {{}, windowSize}, clearColor);
        // render instructions
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_triangle);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        renderPass.CmdEnd(commandBuffer);

        commandBuffer.End();

        vk::GraphicsBase::Base().SubmitCommandBufferGraphics(commandBuffer, semaphore_imageIsAvailable, semaphore_renderIsOver, fence);
        vk::GraphicsBase::Base().PresentImage(semaphore_renderIsOver);

        glfwPollEvents();
        FPSDisplay();

        // wait and reset fence
        fence.WaitAndReset();
    }
    TerminateWindow();

    std::cout << "Hello, Vulkan!" << std::endl;
    return 0;
}