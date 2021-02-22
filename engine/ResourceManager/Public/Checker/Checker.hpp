/**
 * @file "ResourceManager/Public/Checker/Checker.hpp"
 *
 * @author Alexandre Chetafi
 */
#include "log.hpp"

#ifndef ENGINE_CHECKER_HPP
#define ENGINE_CHECKER_HPP

#include <string>

namespace ez
{
	class Checker
	{
	public:
		bool checkWavFile(const std::string &filePath) noexcept;
		bool checkOBJFile(const std::string &filePath) noexcept;
		bool checkIMGFile(const std::string &filePath) noexcept;

	};
}

#endif //ENGINE_CHECKER_HPP
