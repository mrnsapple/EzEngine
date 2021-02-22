/*
** EPITECH PROJECT, 2020
** VulkanTest
** File description:
** HelloTrianglApp
*/

#include "../../../config.hpp"

#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <chrono>

// // Load image files
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
#include "Init.hpp"
#include "ResourceManager.hpp"
#include "ImageResource.hpp"

// // Load obj files
// #define TINYOBJLOADER_IMPLEMENTATION
// #include <tiny_obj_loader.h>

#include "log.hpp"
#include "Settings.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "renderer_exceptions.hpp"

#if defined(EZ_OS_LINUX)
    #ifndef VK_USE_PLATFORM_XLIB_KHR
    #define VK_USE_PLATFORM_XLIB_KHR
    #endif
    #include <vulkan/vulkan_xlib.h>
#elif defined(EZ_OS_WINDOWS)
    #ifndef VK_USE_PLATFORM_WIN32_KHR
    #define VK_USE_PLATFORM_WIN32_KHR
    #endif
    #include <vulkan/vulkan_win32.h>
#endif


using namespace ez;

void Renderer::Init(const Window& WS_handle, std::vector<Mesh*> meshContainer)
{
    LOG(EZ_TRACE, "------ Init Vulkan\n");

    LOG(EZ_TRACE, "createInstance");
    createInstance();

    LOG(EZ_TRACE, "setupDebugMessenger");
    setupDebugMessenger();
    
    LOG(EZ_TRACE, "createWindowSurface");
    createWindowSurface(WS_handle);

    LOG(EZ_TRACE, "pickphysicaldevice");
    pickphysicaldevice(WS_handle);
    
    LOG(EZ_TRACE, "createLogicalDevice");
    createLogicalDevice(WS_handle);

    LOG(EZ_TRACE, "createSwapChain");
    createSwapChain(WS_handle);

    LOG(EZ_TRACE, "createImageViews");
    createImageViews();

    LOG(EZ_TRACE, "createRenderPass");
    createRenderPass();

    LOG(EZ_TRACE, "createDescriptorSetLayout");
    createDescriptorSetLayout();

    LOG(EZ_TRACE, "createGraphicsPipeline");
    createGraphicsPipeline();

    if (msaaLevel > 1)
    {
        LOG(EZ_TRACE, "createColorResources");
        createColorResources();
    }

    LOG(EZ_TRACE, "createDepthResources");
    createDepthResources();

    LOG(EZ_TRACE, "createFramebuffers");
    createFramebuffers();

    LOG(EZ_TRACE, "createCommandPool");
    createCommandPool(WS_handle);

    // LOG(EZ_TRACE, "updateMeshContainer\n");
    // updateMeshContainer(meshContainer);

    // TODO : get meshContainer from 

    for (size_t i = 0; i < meshContainer.size(); ++i)
    {
        LOG(EZ_TRACE, "createTextureImage");
        createTextureImage(meshContainer[i]->primitive.texture);

        LOG(EZ_TRACE, "createTextureImageView");
        createTextureImageView(meshContainer[i]->primitive.texture);

        LOG(EZ_TRACE, "createTextureSampler");
        createTextureSampler(meshContainer[i]->primitive.texture);

        meshContainer[i]->primitive.texture.updateDescriptor();

        LOG(EZ_TRACE, "loadMesh\n");
        loadMesh(meshContainer[i]);
    }


    LOG(EZ_TRACE, "createVertexBuffer");
    createVertexBuffer();

    LOG(EZ_TRACE, "createIndexBuffer");
    createIndexBuffer();

    // LOG(EZ_TRACE, "createUniformBuffers");
    // created in createDescriptorSets
    // for (size_t i = 0; i < _meshContainer.size(); ++i)
        // createUniformBuffers(_meshContainer[i]);

    LOG(EZ_TRACE, "createDescriptorPool");
    createDescriptorPool(meshContainer);

    LOG(EZ_TRACE, "createDescriptorSets");
    createDescriptorSets(meshContainer);

    LOG(EZ_TRACE, "createCommandBuffers");
    createCommandBuffers(meshContainer);

    LOG(EZ_TRACE, "createSyncObjects");
    createSyncObjects();

    LOG(EZ_TRACE, "Init Vulkan - Done\n------\n");
}

void Renderer::Deinit(std::vector<Mesh*> meshContainer)
{
    vkDeviceWaitIdle(_device);
    Cleanup(meshContainer);
}

void Renderer::CleanupSwapChain(std::vector<Mesh*> meshContainer)
{
    if (msaaLevel > 1)
    {
        vkDestroyImageView(_device, _colorImageView, NULL);
        vkDestroyImage(_device, _colorImage, NULL);
        vkFreeMemory(_device, _colorImageMemory, NULL);
    }

    vkDestroyImageView(_device, _depthImageView, NULL);
    vkDestroyImage(_device, _depthImage, NULL);
    vkFreeMemory(_device, _depthImageMemory, NULL);

    for (size_t i = 0; i < _swapChainFramebuffers.size(); ++i)
        vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], NULL);

    vkFreeCommandBuffers(_device, _commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());

    vkDestroyPipeline(_device, _graphicsPipeline, NULL);
    vkDestroyPipelineLayout(_device, _pipelineLayout, NULL);
    vkDestroyRenderPass(_device, _renderPass, NULL);

    for (size_t i = 0; i < _swapChainImageViews.size(); ++i)
        vkDestroyImageView(_device, _swapChainImageViews[i], NULL);

    vkDestroySwapchainKHR(_device, _swapChain, NULL);

    for (auto& mesh : meshContainer)
    {
        for (size_t j = 0; j < mesh->uniformBuffers.size(); ++j)
        {
            vkDestroyBuffer(_device, mesh->uniformBuffers[j].buffer, NULL);
            vkFreeMemory(_device, mesh->uniformBuffers[j].deviceMemory, NULL);
        }
    }
    
    vkDestroyDescriptorPool(_device, _descriptorPool, NULL);
}

