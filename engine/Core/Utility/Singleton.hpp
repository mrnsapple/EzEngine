/**
 * @file "Singleton.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_SINGLETON_HPP
#define ENGINE_SINGLETON_HPP

#include "InstanceLimits.hpp"

namespace ez
{
	template <typename T>
	using Singleton = Instances<T, 1>;
}

#endif //ENGINE_SINGLETON_HPP
