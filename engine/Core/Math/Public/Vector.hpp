/**
 * @file "Core/Math/Private/Vector.tpp"
 *
 * @author Alexandre Chetafi
**/

#ifndef ENGINE_VECTOR_HPP
#define ENGINE_VECTOR_HPP

#include "Matrix.hpp"

namespace ez
{
	/**
	 * This class is a helper for vector with 3 positions
	 * @tparam T Any type able to do operator *, /, +, -
	 */

	template <typename T>
	class Vector3 : public Matrix<T, 3, 1>
	{
	public:
		Vector3();
		Vector3(const T &x, const T &y, const T &z);

		T	&x();
		T	&y();
		T	&z();
	};

	using Vector3i = Vector3<int>;
	using Vector3d = Vector3<double>;
	using Vector3f = Vector3<float>;
	using Vector3s = Vector3<short>;
	using Vector3c = Vector3<char>;
	using Vector3l = Vector3<long>;
	using Vector3ll = Vector3<long long>;

	/**
	 * This class is a helper for vector with 2 positions
	 * @tparam T Any type able to do operator *, /, +, -
	 */

	template <typename T>
	class Vector2 : public Matrix<T, 2, 1>
	{
	public:
		Vector2();
		Vector2(const T &x, const T &y);

		T	&x();
		T	&y();
	};

	using Vector2i = Vector2<int>;
	using Vector2d = Vector2<double>;
	using Vector2f = Vector2<float>;
	using Vector2s = Vector2<short>;
	using Vector2c = Vector2<char>;
	using Vector2l = Vector2<long>;
	using Vector2ll = Vector2<long long>;

	/**
	 * Rotate the matrix by the angle provided
	 * @tparam T The type of the matrix
	 * @param mat Matrix to be transformed
	 * @param angle the angle in degrees
	 */

	template <typename T>
	void			rotate(Vector2<T> &mat, double angle);

	/**
	 * Rotate the matrix by the angle provided on the x axis
	 * @tparam T The type of the matrix
	 * @param mat Matrix to be transformed
	 * @param angle the angle in degrees
	 */

	template <typename T>
	void 			rotatex(Vector3<T> &mat, double angle);

	/**
	 * Rotate the matrix by the angle provided on the y axis
	 * @tparam T The type of the matrix
	 * @param mat Matrix to be transformed
	 * @param angle the angle in degrees
	 */

	template <typename T>
	void 			rotatey(Vector3<T> &mat, double angle);

	/**
	 * Rotate the matrix by the angle provided on the z axis
	 * @tparam T The type of the matrix
	 * @param mat Matrix to be transformed
	 * @param angle the angle in degrees
	 */

	template <typename T>
	void 			rotatez(Vector3<T> &mat, double angle);
}

#include "../Private/Vector.tpp"

#endif //ENGINE_VECTOR_HPP
