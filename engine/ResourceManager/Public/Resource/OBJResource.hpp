/**
 * @file "ResourceManager/Public/Resource/OBJResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_OBJRESSOURCE_HPP
#define ENGINE_OBJRESSOURCE_HPP

#include <string>
#include <deque>
#include <array>
#include <unordered_map>
#include <set>
#include <vector>
#include "Resource.hpp"
namespace ez
{
	namespace details
	{
		struct token
		{
			std::string command;
			std::deque<std::string> parameters;
		};
		static const std::set<std::string> internal {"v", "vp", "vn", "vt"};
		static const std::set<std::string>	composite {"f", "l"};
		static const std::set<std::string> external {"mtllib", "usemtl"};
		static const std::set<std::string> groups {"o", "s"};

		struct token_information
		{
			int 				index;
			std::string			command;
			std::deque<float>	parameters;
		};
	}

	class OBJResource : public Resource
	{
	public:
		struct info {
			std::vector<float>	vertices; 				// v(xyz)
			std::vector<float>	textureCoordinates; 	// vt(xy)
			std::vector<float>	normals;				// nv
		};

		/**
		 * Store the index to call, -1 if not use
		 */

		struct index {
			int vertexIndex;
			int normalIndex;
			int textureCoordinatesIndex;
		};

		/**
		 * Store information about the face
		 */

		struct face {
			std::vector<index> indices;
			int	num_sub_part;
		};

		/**
		 * Store all the vertex index to be look for
		 */

		struct line {
			std::vector<index> indices;
		};

		struct shape {
			std::vector<line>	lines;
			std::vector<face>	faces;
		};

	private:
		info				_infos;
		shape				_shape;

	public:
		bool UpdateResourceData(const ResourceData &) override { return false; }
		bool OnEvent(Event &e) override { return false; }

    	static Resource *Create(const ResourceData &data) { return new OBJResource(data); }
		OBJResource(const ResourceData &data);
		OBJResource(const std::string &name);

		const shape					&getShape() const noexcept;
		const info					&getInfo() const noexcept;

		static std::deque<details::token> 	parseFile(const std::string &filePath);
		static std::deque<details::token> 	parseFile(const ResourceData &data);

		static bool			lexFile(std::deque<details::token> &token, info &infos, shape &shapes);
	};
}

#endif //ENGINE_OBJRESSOURCE_HPP
