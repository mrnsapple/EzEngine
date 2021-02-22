/**
 * @file "Core/UniqueID/Private/UniqueID.cpp"
 *
 * @author Alexandre Chetafi
 */

#include "../Public/UniqueID.hpp"
#include "../Public/UniqueIDManager.hpp"
#include "manager_not_found.hpp"
#include <iostream>

namespace ez
{
	UniqueID::UniqueID()
	{
		if (!UniqueIDManager::getPtr())
			throw manager_not_found("Can not launch uniqueIDManager", __FILE__, __LINE__);
		UniqueIDManager::getPtr()->subscribe(this);
	}

	UniqueID::~UniqueID() noexcept
	{
		try {
			UniqueIDManager::getPtr()->unsubscribe(this);
		} catch (std::exception &e) {
			std::cerr << "The UniqueID Manager is not available anymore, but the ID object still try to unsubscribe" << std::endl;
			std::cerr << e.what() << std::endl;
		}
	}

	UniqueID::UniqueID(UniqueID &&other)
	{
		if (!UniqueIDManager::getPtr())
			throw manager_not_found("Can not find uniqueIDManager", __FILE__, __LINE__);
		UniqueIDManager::getPtr()->replace(*this, other);
		other.setInvalidity();
	}

	UniqueID & UniqueID::operator=(UniqueID &&other)
	{
		if (UniqueIDManager::getPtr())
			throw manager_not_found("Can not find uniqueIDManager", __FILE__, __LINE__);
		UniqueIDManager::getPtr()->replace(*this, other);
		other.setInvalidity();
		return *this;
	}

	bool UniqueID::isValid() const noexcept
	{
		return this->_valid;
	}

	unsigned long long UniqueID::getValue() const noexcept
	{
		return this->_id;
	}

	void UniqueID::setID(unsigned long long id) noexcept
	{
		this->_id = id;
	}

	const UniqueID *UniqueID::getThis() const noexcept
	{
		return this;
	}

	void UniqueID::setInvalidity() noexcept
	{
		this->_valid = false;
	}
}