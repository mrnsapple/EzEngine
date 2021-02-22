/**
 * @file "InstanceLimits.hpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_INSTANCELIMITS_HPP
#define ENGINE_INSTANCELIMITS_HPP

#include <atomic>
#include <stdexcept>

namespace ez
{
	template<typename T, std::size_t N>
	class Instances
	{
	public:
		Instances()
		{
			this->add_element();
		}

		Instances(Instances &)
		{
			this->add_element();
		}

		Instances	&operator=(Instances &)
		{
			this->add_element();
		}

		~Instances() noexcept
		{
			this->del_element();
		}

	private:
		void add_element()
		{
			_counter++;
			if (_counter.load(std::memory_order_relaxed) > N)
				throw std::logic_error("Can not create more instance of this class");
		}

		void del_element()
		{
			_counter--;
		}

		static std::atomic<int>		_counter;
	};

	template <typename T, std::size_t N>
	std::atomic<int> Instances<T, N>::_counter {0};
}

#endif //ENGINE_INSTANCELIMITS_HPP