void Renderer::Cleanup(std::vector<Mesh*> meshContainer)
{
    CleanupSwapChain(meshContainer);

    for (auto& mesh : meshContainer)
    {
        vkDestroyImage(_device, mesh->primitive.texture.image, NULL);
        vkFreeMemory(_device, mesh->primitive.texture.deviceMemory, NULL);
        vkDestroyImageView(_device, mesh->primitive.texture.imageView, NULL);
        vkDestroySampler(_device, mesh->primitive.texture.sampler, NULL);
    }

    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, NULL);

    vkDestroyBuffer(_device, _indexBuffer, NULL);
    vkFreeMemory(_device, _indexBufferMemory, NULL);

    vkDestroyBuffer(_device, _vertexBuffer, NULL);
    vkFreeMemory(_device, _vertexBufferMemory, NULL);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(_device, _renderFinishedSemaphores[i], NULL);
        vkDestroySemaphore(_device, _imageAvailableSemaphores[i], NULL);
        vkDestroyFence(_device, _inFlightFences[i], NULL);
    }

    vkDestroyCommandPool(_device, _commandPool, NULL);

    vkDestroyDevice(_device, NULL);

    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, NULL);

    vkDestroySurfaceKHR(_instance, _surface, NULL);
    vkDestroyInstance(_instance, NULL);
}

void Renderer::DrawFrame(const Window& WS_handle, std::vector<Mesh*> meshContainer)
{
    if (WS_handle.GetWindowSize().format == WindowSizeFormat::MINIMIZED)
        return;
    
    vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

    // Rendering and presentation
    // Acquiring an image from the swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain(WS_handle, meshContainer);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        throw ez::vulkan_failure("SwapChainImage acquisition", __FILE__, __LINE__);

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        vkWaitForFences(_device, 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);

    // Mark the image as now being in use by this frame
    _imagesInFlight[imageIndex] = _inFlightFences[_currentFrame];

    // Updating uniform data (spin rectangle)
    updateUniformBuffer(imageIndex, meshContainer);

    // Submitting the command buffer
    VkSemaphore waitSemaphores[]        = { _imageAvailableSemaphores[_currentFrame] };
    VkSemaphore signalSemaphores[]      = { _renderFinishedSemaphores[_currentFrame] };
    VkPipelineStageFlags waitStages[]   = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo{};
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &_commandBuffers[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

    if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
        throw ez::vulkan_failure("DrawCommandBuffer submition", __FILE__, __LINE__);

    VkSwapchainKHR swapChains[] = { _swapChain };
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;
    presentInfo.swapchainCount      = 1;
    presentInfo.pSwapchains         = swapChains;
    presentInfo.pImageIndices       = &imageIndex;
    presentInfo.pResults            = NULL; // Optional

    result = vkQueuePresentKHR(_presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        RecreateSwapChain(WS_handle, meshContainer);
    else if (result != VK_SUCCESS)
        throw ez::vulkan_failure("SwapChainImage presentation", __FILE__, __LINE__);

    // Frames in flight
    vkQueueWaitIdle(_presentQueue);

    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::RecreateSwapChain(const Window& WS_handle, std::vector<Mesh*> meshContainer)
{
    if (WS_handle.GetWindowSize().format == WindowSizeFormat::MINIMIZED)
        return;

    vkDeviceWaitIdle(_device);

    CleanupSwapChain(meshContainer);

    createSwapChain(WS_handle);
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    if (msaaLevel > 1)
        createColorResources();
    createDepthResources();
    createFramebuffers();
    createDescriptorPool(meshContainer);
    createDescriptorSets(meshContainer);
    createCommandBuffers(meshContainer);
}










// ###################
// #   INIT VULKAN   #
// ###################

void Renderer::createInstance()
{
    // Enable validation layers
    if (enableValidationLayers && !checkValidationLayerSupport())
        throw ez::vulkan_unavailable_validation_layers("", __FILE__, __LINE__);

    // Initialize Application
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Initialize Instance
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Initialize Debug Utils
    if (enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = NULL;
    }

    // CREATE INSTANCE
    if (vkCreateInstance(&createInfo, NULL, &_instance) != VK_SUCCESS)
        throw ez::vulkan_failure("Instance creation", __FILE__, __LINE__);
}







void Renderer::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, NULL, &_debugMessenger) != VK_SUCCESS)
        throw ez::vulkan_failure("DebugMessenger creation", __FILE__, __LINE__);
}







void Renderer::createWindowSurface(const Window& WS_handle)
{
#if defined(EZ_OS_LINUX)
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType    = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext    = NULL;
        createInfo.flags    = 0;
        createInfo.dpy      = ((XWindow*)WS_handle)->GetDisplay();
        createInfo.window   = ((XWindow*)WS_handle)->GetWindow();

        if (vkCreateXlibSurfaceKHR(_instance, &createInfo, NULL, &_surface) != VK_SUCCESS)
            throw ez::vulkan_failure("WindowSurface creation", __FILE__, __LINE__);

#elif defined(EZ_OS_WINDOWS)
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hwnd = ((WNDWindow*)WS_handle)->GetHWND();
            createInfo.hinstance = GetModuleHandle(NULL);

            if (vkCreateWin32SurfaceKHR(_instance, &createInfo, NULL, &_surface) != VK_SUCCESS)
                throw ez::vulkan_failure("WindowSurface creation", __FILE__, __LINE__);
            // call vkCreate...SurfaceKHR(); 
#endif
}







