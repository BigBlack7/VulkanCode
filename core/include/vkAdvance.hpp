#pragma once
#include "vkBase.hpp"
namespace vk
{
    struct GraphicsPipelineCreateInfoPack
    {
    public:
        VkGraphicsPipelineCreateInfo __createInfo__ =
            {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
        std::vector<VkPipelineShaderStageCreateInfo> __shaderStages__;
        // Vertex Input
        VkPipelineVertexInputStateCreateInfo __vertexInputStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        std::vector<VkVertexInputBindingDescription> __vertexInputBindings__;
        std::vector<VkVertexInputAttributeDescription> __vertexInputAttributes__;
        // Input Assembly
        VkPipelineInputAssemblyStateCreateInfo __inputAssemblyStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        // Tessellation
        VkPipelineTessellationStateCreateInfo __tessellationStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO};
        // Viewport
        VkPipelineViewportStateCreateInfo __viewportStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        std::vector<VkViewport> __viewports__;
        std::vector<VkRect2D> __scissors__;
        uint32_t __dynamicViewportCount__ = 1;
        uint32_t __dynamicScissorCount__ = 1;
        // Rasterization
        VkPipelineRasterizationStateCreateInfo __rasterizationStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        // Multisample
        VkPipelineMultisampleStateCreateInfo __multisampleStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        // Depth & Stencil
        VkPipelineDepthStencilStateCreateInfo __depthStencilStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
        // Color Blend
        VkPipelineColorBlendStateCreateInfo __colorBlendStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        std::vector<VkPipelineColorBlendAttachmentState> __colorBlendAttachmentStates__;
        // Dynamic
        VkPipelineDynamicStateCreateInfo __dynamicStateCi__ =
            {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        std::vector<VkDynamicState> __dynamicStates__;
        //--------------------
        GraphicsPipelineCreateInfoPack()
        {
            SetCreateInfos();
            // 若非派生管线，createInfo.basePipelineIndex不得为0，设置为-1
            __createInfo__.basePipelineIndex = -1;
        }
        GraphicsPipelineCreateInfoPack(const GraphicsPipelineCreateInfoPack &other) noexcept
        {
            __createInfo__ = other.__createInfo__;
            SetCreateInfos();

            __vertexInputStateCi__ = other.__vertexInputStateCi__;
            __inputAssemblyStateCi__ = other.__inputAssemblyStateCi__;
            __tessellationStateCi__ = other.__tessellationStateCi__;
            __viewportStateCi__ = other.__viewportStateCi__;
            __rasterizationStateCi__ = other.__rasterizationStateCi__;
            __multisampleStateCi__ = other.__multisampleStateCi__;
            __depthStencilStateCi__ = other.__depthStencilStateCi__;
            __colorBlendStateCi__ = other.__colorBlendStateCi__;
            __dynamicStateCi__ = other.__dynamicStateCi__;
            __shaderStages__ = other.__shaderStages__;
            __vertexInputBindings__ = other.__vertexInputBindings__;
            __vertexInputAttributes__ = other.__vertexInputAttributes__;
            __viewports__ = other.__viewports__;
            __scissors__ = other.__scissors__;
            __colorBlendAttachmentStates__ = other.__colorBlendAttachmentStates__;
            __dynamicStates__ = other.__dynamicStates__;
            UpdateAllArrayAddresses();
        }

        // Getter
        operator VkGraphicsPipelineCreateInfo &() { return __createInfo__; }
        // mon-const function
        void UpdateAllArrays()
        {
            __createInfo__.stageCount = __shaderStages__.size();
            __vertexInputStateCi__.vertexBindingDescriptionCount = __vertexInputBindings__.size();
            __vertexInputStateCi__.vertexAttributeDescriptionCount = __vertexInputAttributes__.size();
            __viewportStateCi__.viewportCount = __viewports__.size() ? uint32_t(__viewports__.size()) : __dynamicViewportCount__;
            __viewportStateCi__.scissorCount = __scissors__.size() ? uint32_t(__scissors__.size()) : __dynamicScissorCount__;
            __colorBlendStateCi__.attachmentCount = __colorBlendAttachmentStates__.size();
            __dynamicStateCi__.dynamicStateCount = __dynamicStates__.size();
            UpdateAllArrayAddresses();
        }

    private:
        // 该函数用于将创建信息的地址赋值给basePipelineIndex中相应成员
        void SetCreateInfos()
        {
            __createInfo__.pVertexInputState = &__vertexInputStateCi__;
            __createInfo__.pInputAssemblyState = &__inputAssemblyStateCi__;
            __createInfo__.pTessellationState = &__tessellationStateCi__;
            __createInfo__.pViewportState = &__viewportStateCi__;
            __createInfo__.pRasterizationState = &__rasterizationStateCi__;
            __createInfo__.pMultisampleState = &__multisampleStateCi__;
            __createInfo__.pDepthStencilState = &__depthStencilStateCi__;
            __createInfo__.pColorBlendState = &__colorBlendStateCi__;
            __createInfo__.pDynamicState = &__dynamicStateCi__;
        }
        // 该函数用于将各个vector中数据的地址赋值给各个创建信息中相应成员，但不改变各个count
        void UpdateAllArrayAddresses()
        {
            __createInfo__.pStages = __shaderStages__.data();
            __vertexInputStateCi__.pVertexBindingDescriptions = __vertexInputBindings__.data();
            __vertexInputStateCi__.pVertexAttributeDescriptions = __vertexInputAttributes__.data();
            __viewportStateCi__.pViewports = __viewports__.data();
            __viewportStateCi__.pScissors = __scissors__.data();
            __colorBlendStateCi__.pAttachments = __colorBlendAttachmentStates__.data();
            __dynamicStateCi__.pDynamicStates = __dynamicStates__.data();
        }
    };
}