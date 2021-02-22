/**
 * @file "Core/Time/Public/Time.hpp"
 * A class to store time
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_TIME_HPP
#define ENGINE_TIME_HPP

#include "../../../config.hpp"

namespace ez
{
	class Time
	{
	public:
		Time() noexcept;
		Time(Int64 microseconds) noexcept;
		~Time() noexcept = default;

		inline Int64	getMicroseconds() const noexcept;
		Int64 			getMilliseconds() const noexcept;
		Int32			getSeconds() const noexcept;

		static Time		seconds(Int32 seconds);
		static Time		seconds(float seconds);
		static Time		milliseconds(Int64 milliseconds);
		static Time		microseconds(Int64 microseconds);

		Time			&operator+=(Time other) noexcept;
		Time			&operator-=(Time other) noexcept;
		Time			&operator*=(Int32 number) noexcept;
		Time			&operator/=(Int32 number) noexcept;
		Time			&operator*=(float number) noexcept;
		Time			&operator/=(float number) noexcept;

	private:
		Int64	_microseconds;
	};

	 bool	operator==(const Time &lhs, const Time &rhs) noexcept;
	 bool	operator!=(const Time &lhs, const Time &rhs) noexcept;
	 bool	operator<=(const Time &lhs, const Time &rhs) noexcept;
	 bool	operator>=(const Time &lhs, const Time &rhs) noexcept;
	 bool	operator<(const Time &lhs, const Time &rhs) noexcept;
	 bool	operator>(const Time &lhs, const Time &rhs) noexcept;

	 Time	operator-(Time other) noexcept;
	 Time	operator+(Time lhs, Time rhs) noexcept;
	 Time	operator-(Time lhs, Time rhs) noexcept;
	 Time	operator*(Time lhs, Int32 number) noexcept;
	 Time	operator*(Int32 number,Time rhs) noexcept;
	 Time	operator/(Time lhs, Int32 number) noexcept;
	 Time	operator/(Int32 number,Time rhs) noexcept;

	 Time	operator*(Time lhs, float number) noexcept;
	 Time	operator/(Time lhs, float number) noexcept;
}

#endif //ENGINE_TIME_HPP
