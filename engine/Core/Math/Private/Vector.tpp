/**
 * @file "Core/Math/Private/Vector.tpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_VECTOR_TPP
#define ENGINE_VECTOR_TPP

#include <cmath>

namespace ez
{
	template <typename T>
	Vector2<T>::Vector2() : Matrix<T, 2, 1>()
	{
		this->x() = (T)0;
		this->y() = (T)0;
	}

	template <typename T>
	Vector2<T>::Vector2(const T &x, const T &y) : Matrix<T, 2, 1>()
	{
		this->x() = x;
		this->y() = y;
	}

	template <typename T>
	T & Vector2<T>::x()
	{
		return (*this)(0, 0);
	}

	template <typename T>
	T & Vector2<T>::y()
	{
		return (*this)(1, 0);
	}

	template <typename T>
	Vector3<T>::Vector3() : Matrix<T, 3, 1>()
	{
		this->x() = (T)0;
		this->y() = (T)0;
		this->z() = (T)0;
	}

	template <typename T>
	Vector3<T>::Vector3(const T &x, const T &y, const T &z) : Matrix<T, 3, 1>()
	{
		this->x() = x;
		this->y() = y;
		this->z() = z;
	}

	template <typename T>
	T & Vector3<T>::x()
	{
		return (*this)(0, 0);
	}

	template <typename T>
	T & Vector3<T>::y()
	{
		return (*this)(1, 0);
	}

	template <typename T>
	T & Vector3<T>::z()
	{
		return (*this)(2, 0);
	}

	template <typename T>
	void			rotate(Vector2<T> &mat, double angle)
	{
		if (angle == (double) 90) {
			auto x = mat.x();
			mat.x() = mat.y();
			mat.y() = -x;
		} else if (angle == (double) 180) {
			mat.x() = - mat.x();
			mat.y() = - mat.y();
		} else if (angle == (double) 270) {
			auto x = mat.x();
			mat.x() = - mat.y();
			mat.y() = x;
		} else {
			auto c = std::cos(angle);
			auto s = std::sin(angle);
			auto xx = mat(0, 0);
			auto yy = mat(1, 0);

			mat(0, 0) = xx * c - yy * s;
			mat(1, 0) = xx * s + yy * c;
		}
	}

	template <typename T>
	void 			rotatex(Vector3<T> &mat, double angle)
	{
		auto c = std::cos(angle);
		auto s = std::sin(angle);
		auto xx = mat(0, 0);
		auto yy = mat(1, 0);
		auto zz = mat(2, 0);

		mat(1, 0) = yy * c + zz * s;
		mat(2, 0) = - yy * s + zz * c;
	}

	template <typename T>
	void 			rotatey(Vector3<T> &mat, double angle)
	{
		auto c = std::cos(angle);
		auto s = std::sin(angle);
		auto xx = mat(0, 0);
		auto yy = mat(1, 0);
		auto zz = mat(2, 0);

		mat(0, 0) = xx * c - zz * s;
		mat(2, 0) = xx * s + zz * c;
	}

	template <typename T>
	void 			rotatez(Vector3<T> &mat, double angle)
	{
		auto c = std::cos(angle);
		auto s = std::sin(angle);
		auto xx = mat(0, 0);
		auto yy = mat(1, 0);
		auto zz = mat(2, 0);

		mat(0, 0) = xx * c + yy * s;
		mat(1, 0) = - xx * s + yy * c;
	}
}

#endif //ENGINE_VECTOR_TPP