void Renderer::pickphysicaldevice(const Window& WS_handle)
{
    uint32_t pDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(_instance, &pDeviceCount, NULL) != VK_SUCCESS)
        throw ez::vulkan_failure("PhysicalDevice enumeration", __FILE__, __LINE__);
    else if (pDeviceCount == 0)
        throw ez::vulkan_failure("to find GPU with Vulkan support", __FILE__, __LINE__);
    std::vector<VkPhysicalDevice> pDevices(pDeviceCount);
    if (vkEnumeratePhysicalDevices(_instance, &pDeviceCount, pDevices.data()) != VK_SUCCESS)
        throw ez::vulkan_failure("PhysicalDevice enumeration", __FILE__, __LINE__);

    for (const auto& pDevice : pDevices) {
        if (isDeviceSuitable(pDevice, WS_handle)) {
            _physicalDevice = pDevice;
            _msaaSamples = getMaxUsableSampleCount();
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE)
        throw ez::vulkan_failure("to find suitable PhysicalDevice", __FILE__, __LINE__);
}







void Renderer::createLogicalDevice(const Window& WS_handle)
{
// Specifying the queues to be created
    QueueFamilyIndices famIndices = findQueueFamilies(_physicalDevice, WS_handle);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {famIndices.graphicsFamily.value(), famIndices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex    = queueFamily;
        queueCreateInfo.queueCount          = 1;
        queueCreateInfo.pQueuePriorities    = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

// Specifying used device features
    VkPhysicalDeviceFeatures pDeviceFeatures{};
    pDeviceFeatures.samplerAnisotropy = enableAnisotropy ? VK_TRUE : VK_FALSE;
    pDeviceFeatures.sampleRateShading = enableTextureSampling ? VK_TRUE : VK_FALSE;

// Creating the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount     = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos        = queueCreateInfos.data();
    createInfo.pEnabledFeatures         = &pDeviceFeatures;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames  = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount    = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames  = validationLayers.data();
    }
    else
        createInfo.enabledLayerCount    = 0;
    
    if (vkCreateDevice(_physicalDevice, &createInfo, NULL, &_device) != VK_SUCCESS)
        throw ez::vulkan_failure("LogicalDevice creation", __FILE__, __LINE__);


// Retrieving queue handles
    vkGetDeviceQueue(_device, famIndices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, famIndices.presentFamily.value(),  0, &_presentQueue);
}







void Renderer::createSwapChain(const Window& WS_handle)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(_physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode     = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent                = chooseSwapExtent(swapChainSupport.capabilities, WS_handle);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = _surface;
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices famIndices = findQueueFamilies(_physicalDevice, WS_handle);
    uint32_t queueFamilyIndices[] = {famIndices.graphicsFamily.value(), famIndices.presentFamily.value()};

    if (famIndices.graphicsFamily != famIndices.presentFamily) {
        createInfo.imageSharingMode         = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount    = 2;
        createInfo.pQueueFamilyIndices      = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount    = 0; // Optional
        createInfo.pQueueFamilyIndices      = NULL; // Optional
    }

    createInfo.preTransform     = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode      = presentMode;
    createInfo.clipped          = VK_TRUE;
    createInfo.oldSwapchain     = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(_device, &createInfo, NULL, &_swapChain) != VK_SUCCESS)
        throw ez::vulkan_failure("SwapChain creation", __FILE__, __LINE__);
    

    // Get SwapChain image handles
    imageCount = 0;
    vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, NULL);
    _swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;
}







void Renderer::createImageViews()
{
    _swapChainImageViews.resize(_swapChainImages.size());

    for (size_t i = 0; i < _swapChainImageViews.size(); ++i)
        _swapChainImageViews[i] = createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}







void Renderer::createRenderPass()
{
    // Attachment description
    VkAttachmentDescription colorAttachment{}; // => multisampled attachment needs to be resolved to regular image
    colorAttachment.format          = _swapChainImageFormat;
    colorAttachment.samples         = _msaaSamples;
    colorAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout     = msaaLevel > 1 ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format           = _swapChainImageFormat;
    colorAttachmentResolve.samples          = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{}; // => depth attachment is never presented so no need for resolving it to regular image
    depthAttachment.format          = findDepthFormat();
    depthAttachment.samples         = _msaaSamples;
    depthAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Subpasses and attachment references
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment   = 0;
    colorAttachmentRef.layout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment   = 1;
    depthAttachmentRef.layout       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment    = 2;
    colorAttachmentResolveRef.layout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    if (msaaLevel > 1)
        subpass.pResolveAttachments     = &colorAttachmentResolveRef;

    // Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass       = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass       = 0;
    dependency.srcStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask    = 0;
    dependency.dstStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Render pass
    std::array<VkAttachmentDescription, 3> MSattachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount  = static_cast<uint32_t>(msaaLevel > 1 ? MSattachments.size() : attachments.size());
    renderPassInfo.pAttachments     = msaaLevel > 1 ? MSattachments.data() : attachments.data();
    renderPassInfo.subpassCount     = 1;
    renderPassInfo.pSubpasses       = &subpass;
    renderPassInfo.dependencyCount  = 1;
    renderPassInfo.pDependencies    = &dependency;

    if (vkCreateRenderPass(_device, &renderPassInfo, NULL, &_renderPass) != VK_SUCCESS)
        throw ez::vulkan_failure("RenderPass creation", __FILE__, __LINE__);
}







void Renderer::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = NULL; // Optional

    // samplerDescriptor for fragmentShader access to texture through sampler
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding            = 1;
    samplerLayoutBinding.descriptorCount    = 1;
    samplerLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = NULL;
    samplerLayoutBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings    = bindings.data();

    if (vkCreateDescriptorSetLayout(_device, &layoutInfo, NULL, &_descriptorSetLayout) != VK_SUCCESS)
        throw ez::vulkan_failure("DescriptorSetLayout creation", __FILE__, __LINE__);

}







