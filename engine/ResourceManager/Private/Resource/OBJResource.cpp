/**
 * @file "ResourceManager/Private/Resource/OBJResource.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../../Public/Resource/OBJResource.hpp"
#include "Resource/ResourceData.hpp"
#include <sstream>
#include <fstream>
#include <string>

namespace ez
{
	OBJResource::OBJResource(const ResourceData &data)
	{
		auto all = parseFile(data);
		lexFile(all, this->_infos, this->_shape);
	}

	OBJResource::OBJResource(const std::string &name)
	{
		auto all = parseFile(name);
		lexFile(all, this->_infos, this->_shape);
	}

	const OBJResource::info &OBJResource::getInfo() const noexcept
	{
		return this->_infos;
	}

	const OBJResource::shape& OBJResource::getShape() const noexcept
	{
		return this->_shape;
	}

	namespace {
		void eraseComment(std::string &s)
		{
			auto commentPosition = s.find('#');
			if (commentPosition != std::string::npos) {
				s.erase(commentPosition, std::string::npos);
			}
		}

		details::token createToken(const std::string &s)
		{
			details::token token;
			std::stringstream ss;
			ss << s;
			std::string result;
			ss >> token.command;
			while (!ss.eof()) {
				std::string _tmp;
				ss >> _tmp;
				token.parameters.emplace_back(_tmp);
			}
			return token;
		}
	}

	std::deque<details::token> OBJResource::parseFile(const std::string &filePath)
	{
		std::ifstream file(filePath);
		std::string 				line;
		std::deque<details::token>	result;

		if (!file)
			throw std::logic_error("Can not open the file");

		while (std::getline(file, line)) {
			eraseComment(line);
			auto token = createToken(line);
			if (token.parameters.empty())
				continue;
			result.emplace_back(token);
		}
		return result;
	}

	std::deque<details::token> OBJResource::parseFile(const ResourceData &data)
	{
		std::stringstream ss;
		std::string _file((const char *) data.getData(), data.getSize());
		ss << _file;
		std::deque<details::token>	result;
		std::string line;
		while (std::getline(ss, line)) {
			eraseComment(line);
			auto token = createToken(line);
			if (token.parameters.empty())
				continue;
			result.emplace_back(token);
		}
		return result;
	}

	namespace
	{
		struct vecfloat {
			float _[3] = {0, 0, 0};
		};

		vecfloat getFloat(const std::deque<std::string> &s) {
			vecfloat f_;
			int index = 0;
			for (auto &value : s) {
				if (index > 3)
					continue;
				f_._[index] = std::stof(value);
				index++;
			}
			return f_;
		}

		void addFromCommandToIndex(const std::string &command, OBJResource::info &infos, vecfloat &v) {
			if (command == "v") {
				infos.vertices.push_back(v._[0]);
				infos.vertices.push_back(v._[1]);
				infos.vertices.push_back(v._[2]);
			} else if (command == "vt") {
				infos.textureCoordinates.push_back(v._[0]);
				infos.textureCoordinates.push_back(v._[1]);
			} else if (command == "vn") {
				infos.normals.push_back(v._[0]);
				infos.normals.push_back(v._[1]);
				infos.normals.push_back(v._[2]);
			}
		}

		// enough is enough
		void ZEFUNCTIONFORAGREATCAUSE(OBJResource::face &face, const std::string &part) {
			std::stringstream ss;
			ss << part;
			int i = 0;
			for (; i < 3; i++) {
				OBJResource::index	index = {-1, -1, -1};
				std::string 		indexString;
				std::getline(ss, indexString, '/');
				if (!indexString.empty()) {
					switch (i) {
						case 0: index.vertexIndex = std::stoi(indexString) - 1; break;
						case 1: index.textureCoordinatesIndex = std::stoi(indexString) - 1; break;
						case 2: index.normalIndex = std::stoi(indexString) - 1; break;
						default: break;
					}
				}
				face.indices.push_back(index);
			}
			while (i < 3) {
				face.indices.push_back({-1, -1, -1});
				i++;
			}
		}

		void resolveComposite(const std::string &command, const std::deque<std::string> &parameters, OBJResource::shape &shapes) {
			if (command == "f") {
				OBJResource::face face;
				face.num_sub_part = 0;
				for (auto &elementSubPart : parameters) {
					ZEFUNCTIONFORAGREATCAUSE(face, elementSubPart);
					face.num_sub_part++;
				}
				shapes.faces.push_back(face);
			} else if (command == "l") {
				OBJResource::line line;
				for (auto &elementIndex : parameters) {
					line.indices.push_back({std::stoi(elementIndex), -1, -1});
				}
				shapes.lines.push_back(line);
			}
		}
	}

	bool OBJResource::lexFile(std::deque<details::token> &token, info &infos, shape &shapes)
	{
		for (auto &element : token) {
			if (details::internal.find(element.command) != details::internal.end()) {
				auto ff = getFloat(element.parameters);
				addFromCommandToIndex(element.command, infos, ff);
			} else if (details::composite.find(element.command) != details::composite.end()) {
				resolveComposite(element.command, element.parameters, shapes);
			}
		}
		return true;
	}
}