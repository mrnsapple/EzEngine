
#include "WeakUniqueId.hpp"
#include "UniqueID.hpp"
#include "WeakNodeID.hpp"

namespace ez
{
	WeakUniqueID::operator bool()
	{
		return this->isValid();
	}

	bool WeakUniqueID::operator==(const UniqueID &id) const
	{
		return this->_valid ? this->_node->compare(id.getThis()) : false;
	}

	bool WeakUniqueID::operator!=(const UniqueID &id) const
	{
		return this->_valid ? !this->_node->compare(id.getThis()) : true;
	}

}