/*
** EPITECH PROJECT, 2020
** ezengine
** File description:
** VulkanIntegration
*/

#ifndef _EZ_ENGINE_RENDERER_HPP_
#define _EZ_ENGINE_RENDERER_HPP_

#include "../../../config.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <array>
#include <vulkan/vulkan.h>

#include "Model.hpp"
#include "HashingOperators.hpp"














namespace ez
{

// ############################################
// #   GLOBAL VARIABLES / MACRO DEFINITIONS   #
// ############################################

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }


    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };


    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
        // VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };


    class Window;


















// ######################
// #   RENDERER CLASS   #
// ######################

    class Renderer
    {
        // public:
        //     Texture myTexture;

        private:
            VkInstance                      _instance;
            VkApplicationInfo               _appInfo;

            VkDebugUtilsMessengerEXT        _debugMessenger;

            VkDevice                        _device;
            VkPhysicalDevice                _physicalDevice;
            VkQueue                         _graphicsQueue;
            VkQueue                         _presentQueue;

            VkSurfaceKHR                    _surface;
            VkSwapchainKHR                  _swapChain;
            std::vector<VkImage>            _swapChainImages;
            VkFormat                        _swapChainImageFormat;
            VkExtent2D                      _swapChainExtent;
            std::vector<VkImageView>        _swapChainImageViews;
            std::vector<VkFramebuffer>      _swapChainFramebuffers;

            VkRenderPass                    _renderPass;
            VkDescriptorSetLayout           _descriptorSetLayout;
            VkPipelineLayout                _pipelineLayout;
            VkPipeline                      _graphicsPipeline;

            VkCommandPool                   _commandPool;
            std::vector<VkCommandBuffer>    _commandBuffers;

            std::vector<VkSemaphore>        _imageAvailableSemaphores;
            std::vector<VkSemaphore>        _renderFinishedSemaphores;
            std::vector<VkFence>            _inFlightFences;
            std::vector<VkFence>            _imagesInFlight;
            size_t                          _currentFrame = 0;

            std::vector<Vertex>             _vertices;
            std::vector<uint32_t>           _indices;
            VkBuffer                        _vertexBuffer;
            VkDeviceMemory                  _vertexBufferMemory;
            VkBuffer                        _indexBuffer;
            VkDeviceMemory                  _indexBufferMemory;
            // std::vector<VkBuffer>           _uniformBuffers;
            // std::vector<VkDeviceMemory>     _uniformBuffersMemory;

            VkDescriptorPool                _descriptorPool;
            // std::vector<VkDescriptorSet>    _descriptorSets;

            // uint32_t                        _mipLevels;
            // VkImage                         _textureImage;
            // VkDeviceMemory                  _textureImageMemory;
            // VkImageView                     _textureImageView;
            // VkSampler                       _textureSampler;

            VkImage                         _depthImage;
            VkDeviceMemory                  _depthImageMemory;
            VkImageView                     _depthImageView;

            VkSampleCountFlagBits           _msaaSamples = VK_SAMPLE_COUNT_1_BIT;
            // MultiSample swapChain image needs to be refactored
            VkImage                         _colorImage;
            VkDeviceMemory                  _colorImageMemory;
            VkImageView                     _colorImageView;

        public:
            void Init               (const Window& WS_handle, std::vector<Mesh*> meshContainer);
            void Deinit             (std::vector<Mesh*> meshContainer);
            void CleanupSwapChain   (std::vector<Mesh*> meshContainer);
            void Cleanup            (std::vector<Mesh*> meshContainer);
            void DrawFrame          (const Window& WS_handle, std::vector<Mesh*> meshContainer);
            void RecreateSwapChain  (const Window& WS_handle, std::vector<Mesh*> meshContainer);

        private:
            // init
            void createInstance             ();
            void setupDebugMessenger        ();
            void createWindowSurface        (const Window& WS_handle);
            void pickphysicaldevice         (const Window& WS_handle);
            void createLogicalDevice        (const Window& WS_handle);
            void createSwapChain            (const Window& WS_handle);
            void createImageViews           ();
            void createRenderPass           ();
            void createDescriptorSetLayout  ();
            void createGraphicsPipeline     ();
            void createColorResources       ();
            void createDepthResources       ();
            void createFramebuffers         ();
            void createCommandPool          (const Window& WS_handle);
            void createTextureImage         (Texture& texture);
            void createTextureImageView     (Texture& texture);
            void createTextureSampler       (Texture& texture);
            // void loadMesh                   (Mesh& model);
            void createVertexBuffer         ();
            void createIndexBuffer          ();
            // void createUniformBuffers       (Mesh& model);
            void createDescriptorPool       (std::vector<Mesh*> meshContainer);
            void createDescriptorSets       (std::vector<Mesh*> meshContainer);
            void createCommandBuffers       (std::vector<Mesh*> meshContainer);
            void createSyncObjects          ();

        public:
            // Camera _myCam;




            // create instance
            bool                        checkValidationLayerSupport ();
            std::vector<const char*>    getRequiredExtensions       ();

            // setup debug messenger
            void                        populateDebugMessengerCreateInfo    (VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
            {
                std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
                return VK_FALSE;
            }

            // pick physical device
            bool                        isDeviceSuitable            (VkPhysicalDevice device, const Window& WS_handle);
            bool                        checkDeviceExtensionSupport (VkPhysicalDevice device);
            QueueFamilyIndices          findQueueFamilies           (VkPhysicalDevice device, const Window& WS_handle);
        
            // Swap Chain
            SwapChainSupportDetails     querySwapChainSupport       (VkPhysicalDevice device);
            VkSurfaceFormatKHR          chooseSwapSurfaceFormat     (const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR            chooseSwapPresentMode       (const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D                  chooseSwapExtent            (const VkSurfaceCapabilitiesKHR& capabilities, const Window& WS_handle);

            // graphics pipeline
            static std::vector<char>    readFile                    (const std::string& filename);
            VkShaderModule              createShaderModule          (const std::vector<char>& code);

            // create vertex buffer
            uint32_t                    findMemoryType              (uint32_t typeFilter, VkMemoryPropertyFlags properties);
            void                        createBuffer                (VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            void                        copyBuffer                  (VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
            VkCommandBuffer             beginSingleTimeCommands     ();
            void                        endSingleTimeCommands       (VkCommandBuffer commandBuffer);

            // uniform data
            void                        updateUniformBuffer         (uint32_t currentImage, std::vector<Mesh*> meshContainer);
            void                        loadMesh                    (Mesh* shape);

            // create image/textures
            void                        createImage                 (uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
            void                        transitionImageLayout       (VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
            void                        copyBufferToImage           (VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
            VkImageView                 createImageView             (VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

            // depth buffering
            VkFormat                    findSupportedFormat         (const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
            VkFormat                    findDepthFormat             ();
            bool                        hasStencilComponent         (VkFormat format);

            // generate mipmaps
            void                        generateMipmaps             (VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

            // anti aliasing
            VkSampleCountFlagBits       getMaxUsableSampleCount     ();
    };
}

#endif