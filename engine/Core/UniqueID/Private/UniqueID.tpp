/**
 * @file "UniqueID.tpp.cpp"
 *
 * @author Alexandre Chetafi
**/

namespace ez
{
	bool inline UniqueID::operator==(const UniqueID &other) const noexcept
	{
		return this->_id == other._id;
	}

	bool inline UniqueID::operator!=(const UniqueID &other) const noexcept
	{
		return this->_id != other._id;
	}
}