void Renderer::createGraphicsPipeline()
{
    // Init Shaders
    auto vertShaderCode = readFile(SHADER_VERT_PATH);
    auto fragShaderCode = readFile(SHADER_FRAG_PATH);

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName  = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName  = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // Vertex Input
    auto bindingDescription    = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount   = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();

    // Input Assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport and Scissors
    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = (float)_swapChainExtent.width;
    viewport.height   = (float)_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = _swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports    = &viewport;
    viewportState.scissorCount  = 1;
    viewportState.pScissors     = &scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType                    = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable         = VK_FALSE;
    rasterizer.rasterizerDiscardEnable  = VK_FALSE;
    rasterizer.polygonMode              = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth                = 1.0f;
    rasterizer.cullMode                 = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace                = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable          = VK_FALSE;
    rasterizer.depthBiasConstantFactor  = 0.0f; // Optional
    rasterizer.depthBiasClamp           = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor     = 0.0f; // Optional

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable   = enableTextureSampling ? VK_TRUE : VK_FALSE;
    multisampling.minSampleShading      = enableTextureSampling ? 0.2f :  VK_FALSE;
    multisampling.rasterizationSamples  = _msaaSamples;
    multisampling.minSampleShading      = 1.0f; // Optional
    multisampling.pSampleMask           = NULL; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable      = VK_FALSE; // Optional

    // Depth and stencil state
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType                  = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable        = VK_TRUE;
    depthStencil.depthWriteEnable       = VK_TRUE;
    depthStencil.depthCompareOp         = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable  = VK_FALSE;
    depthStencil.minDepthBounds         = 0.0f; // Optional
    depthStencil.maxDepthBounds         = 1.0f; // Optional
    depthStencil.stencilTestEnable      = VK_FALSE;
    depthStencil.front                  = {}; // Optional
    depthStencil.back                   = {}; // Optional

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable         = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable     = VK_FALSE;
    colorBlending.logicOp           = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount   = 1;
    colorBlending.pAttachments      = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    // Dynamic state // Optional
    // VkDynamicState dynamicStates[] = {
    //     VK_DYNAMIC_STATE_VIEWPORT,
    //     VK_DYNAMIC_STATE_SCISSOR
    // };

    // VkPipelineDynamicStateCreateInfo dynamicState{};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 2;
    // dynamicState.pDynamicStates = dynamicStates;

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount         = 1;
    pipelineLayoutInfo.pSetLayouts            = &_descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges    = NULL; // Optional

    if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, NULL, &_pipelineLayout) != VK_SUCCESS)
        throw ez::vulkan_failure("PipelineLayout creation", __FILE__, __LINE__);
    
    // Create Graphics Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount             = 2;
    pipelineInfo.pStages                = shaderStages;
    pipelineInfo.pVertexInputState      = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState    = &inputAssembly;
    pipelineInfo.pViewportState         = &viewportState;
    pipelineInfo.pRasterizationState    = &rasterizer;
    pipelineInfo.pMultisampleState      = &multisampling;
    pipelineInfo.pDepthStencilState     = &depthStencil;
    pipelineInfo.pColorBlendState       = &colorBlending;
    pipelineInfo.pDynamicState          = NULL; // Optional
    pipelineInfo.layout                 = _pipelineLayout;
    pipelineInfo.renderPass             = _renderPass;
    pipelineInfo.subpass                = 0;
    pipelineInfo.basePipelineHandle     = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex      = -1; // Optional

    if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &_graphicsPipeline) != VK_SUCCESS)
        throw ez::vulkan_failure("GraphicsPipeline creation", __FILE__, __LINE__);

    // Delete Shader Modules
    vkDestroyShaderModule(_device, fragShaderModule, NULL);
    vkDestroyShaderModule(_device, vertShaderModule, NULL);
}







void Renderer::createDepthResources()
{
    VkFormat depthFormat = findDepthFormat();
    createImage(_swapChainExtent.width, _swapChainExtent.height, 1, _msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory);
    _depthImageView = createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}







