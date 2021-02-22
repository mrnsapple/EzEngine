//
// Created by phill on 06/11/2020.
//

#include "WeakNodeID.hpp"
#include "UniqueIDManager.hpp"

namespace ez //TODO check atomic
{
	WeakNodeID::WeakNodeID() : _count(0), _valid(true)
	{}

	void WeakNodeID::add()
	{
		this->_count.fetch_add(1, std::memory_order_relaxed);
	}

	void WeakNodeID::remove()
	{
		this->_count.fetch_sub(1, std::memory_order_relaxed);
		if (this->_valid && this->use_count() <= 0) {
			UniqueIDManager::getPtr()->deleteNode(this);
		}
	}

	bool WeakNodeID::compare(const UniqueID *id)
	{
		if (this->_valid)
			return UniqueIDManager::getPtr()->compare(this, id);
		return false;
	}

	int32_t WeakNodeID::use_count() const noexcept
	{
		return this->_count.load(std::memory_order_relaxed);
	}

	bool WeakNodeID::isValid() noexcept
	{
		if (this->_valid)
			this->_valid = UniqueIDManager::getPtr()->isWeakValid(this);
		return this->_valid;
	}

	void WeakNodeID::setInvalid() noexcept
	{
		this->_valid.store(false);
	}
}