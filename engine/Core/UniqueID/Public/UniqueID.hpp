/**
 * @file "Core/UniqueID/Public/UniqueID.hpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_UNIQUEID_HPP
#define ENGINE_UNIQUEID_HPP

#include <memory>

namespace ez
{
	class UniqueIDManager;
	class WeakUniqueID;

	class UniqueID
	{
	public:
		friend class UniqueIDManager;
		friend class WeakUniqueID;
		UniqueID();
		~UniqueID() noexcept;

		UniqueID(UniqueID &&);
		UniqueID	&operator=(UniqueID &&);

		UniqueID	&operator=(UniqueID &) = delete;
		UniqueID(UniqueID &) = delete;

		bool inline operator==(const UniqueID &other) const noexcept;
		bool inline operator!=(const UniqueID &other) const noexcept;

		bool isValid() const noexcept;
		unsigned long long getValue() const noexcept;
	private:
		void setID(unsigned long long) noexcept;
		const UniqueID	*getThis() const noexcept;
		void setInvalidity() noexcept;

	private:
		unsigned long long					_id;
		bool 								_valid {true};
	};
}

#include "../Private/UniqueID.tpp"

#endif //ENGINE_UNIQUEID_HPP