void Renderer::createFramebuffers()
{
    _swapChainFramebuffers.resize(_swapChainImageViews.size());

    for (size_t i = 0; i < _swapChainFramebuffers.size(); ++i)
    {
        std::array<VkImageView, 2> attachments = {
            _swapChainImageViews[i],
            _depthImageView
        };

        std::array<VkImageView, 3> MSattachments = {
            _colorImageView,
            _depthImageView,
            _swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = _renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(msaaLevel > 1 ? MSattachments.size() : attachments.size());
        framebufferInfo.pAttachments    = msaaLevel > 1 ? MSattachments.data() : attachments.data();
        framebufferInfo.width           = _swapChainExtent.width;
        framebufferInfo.height          = _swapChainExtent.height;
        framebufferInfo.layers          = 1;

        if (vkCreateFramebuffer(_device, &framebufferInfo, NULL, &_swapChainFramebuffers[i]) != VK_SUCCESS)
            throw ez::vulkan_failure("Framebuffer creation", __FILE__, __LINE__);
    }
}







void Renderer::createCommandPool(const Window& WS_handle)
{
    // Command Pools
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(_physicalDevice, WS_handle);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags            = 0; // Optional

    if (vkCreateCommandPool(_device, &poolInfo, NULL, &_commandPool) != VK_SUCCESS)
        throw ez::vulkan_failure("CommandPool creation", __FILE__, __LINE__);
}







void Renderer::createTextureImage(Texture& texture)
{
	LOG(EZ_TRACE, "|- Start creating TextureImage");

    //ResourceManager &_manager = Init::getPtr()->getResourceManager();
    //std::string binarypath = std::get<1>( _manager.saveResource(texture.path) );
    //ImageResource *res = (ImageResource *)_manager.loadResource(binarypath);

    // int texWidth, texHeight, texChannels;
    // stbi_uc* pixels = stbi_load(texture.path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    // texture.width = static_cast<uint32_t>(texWidth);
    // texture.height = static_cast<uint32_t>(texHeight);
    // texture.format = VK_FORMAT_R8G8B8A8_SRGB;

    //texture.width = res->getWidth();
    //texture.height = res->getHeight();
    //texture.format = VK_FORMAT_R8G8B8A8_SRGB;
    std::cout << texture.width << std::endl;
    std::cout << texture.height << std::endl;
    std::cout << texture.format << std::endl;
    std::cout << texture.width << std::endl;

    uint8_t* pixels = texture.pixels;

    VkDeviceSize imageSize = texture.width * texture.height * 4;
    
    texture.mipLevels = (std::floor(std::log2(std::max(texture.width, texture.height)))) + 1;

    if (!pixels)
        throw ez::vulkan_failure("to load TextureImage", __FILE__, __LINE__);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(_device, stagingBufferMemory);

    // stbi_image_free(pixels);

    createImage(texture.width, texture.height, texture.mipLevels, VK_SAMPLE_COUNT_1_BIT, texture.format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture.image, texture.deviceMemory);

    transitionImageLayout(texture.image, texture.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.mipLevels);
    copyBufferToImage(stagingBuffer, texture.image, texture.width, texture.height);
    // transitionImageLayout(texture.image, texture.format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.mipLevels);
    // transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);

    generateMipmaps(texture.image, texture.format, texture.width, texture.height, texture.mipLevels);

	LOG(EZ_TRACE, "|- Finished creating TextureImage");
}







void Renderer::createTextureImageView(Texture& texture)
{
    texture.imageView = createImageView(texture.image, texture.format, VK_IMAGE_ASPECT_COLOR_BIT, texture.mipLevels);
}







void Renderer::createTextureSampler(Texture& texture)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter               = VK_FILTER_LINEAR;
    samplerInfo.minFilter               = VK_FILTER_LINEAR;
    samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    // (isDeviceSuitable checks anisotropy)
    samplerInfo.anisotropyEnable        = enableAnisotropy ? VK_TRUE : VK_FALSE;
    samplerInfo.maxAnisotropy           = enableAnisotropy ? 16.0f : 1.0f;
    samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable           = VK_FALSE;
    samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod                  = 0.0f; // Optional
    samplerInfo.maxLod                  = static_cast<float>(texture.mipLevels);
    samplerInfo.mipLodBias              = 0.0f; // Optional

    if (vkCreateSampler(_device, &samplerInfo, NULL, &(texture.sampler)) != VK_SUCCESS)
        throw ez::vulkan_failure("TextureSampler creation", __FILE__, __LINE__);
}







void Renderer::createVertexBuffer()
{
    // buffer creation
    VkDeviceSize bufferSize = sizeof(_vertices[0]) * _vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, _vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
    copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);
}







void Renderer::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, _indices.data(), (size_t) bufferSize);
    vkUnmapMemory(_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);
    copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, NULL);
    vkFreeMemory(_device, stagingBufferMemory, NULL);
}








void Renderer::createDescriptorPool(std::vector<Mesh*> meshContainer)
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(meshContainer.size() * _swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(meshContainer.size() * _swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount  = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes     = poolSizes.data();
    poolInfo.maxSets        = static_cast<uint32_t>(poolSizes.size() * meshContainer.size() * _swapChainImages.size());
    poolInfo.flags          = 0; // Optional

    if (vkCreateDescriptorPool(_device, &poolInfo, NULL, &_descriptorPool) != VK_SUCCESS)
        throw ez::vulkan_failure("DescriptorPool creation", __FILE__, __LINE__);
}







void Renderer::createDescriptorSets(std::vector<Mesh*> meshContainer)
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool        = _descriptorPool;
    allocInfo.descriptorSetCount    = 1;
    allocInfo.pSetLayouts           = &_descriptorSetLayout;

    for (auto& mesh : meshContainer)
    {
        mesh->uniformBuffers.resize(_swapChainImages.size());

        for (size_t i = 0; i < mesh->uniformBuffers.size(); ++i)
        {
        // allocate DescriptorSet
            if (vkAllocateDescriptorSets(_device, &allocInfo, &(mesh->uniformBuffers[i].descriptorSet)) != VK_SUCCESS)
                throw ez::vulkan_failure("MeshDescriptorSets allocation", __FILE__, __LINE__);

        // allocate UniformBuffers
            createBuffer(
                sizeof(MVPObject),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                mesh->uniformBuffers[i].buffer, mesh->uniformBuffers[i].deviceMemory
            );

        // setup DescriptorBufferInfo
            mesh->uniformBuffers[i].descriptorBufferInfo.buffer   = mesh->uniformBuffers[i].buffer;
            mesh->uniformBuffers[i].descriptorBufferInfo.offset   = 0;
            mesh->uniformBuffers[i].descriptorBufferInfo.range    = sizeof(MVPObject);

        // create DescriptorWrites
            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
            descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet          = mesh->uniformBuffers[i].descriptorSet;
            descriptorWrites[0].dstBinding      = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo     = &(mesh->uniformBuffers[i].descriptorBufferInfo);

            descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet          = mesh->uniformBuffers[i].descriptorSet;
            descriptorWrites[1].dstBinding      = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo      = &(mesh->primitive.texture.descriptorImageInfo);

        // update DescriptorSet
            vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, NULL);
        }
    }
}







