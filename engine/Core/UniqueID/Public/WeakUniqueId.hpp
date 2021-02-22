//
// Created by phill on 06/11/2020.
//

#ifndef ENGINE_WEAKUNIQUEID_HPP
#define ENGINE_WEAKUNIQUEID_HPP

namespace ez
{
	class UniqueID;
	class WeakNodeID;

	class WeakUniqueID
	{
	public:
		WeakUniqueID() = delete;
		explicit WeakUniqueID(const UniqueID &);
		WeakUniqueID(const WeakUniqueID &);
		WeakUniqueID(WeakUniqueID &&) noexcept;

		WeakUniqueID	&operator=(const WeakUniqueID &) noexcept;
		WeakUniqueID	&operator=(WeakUniqueID &&) noexcept;

		~WeakUniqueID();

		bool 			isValid() noexcept;

		inline		explicit operator bool();
		inline bool	operator==(const UniqueID &) const;
		inline bool	operator!=(const UniqueID &) const;

	private:
		const WeakUniqueID	*getPtr() const noexcept;

	private:
		bool			_valid;
		WeakNodeID		*_node;
	};
}

#include "../Private/WeakUniqueID.tpp"

#endif //ENGINE_WEAKUNIQUEID_HPP
