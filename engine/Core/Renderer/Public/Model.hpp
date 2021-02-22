/*
** EPITECH PROJECT, 2020
** engine
** File description:
** Model
*/

#pragma once

#include "../../../config.hpp"

#include <vulkan/vulkan.h>

#include "Vector.hpp"
#include "ImageResource.hpp"
#include "OBJResource.hpp"

namespace ez
{
	class Renderer;
	// struct shape;
	// struct info;

	struct MVPObject
	{
		Matrix<float, 4, 4> model;
		Matrix<float, 4, 4> view;
		Matrix<float, 4, 4> proj;
	};

	struct UniformBuffer
	{
		VkBuffer buffer;
		VkDeviceMemory deviceMemory;
		VkDescriptorBufferInfo descriptorBufferInfo;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		void* mapped;
	};

	struct Vertex
	{
		Vertex() {}
		Vertex(const std::vector<float>& thePosition, const std::vector<float>& theColor, const std::vector<float>& theTexCoord);

		ez::Vector3f pos;
		ez::Vector3f color;
		ez::Vector2f texCoord;

		void operator=(const Vertex& other)
		{
			pos = other.pos;
			color = other.color;
			texCoord = other.texCoord;
		}

		bool operator==(const Vertex& other) const
		{
			return (pos == other.pos) && (color == other.color) && (texCoord == other.texCoord);
		}

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding	= 0;
			attributeDescriptions[0].location   = 0;
			attributeDescriptions[0].format	 = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset	 = offsetof(Vertex, pos);

			attributeDescriptions[1].binding	= 0;
			attributeDescriptions[1].location   = 1;
			attributeDescriptions[1].format	 = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset	 = offsetof(Vertex, color);

			attributeDescriptions[2].binding	= 0;
			attributeDescriptions[2].location   = 2;
			attributeDescriptions[2].format	 = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset	 = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};

	struct Texture
	{
		uint8_t* pixels;
		VkImage					image;
		VkImageLayout			imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkDeviceMemory			deviceMemory;
		VkImageView				imageView;
		VkFormat				format;
		VkDescriptorImageInfo	descriptorImageInfo;
		VkSampler				sampler;
		uint32_t				width, height;
		uint32_t				mipLevels;

		void updateDescriptor();
		// void destroy();
		void loadImage(const ImageResource* const image);
	};

	struct Primitive
	{
		uint32_t firstIndex;
		uint32_t indexCount;
		uint32_t firstVertex;
		uint32_t vertexCount;
		// Material& material;
		Texture texture;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		void loadShape(const ez::OBJResource::info& theInfo, const ez::OBJResource::shape& theShape);
		// struct Dimensions {
		// 	glm::vec3 min = glm::vec3(FLT_MAX);
		// 	glm::vec3 max = glm::vec3(-FLT_MAX);
		// 	glm::vec3 size;
		// 	glm::vec3 center;
		// 	float radius;
		// } dimensions;

		// void setDimensions(glm::vec3 min, glm::vec3 max);
		// Primitive(uint32_t firstIndex, uint32_t indexCount, Material& material) : firstIndex(firstIndex), indexCount(indexCount), material(material) {};
	};

	struct Transform
	{
		ez::Vector3f position;
		ez::Vector3f rotation;
		ez::Vector3f scale;
	};

	// struct UniformBuffer
	// {
	// 	VkBuffer buffer;
	// 	VkDeviceMemory deviceMemory;
	// 	VkDescriptorBufferInfo descriptorBufferInfo;
	// 	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	// 	void* mapped;
	// };

	struct Mesh
	{
		std::string name;

		Primitive primitive;
		std::vector<UniformBuffer> uniformBuffers;

		// Transform transform;
		MVPObject mvpObject;

		void init(const std::string& modelName);
		void updateDescriptors();
	};

	struct Square : public Mesh
	{
		Square() : Mesh()
		{
			this->primitive.vertices = {
                {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            };

            this->primitive.indices = {
                0, 1, 2, 2, 3, 0,
            };
		}
	};

	struct Cube : public Mesh
	{
		Cube() : Mesh()
		{
			this->primitive.vertices = {
                // top
                {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 0
                {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 1
                {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // 2
                {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // 3

                // bottom
                {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.1f}, {0.0f, 0.0f}}, // 4
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.1f}, {1.0f, 0.0f}}, // 5
                {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.1f}, {1.0f, 1.0f}}, // 6
                {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.1f}, {0.0f, 1.0f}}, // 7

                // back
                {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.2f}, {0.0f, 0.0f}}, // 8
                {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.2f}, {1.0f, 0.0f}}, // 9
                {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.2f}, {1.0f, 1.0f}}, // 10
                {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.2f}, {0.0f, 1.0f}}, // 11

                // front
                {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.3f}, {0.0f, 0.0f}}, // 12
                {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.3f}, {1.0f, 0.0f}}, // 13
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.3f}, {1.0f, 1.0f}}, // 14
                {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.3f}, {0.0f, 1.0f}}, // 15

                // left
                {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.4f}, {0.0f, 0.0f}}, // 16
                {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.4f}, {1.0f, 0.0f}}, // 17
                {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.4f}, {1.0f, 1.0f}}, // 18
                {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.4f}, {0.0f, 1.0f}}, // 19

                // right
                {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.5f}, {0.0f, 0.0f}}, // 20
                {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.5f}, {1.0f, 0.0f}}, // 21
                {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}, {1.0f, 1.0f}}, // 22
                {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.5f}, {0.0f, 1.0f}}, // 23
            };

            this->primitive.indices = {
                0, 1, 2, 2, 3, 0,		// top
                4, 5, 6, 6, 7, 4,		// bottom
                8, 9, 10, 10, 11, 8,	// back
				12, 13, 14, 14, 15, 12,	// front
				16, 17, 18, 18, 19, 16,	// left
				20, 21, 22, 22, 23, 20,	// right
            };
		}
	};

	// struct Mesh
	// {
	// 	// vks::VulkanDevice* device;

	// 	// std::vector<Primitive*> primitives;
	// 	// std::string name;

	// 	// struct UniformBuffer {
	// 	// 	VkBuffer buffer;
	// 	// 	VkDeviceMemory memory;
	// 	// 	VkDescriptorBufferInfo descriptor;
	// 	// 	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	// 	// 	void* mapped;
	// 	// } uniformBuffer;

	// 	// struct UniformBlock {
	// 	// 	glm::mat4 matrix;
	// 	// 	glm::mat4 jointMatrix[64]{};
	// 	// 	float jointcount{ 0 };
	// 	// } uniformBlock;

	// 	// Mesh(vks::VulkanDevice* device, glm::mat4 matrix);
	// 	// ~Mesh();
	// };

	struct Camera
	{
		Transform transform;
		MVPObject mvpObject;
		std::vector<UniformBuffer> uniformBuffers;
	};
}
