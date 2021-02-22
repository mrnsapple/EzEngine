//
// Created by phill on 06/11/2020.
//

#include "UniqueID.hpp"
#include "WeakUniqueId.hpp"
#include "UniqueIDManager.hpp"
#include "WeakNodeID.hpp"

namespace ez
{
	WeakUniqueID::WeakUniqueID(const UniqueID &id) : _node(UniqueIDManager::getPtr()->getNode(id.getThis())), _valid(true)
	{
		if (this->_node) {
			this->_node->add();
		} else {
			this->_valid = false;
		}
	}

	WeakUniqueID::WeakUniqueID(const WeakUniqueID &other) : _valid(other._node), _node(other._node)
	{
		if (!other._valid || !other._node)
			this->_valid = false;
		else {
			other._node->add();
		}
	}

	WeakUniqueID::WeakUniqueID(WeakUniqueID &&other) noexcept : _valid(other._valid), _node(other._node)
	{
		other._valid = false;
		other._node = nullptr;
	}

	WeakUniqueID &WeakUniqueID::operator=(const WeakUniqueID &other) noexcept
	{
		if (this != &other) {
			this->_node = other._node;
			this->_valid = other._valid;
			if (this->_node)
				this->_node->add();
		}
		return *this;
	}

	WeakUniqueID &WeakUniqueID::operator=(WeakUniqueID &&other) noexcept
	{
		this->_node = other._node;
		this->_valid = other._valid;
		other._valid = false;
		other._node = nullptr;
		return *this;
	}

	WeakUniqueID::~WeakUniqueID()
	{
		if (this->_node)
			this->_node->remove();
	}

	bool WeakUniqueID::isValid() noexcept
	{
		if (this->_valid) {
			this->_valid = this->_node->isValid();
		}
		return this->_valid;
	}

	const WeakUniqueID * WeakUniqueID::getPtr() const noexcept
	{
		return this;
	}
}