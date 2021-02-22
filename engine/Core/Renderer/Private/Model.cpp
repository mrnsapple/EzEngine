/*
** EPITECH PROJECT, 2020
** engine
** File description:
** Model
*/

// Load image files
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

#include "renderer_exceptions.hpp"

#include "Settings.hpp"
#include "Renderer.hpp"
#include "Model.hpp"

// Load obj files
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>




// ###############
// #   TEXTURE   #
// ###############

ez::Vertex::Vertex(const std::vector<float>& thePosition, const std::vector<float>& theColor, const std::vector<float>& theTexCoord)
{
    if (thePosition.size() == 3)
    {
        this->pos.x() = thePosition[0];
        this->pos.y() = thePosition[1];
        this->pos.z() = thePosition[2];
    }
    if (theColor.size() == 3)
    {
        this->color.x() = theColor[0];
        this->color.y() = theColor[1];
        this->color.z() = theColor[2];
    }
    if (theTexCoord.size() == 2)
    {
        this->texCoord.x() = theTexCoord[0];
        this->texCoord.y() = theTexCoord[1];
    }
}










// ###############
// #   TEXTURE   #
// ###############

void ez::Texture::updateDescriptor()
{
	descriptorImageInfo.imageLayout = imageLayout;
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.sampler = sampler;
}

// void ez::Texture::destroy()
// {
// 	vkDestroyImageView(*device, imageView, NULL);
// 	vkDestroyImage(*device, image, NULL);
// 	vkFreeMemory(*device, deviceMemory, NULL);
// 	vkDestroySampler(*device, sampler, NULL);
// }

void ez::Texture::loadImage(const ImageResource* const image)
{
    width = image->getWidth();
    format = VK_FORMAT_R8G8B8A8_SRGB;
    height = image->getHeight();
    pixels =image->getPixel();
}










// #################
// #   PRIMITIVE   #
// #################

void ez::Primitive::loadShape(const ez::OBJResource::info& theInfo, const ez::OBJResource::shape& theShape)
{
    std::unordered_map<Vertex, uint32_t, ez::details::VertexHasher> uniqueVertices {};

	for (const auto& face : theShape.faces)
	{
		for (auto index = 0; index < face.num_sub_part * 3; index += 3)
		{
			ez::Vertex vertex;

			vertex.pos = {
					theInfo.vertices[3 * face.indices[index].vertexIndex + 0],
					theInfo.vertices[3 * face.indices[index].vertexIndex + 1],
					theInfo.vertices[3 * face.indices[index].vertexIndex + 2]
			};

			vertex.texCoord = {
					theInfo.textureCoordinates[2 * face.indices[index + 1].textureCoordinatesIndex + 0],
					1.0f - theInfo.textureCoordinates[2 * face.indices[index + 1].textureCoordinatesIndex + 1]
			};

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
                ++(vertexCount);
            }

            indices.push_back(uniqueVertices[vertex]);
            ++(indexCount);
        }
    }
}









// ############
// #   MESH   #
// ############

void ez::Mesh::init(const std::string& modelName)
{
    name = modelName;
}

void ez::Mesh::updateDescriptors()
{
    for (size_t i = 0; i < uniformBuffers.size(); ++i)
    {
        uniformBuffers[i].descriptorBufferInfo.buffer   = uniformBuffers[i].buffer;
        uniformBuffers[i].descriptorBufferInfo.offset   = 0;
        uniformBuffers[i].descriptorBufferInfo.range    = sizeof(MVPObject);
    }
}