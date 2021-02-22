//
// Created by phill on 06/11/2020.
//

#ifndef ENGINE_WEAKNODEID_HPP
#define ENGINE_WEAKNODEID_HPP

#include <atomic>

namespace ez
{
	class UniqueID;
	class WeakUniqueID;
}

namespace ez
{
	class WeakNodeID
	{
	public:
		friend class WeakUniqueID;
		friend class UniqueIDManager;
		WeakNodeID();
		WeakNodeID(WeakNodeID &) = delete;

		WeakNodeID	&operator=(WeakNodeID &) = delete;

	private:
		void	add();
		void 	remove();
		bool	compare(const UniqueID *);
		bool	isValid() noexcept;

		int32_t	use_count() const noexcept;

		void 	setInvalid() noexcept;

	private:
		std::atomic<bool>		_valid;
		std::atomic<int32_t>	_count;
	};
}

#endif //ENGINE_WEAKNODEID_HPP
