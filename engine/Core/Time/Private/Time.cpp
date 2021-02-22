/**
 * @file "Core/Time/Private/Time.cpp"
 *
 * @author Alexandre Chetafi
**/

#include "Time.hpp"

namespace ez
{
	Time::Time() noexcept : _microseconds(0)
	{}

	Time::Time(Int64 milliseconds) noexcept : _microseconds(milliseconds)
	{}

	 Int64	Time::getMicroseconds() const noexcept
	{
		return this->_microseconds;
	}

	Int64 Time::getMilliseconds() const noexcept
	{
		return this->_microseconds / 1000;
	}

	Int32 Time::getSeconds() const noexcept
	{
		return static_cast<Int32>(this->_microseconds / 1000000);
	}

	 Time Time::seconds(Int32 seconds)
	{
		return Time(static_cast<Int64>(seconds * 1000000));
	}

	 Time Time::seconds(float seconds)
	{
		return Time(static_cast<Int64>(seconds * 10000000));
	}

	 Time Time::milliseconds(Int64 milliseconds)
	{
		return Time(milliseconds * 1000);
	}

	 Time Time::microseconds(Int64 microseconds)
	{
		return Time(microseconds);
	}

	Time	&Time::operator+=(Time other) noexcept
	{
		this->_microseconds += other.getMicroseconds();
		return *this;
	}

	Time	&Time::operator-=(Time other) noexcept
	{
		this->_microseconds -= other.getMicroseconds();
		return *this;
	}

	Time	&Time::operator*=(Int32 number) noexcept
	{
		this->_microseconds *= number;
		return *this;
	}

	Time	&Time::operator/=(Int32 number) noexcept
	{
		this->_microseconds /= number;
		return *this;
	}

	Time	&Time::operator/=(float number) noexcept
	{
		this->_microseconds /= number;
		return *this;
	}

	Time	&Time::operator*=(float number) noexcept
	{
		this->_microseconds *= number;
		return *this;
	}

	 bool	operator==(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() == rhs.getMicroseconds();
	}

	 bool	operator!=(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() != rhs.getMicroseconds();
	}

	 bool	operator<=(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() <= rhs.getMicroseconds();
	}

	 bool	operator>=(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() >= rhs.getMicroseconds();
	}

	 bool	operator<(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() < rhs.getMicroseconds();
	}

	 bool	operator>(const Time &lhs, const Time &rhs) noexcept
	{
		return lhs.getMicroseconds() > rhs.getMicroseconds();
	}

	 Time operator-(Time other) noexcept
	{
		return Time(- other.getMicroseconds());
	}

	 Time	operator+(Time lhs, Time rhs) noexcept
	{
		return Time(lhs.getMicroseconds() + rhs.getMicroseconds());
	}

	Time	operator-(Time lhs, Time rhs) noexcept
	{
		return Time(lhs.getMicroseconds() - rhs.getMicroseconds());
	}

	 Time	operator*(Time lhs, Int32 number) noexcept
	{
		return Time(lhs.getMicroseconds() * number);
	}

	 Time	operator*(Int32 number,Time rhs) noexcept
	{
		return Time(rhs.getMicroseconds() * number);
	}

	 Time	operator/(Time lhs, Int32 number) noexcept
	{
		return Time(lhs.getMicroseconds() / number);
	}

	 Time	operator/(Int32 number,Time rhs) noexcept
	{
		return Time(rhs.getMicroseconds() / number);
	}

#pragma warning(disable: 4244)

	 Time	operator*(Time lhs, float number) noexcept
	{
		return Time(lhs.getMicroseconds() * number);
	}

	 Time	operator/(Time lhs, float number) noexcept
	{
		return Time(lhs.getMicroseconds() / number);
	}

#pragma warning( default: 4244)
}