void Renderer::createCommandBuffers(std::vector<Mesh*> meshContainer)
{
    // Command buffer allocation
    _commandBuffers.resize(_swapChainImages.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = _commandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) _commandBuffers.size();

    if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
        throw ez::vulkan_failure("CommandBuffers allocation", __FILE__, __LINE__);

    // Starting command buffer recording
    for (size_t i = 0; i < _commandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags            = 0; // Optional
        beginInfo.pInheritanceInfo = NULL; // Optional

        if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
            throw ez::vulkan_failure("to begin recording CommandBuffer", __FILE__, __LINE__);

        // Starting a render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = _renderPass;
        renderPassInfo.framebuffer       = _swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        // (start recording)
        vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Basic drawing commands
        vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

            // Binding the vertex buffer
        VkBuffer vertexBuffers[] = {_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(_commandBuffers[i], _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        for (size_t j = 0; j < meshContainer.size(); ++j)
        {
            vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &(meshContainer[j]->uniformBuffers[i].descriptorSet), 0, NULL);
            vkCmdDrawIndexed(_commandBuffers[i], meshContainer[j]->primitive.indexCount, 1, meshContainer[j]->primitive.firstIndex, 0, 0);
        }

        // (stop recording)
        vkCmdEndRenderPass(_commandBuffers[i]);

        if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
            throw ez::vulkan_failure("CommandBuffer recording", __FILE__, __LINE__);
    }
}







void Renderer::createSyncObjects()
{
    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    _imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (vkCreateSemaphore(_device, &semaphoreInfo, NULL, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_device, &semaphoreInfo, NULL, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_device, &fenceInfo, NULL, &_inFlightFences[i]) != VK_SUCCESS)
            throw ez::vulkan_failure("SynchronizationObjects creation", __FILE__, __LINE__);
    }
}




















// #############################
// #   SETUP DEBUG MESSENGER   #
// #############################

void Renderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

std::vector<const char*> Renderer::getRequiredExtensions()
{
    std::vector<const char*> extensions;

    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    #if defined(EZ_OS_LINUX)
      extensions.push_back("VK_KHR_xlib_surface");
    #elif defined(EZ_OS_WINDOWS)
      extensions.push_back("VK_KHR_win32_surface");
    #endif
    extensions.push_back("VK_KHR_surface");
    return extensions;
}

bool Renderer::checkValidationLayerSupport()
{
    uint32_t layerCount = 0;
    if (vkEnumerateInstanceLayerProperties(&layerCount, NULL) != VK_SUCCESS)
        throw ez::vulkan_failure("InstanceLayerProperties enumeration", __FILE__, __LINE__);
    else if (layerCount == 0)
        throw ez::vulkan_failure("to find any InstanceLayerProperty", __FILE__, __LINE__);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    if (vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) != VK_SUCCESS)
        throw ez::vulkan_failure("InstanceLayerProperties enumeration", __FILE__, __LINE__);

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}










// ############################
// #   PICK PHYSICAL DEVICE   #
// ############################

bool Renderer::isDeviceSuitable(VkPhysicalDevice device, const Window& WS_handle)
{
    // printf("\tisDeviceSuitable\n");
    QueueFamilyIndices famIndices = findQueueFamilies(device, WS_handle);

    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;

    // VkPhysicalDeviceProperties deviceProperties;
    // VkPhysicalDeviceFeatures deviceFeatures;
    // vkGetPhysicalDeviceProperties(device, &deviceProperties);
    // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return famIndices.isComplete() && extensionsSupported && swapChainAdequate && (enableAnisotropy ? supportedFeatures.samplerAnisotropy : true);
}

bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount = 0;
    if (vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL) != VK_SUCCESS)
        throw ez::vulkan_failure("DeviceExtensionProperties enumeration", __FILE__, __LINE__);
    else if (extensionCount == 0)
        throw ez::vulkan_failure("to find any DeviceExtensionProperty", __FILE__, __LINE__);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    if (vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions.data()) != VK_SUCCESS)
        throw ez::vulkan_failure("DeviceExtensionProperties enumeration", __FILE__, __LINE__);

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}










// #############################
// #   CREATE LOGICAL DEVICE   #
// #############################

QueueFamilyIndices Renderer::findQueueFamilies(VkPhysicalDevice device, const Window& WS_handle)
{
    // printf("\t\tfindQueueFamilies\n");

    QueueFamilyIndices famIndices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    VkBool32 verif = VK_FALSE;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            famIndices.graphicsFamily = i;

#if defined(EZ_OS_LINUX)

        if (vkGetPhysicalDeviceXlibPresentationSupportKHR(device, i,
                            ((XWindow*)WS_handle)->GetDisplay(),
                            ((XWindow*)WS_handle)->GetVisualID()) == VK_TRUE)
            famIndices.presentFamily = i;

#elif defined(EZ_OS_WINDOWS)
        if (vkGetPhysicalDeviceWin32PresentationSupportKHR(device, i))
            famIndices.presentFamily = i;
#endif
        
        if (vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &verif) != VK_SUCCESS)
            throw ez::vulkan_failure("to get PhysicalDeviceSurfaceSupport", __FILE__, __LINE__);

        if (famIndices.isComplete() && verif == VK_TRUE)
            break;

        ++i;
    }

    return famIndices;
}










// #########################
// #   CREATE SWAP CHAIN   #
// #########################

