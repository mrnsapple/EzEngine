/**
 * @file "Audio/Public/Initialization/Initialization.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_INITIALIZATION_HPP
#define ENGINE_INITIALIZATION_HPP

namespace ez::Audio
{
	class Initialization
	{
	public:
		Initialization();
		~Initialization() noexcept;

		Initialization(Initialization &) = delete;
		Initialization	&operator=(Initialization &) = delete;
		Initialization(Initialization &&) noexcept;
		Initialization	&operator=(Initialization &&) noexcept;

	private:
		bool 	_can_be_destroyed;
	};
}

#endif //ENGINE_INITIALIZATION_HPP