SwapChainSupportDetails Renderer::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &(details.capabilities)) != VK_SUCCESS)
        throw ez::vulkan_failure("to get PhysicalDeviceSurfaceCapabilities", __FILE__, __LINE__);

    uint32_t formatCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, NULL) != VK_SUCCESS)
        throw ez::vulkan_failure("to get PhysicalDeviceSurfaceFormats", __FILE__, __LINE__);
    else
    {
        assert(formatCount > 0);
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
        assert(formatCount > 0);
    }

    uint32_t presentModeCount = 0;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, NULL) != VK_SUCCESS)
        throw ez::vulkan_failure("to get PhysicalDeviceSurfacePresentModes", __FILE__, __LINE__);
    else
    {
        assert(presentModeCount > 0);
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
        assert(presentModeCount > 0);
    }

    return details;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;

    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& WS_handle)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;
    else
    {
        WindowSize currentSize  = WS_handle.GetWindowSize();
        VkExtent2D actualExtent = { currentSize.width, currentSize.height };
        
        // printf("choosing swap extent : w=%i, h=%i\n", currentSize.width, currentSize.height);
        // actualExtent.width  = std::max(capabilities.minImageExtent.width,  std::min(capabilities.maxImageExtent.width,  actualExtent.width));
        // actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        // printf("returning swap extent : w=%i, h=%i\n", actualExtent.width, actualExtent.height);

        return actualExtent;
    }
}










// ################################
// #   CREATE GRAPHICS PIPELINE   #
// ################################

std::vector<char> Renderer::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw ez::vulkan_failure("to open file", __FILE__, __LINE__);
    
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    // printf("buffersize = %li\n", buffer.size());

    return buffer;
}

VkShaderModule Renderer::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_device, &createInfo, NULL, &shaderModule) != VK_SUCCESS)
        throw ez::vulkan_failure("ShaderModule creation", __FILE__, __LINE__);

    return shaderModule;
}










// ############################
// #   CREATE VERTEX BUFFER   #
// ############################

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    throw ez::vulkan_failure("to find suitable memory type", __FILE__, __LINE__);
}

void Renderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size        = size;
    bufferInfo.usage       = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_device, &bufferInfo, NULL, &buffer) != VK_SUCCESS)
        throw ez::vulkan_failure("Buffer creation", __FILE__, __LINE__);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_device, &allocInfo, NULL, &bufferMemory) != VK_SUCCESS)
        throw ez::vulkan_failure("BufferMemory allocation", __FILE__, __LINE__);

    vkBindBufferMemory(_device, buffer, bufferMemory, 0);
}

void Renderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer Renderer::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool           = _commandPool;
    allocInfo.commandBufferCount    = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_graphicsQueue);

    vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}










// ####################
// #   UNIFORM DATA   #
// ####################

void Renderer::updateUniformBuffer(uint32_t currentImage, std::vector<Mesh*> meshContainer)
{
    LOG(EZ_TRACE,"CURRENT IMAGE");
    std::cout << currentImage << std::endl;
     static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    std::cout << time << std::endl;

// make sure to map every uniformBuffer for each Mesh
    static std::vector<std::vector<bool>> isMapped;
    if (isMapped.empty())
    {
        isMapped.resize(_swapChainImages.size());
        for (size_t i = 0; i < _swapChainImages.size(); ++i)
            isMapped[i].resize(meshContainer.size(), false);
    }

    // std::cout << "time : " << time << std::endl;

    for (size_t i = 0; i < meshContainer.size(); ++i)
    {
    // These functions can be called way sooner than now 
    // They apply modelMatrix transformations to the Mesh
    // (translation, rotation, scale)
        // if (i % 2 == 0)
        //     meshContainer[i]->mvpObject.model = ez::rotate(makeIdentity<float, 4>(), ez::radians(0.0f) * time, Vector3f(0.0f, 0.0f, 1.0f));
        // else
        //     meshContainer[i]->mvpObject.model = ez::rotate(makeIdentity<float, 4>(), ez::radians(0.0f) * -time, Vector3f(0.0f, 0.0f, 1.0f));

    // -------
    // mvpObject should only exist once in a UNIQUE Camera object
    // this would allow us to translate objects
    // -------
    // update (mvpObject.view and mvpObject.proj) every frame to see the Mesh
        meshContainer[i]->mvpObject.view = ez::lookAt(Vector3f(2.0f, 2.0f, 2.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
        meshContainer[i]->mvpObject.proj = ez::perspective(ez::radians(45.0f), _swapChainExtent.width / (float) _swapChainExtent.height, 0.1f, 10.0f);
        meshContainer[i]->mvpObject.proj(1, 1) *= -1;

        if (isMapped[currentImage][i] == false)
        {
            isMapped[currentImage][i] = true;
            vkMapMemory(_device, meshContainer[i]->uniformBuffers[currentImage].deviceMemory, 0, sizeof(MVPObject), 0, &(meshContainer[i]->uniformBuffers[currentImage].mapped));
        }

        memcpy(meshContainer[i]->uniformBuffers[currentImage].mapped, &(meshContainer[i]->mvpObject), sizeof(MVPObject));
    }
}

void Renderer::loadMesh(Mesh* mesh)
{
    if (mesh->primitive.vertices.empty())
        throw ez::vulkan_failure("to loadMesh from empty vertex vector", __FILE__, __LINE__);
    else if (mesh->primitive.indices.empty())
        throw ez::vulkan_failure("to loadMesh from empty index vector", __FILE__, __LINE__);

    mesh->primitive.firstIndex =  _indices.size();
    mesh->primitive.firstVertex =  _vertices.size();
    mesh->primitive.indexCount =  0;
    mesh->primitive.vertexCount = 0;

    // std::unordered_map<Vertex, uint32_t, ez::details::VertexHasher> uniqueVertices {};

    for (const auto& vertex : mesh->primitive.vertices)
    {
        _vertices.push_back(vertex);
        ++mesh->primitive.vertexCount;

        // if (uniqueVertices.count(vertex) == 0) {
        //     uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
        //     _vertices.push_back(vertex);
        //     ++mesh->primitive.vertexCount;
        // }

        // _indices.push_back(uniqueVertices[vertex]);
        // ++mesh->primitive.indexCount;
    }

    for (const auto& index : mesh->primitive.indices)
    {
        if (index < mesh->primitive.vertices.size())
        {
            _indices.push_back(index + mesh->primitive.firstVertex);
            ++mesh->primitive.indexCount;
        }
        else
            throw std::logic_error("Primitive has index referencing unexisting vertex");
    }

    assert(mesh->primitive.indices.size() == mesh->primitive.indexCount);
    assert(mesh->primitive.vertices.size() == mesh->primitive.vertexCount);
}





// ####################
// #   CREATE IMAGE   #
// ####################

void Renderer::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType     = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width  = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth  = 1;
    imageInfo.mipLevels     = mipLevels;
    imageInfo.arrayLayers   = 1;
    imageInfo.format        = format;
    imageInfo.tiling        = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage         = usage;
    imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples       = numSamples;
    imageInfo.flags         = 0; // Optional

    if (vkCreateImage(_device, &imageInfo, NULL, &image) != VK_SUCCESS)
        throw ez::vulkan_failure("Image creation", __FILE__, __LINE__);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType             = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize    = memRequirements.size;
    allocInfo.memoryTypeIndex   = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_device, &allocInfo, NULL, &imageMemory) != VK_SUCCESS)
        throw ez::vulkan_failure("ImageMemory allocation", __FILE__, __LINE__);

    vkBindImageMemory(_device, image, imageMemory, 0);
}

void Renderer::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    VkImageMemoryBarrier barrier{};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = newLayout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;
    
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;

    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    
    } else
        throw ez::vulkan_failure(", unsupported layout transition", __FILE__, __LINE__);

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);
}

void Renderer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset      = 0;
    region.bufferRowLength   = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel        = 0;
    region.imageSubresource.baseArrayLayer  = 0;
    region.imageSubresource.layerCount      = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(commandBuffer);
}

VkImageView Renderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image      = image;
    viewInfo.viewType   = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format     = format;
    viewInfo.subresourceRange.aspectMask        = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel      = 0;
    viewInfo.subresourceRange.levelCount        = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer    = 0;
    viewInfo.subresourceRange.layerCount        = 1;

    VkImageView imageView;
    if (vkCreateImageView(_device, &viewInfo, NULL, &imageView) != VK_SUCCESS)
        throw ez::vulkan_failure("TextureImageView creation", __FILE__, __LINE__);

    return imageView;
}










// #######################
// #   DEPTH BUFFERING   #
// #######################

VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            return format;
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            return format;
    }

    throw ez::vulkan_failure("to find SupportedFormat", __FILE__, __LINE__);
}

VkFormat Renderer::findDepthFormat()
{
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool Renderer::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}










// #######################
// #   GENERATE MIPMAP   #
// #######################

void Renderer::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(_physicalDevice, imageFormat, &formatProperties);
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        throw ez::vulkan_failure(", texture image format does not support linear blitting", __FILE__, __LINE__);

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image                           = image;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;
    barrier.subresourceRange.levelCount     = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; ++i)
    {
        barrier.subresourceRange.baseMipLevel   = i - 1;
        barrier.oldLayout                       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout                       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask                   = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask                   = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, NULL,
            0, NULL,
            1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0]                  = { 0, 0, 0 };
        blit.srcOffsets[1]                  = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel        = i - 1;
        blit.srcSubresource.baseArrayLayer  = 0;
        blit.srcSubresource.layerCount      = 1;
        blit.dstOffsets[0]                  = { 0, 0, 0 };
        blit.dstOffsets[1]                  = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel        = i;
        blit.dstSubresource.baseArrayLayer  = 0;
        blit.dstSubresource.layerCount      = 1;

        vkCmdBlitImage(commandBuffer,
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);

        barrier.oldLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask   = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, NULL,
            0, NULL,
            1, &barrier);
        
        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, NULL,
        0, NULL,
        1, &barrier);

    endSingleTimeCommands(commandBuffer);
}










// #####################
// #   ANTI ALIASING   #
// #####################

VkSampleCountFlagBits Renderer::getMaxUsableSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    VkSampleCountFlagBits result = VK_SAMPLE_COUNT_1_BIT;

    if ((counts & VK_SAMPLE_COUNT_64_BIT) && msaaLevel >= 64)
    {
        msaaLevel = 64;
        result = VK_SAMPLE_COUNT_64_BIT;
    }
    else if ((counts & VK_SAMPLE_COUNT_32_BIT) && msaaLevel >= 32)
    {
        msaaLevel = 32;
        result = VK_SAMPLE_COUNT_32_BIT;
    }
    else if ((counts & VK_SAMPLE_COUNT_16_BIT) && msaaLevel >= 16)
    {
        msaaLevel = 16;
        result = VK_SAMPLE_COUNT_16_BIT;
    }
    else if ((counts & VK_SAMPLE_COUNT_8_BIT)  && msaaLevel >= 8)
    {
        msaaLevel = 8;
        result = VK_SAMPLE_COUNT_8_BIT;
    }
    else if ((counts & VK_SAMPLE_COUNT_4_BIT)  && msaaLevel >= 4)
    {
        msaaLevel = 4;
        result = VK_SAMPLE_COUNT_4_BIT;
    }
    else if ((counts & VK_SAMPLE_COUNT_2_BIT)  && msaaLevel >= 2)
    {
        msaaLevel = 2;
        result = VK_SAMPLE_COUNT_2_BIT;
    }
    else
        msaaLevel = 1;

    return result;
}

void Renderer::createColorResources()
{
    VkFormat colorFormat = _swapChainImageFormat;

    createImage(_swapChainExtent.width, _swapChainExtent.height, 1, _msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _colorImage, _colorImageMemory);
    _colorImageView = createImageView(_colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}