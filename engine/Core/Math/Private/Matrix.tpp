/**
 * @file "Core/Math/Private/Matrix.tpp"
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_MATRIX_TPP
#define ENGINE_MATRIX_TPP

#include "division_by_zero.hpp"
//#include "../../Exception/Public/out_of_bound.hpp"
#include <cstring>
#include <cmath>
#include <cassert>

namespace ez
{
	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>::Matrix() : _data()
	{
		if (col == 0 || row == 0)
			throw std::logic_error("The matrix can not be of size zero");
		std::memset(this->_data, 0, sizeof(T) * col * row);
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>::Matrix(const Matrix<T, row, col> &mat)
	{
		std::memcpy(this->_data, mat._data, sizeof(T) * col * row);
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>::Matrix(Matrix<T, row, col> &&mat) noexcept
	{
		std::memcpy(this->_data, mat._data, sizeof(T) * col * row);
		std::memset(mat._data, 0, sizeof(T) * col * row);
	}

	template <typename T, std::size_t row, std::size_t col>
	const T	&Matrix<T, row, col>::getData() const noexcept
	{
		return this->_data;
	}

	template <typename T, std::size_t row, std::size_t col>
	const void	*Matrix<T, row, col>::getRawData() const noexcept
	{
		return this->_data;
	}

	template <typename T, std::size_t row, std::size_t col>
	const T	&Matrix<T, row, col>::get(int line, int columns) const
	{
		//if (columns >= col || columns < 0 || line >= row || line < 0)
		//	throw ez::ouf_of_bound("d", __FILE__, __LINE__);
		return this->_data[line][columns];
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, col, row> Matrix<T, row, col>::transpose() const noexcept
	{
		Matrix<T, col, row> result;

		for (auto i = 0; i < row; i++) {
			for (auto j = 0; j < col; j++) {
				result(i, j) = this->get(j, i);
			}
		}
		return result;
	}

	template <typename T, std::size_t row, std::size_t col>
	bool Matrix<T, row, col>::is_inversible() const noexcept
	{
		if constexpr (row >= 4 || col >= 4 || col != row)
			return false;
		else
			return getDeterminant(*this);
	}

	template <typename T, std::size_t row, std::size_t col>
	T	&Matrix<T, row, col>::operator()(int line, int column)
	{
		//if (column >= col || column < 0 || line >= row || line < 0)
		//	throw ez::ouf_of_bound("d", __FILE__, __LINE__);
		return this->_data[line][column];
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	&Matrix<T, row, col>::operator+=(const Matrix<T, row, col> &mat)
	{
		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				this->_data[i][j] += mat.get(i, j);
			}
		}
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	&Matrix<T, row, col>::operator-=(const Matrix<T, row, col> &mat)
	{
		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				this->_data[j][i] -= mat.get(j, i);
			}
		}
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col> //TODO problem
	Matrix<T, row, col>	&Matrix<T, row, col>::operator*=(const Matrix<T, row, col> &mat)
	{
		Matrix<T, row, col>		_tmp;
		for (auto i = 0; i < row; i++) {
			for (auto j = 0; j < col; j++) {
				for (auto k = 0, l = 0; k < row && l < col; k++, l++) {
					_tmp(i,j) += this->get(i,l) * mat.get(k,j);
				}
			}
		}
		this->operator=(_tmp);
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	&Matrix<T, row, col>::operator*=(const T &element)
	{
		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				this->_data[j][i] *= element;
			}
		}
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	&Matrix<T, row, col>::operator/=(const T &elem)
	{
		if (!elem) {
			throw ez::division_by_zero("", __FILE__, __LINE__);
		}
		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				this->_data[j][i] /= elem;
			}
		}
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	&Matrix<T, row, col>::operator=(const Matrix<T, row, col> &mat)
	{
		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				this->_data[j][i] = mat.get(j, i);
			}
		}
		return *this;
	}

	template <typename T, std::size_t row, std::size_t col>
	bool	Matrix<T, row, col>::operator==(const Matrix<T, row, col> &mat) const
	{
		return !std::memcmp(this->getRawData(), mat.getRawData(), sizeof(T) * row * col);
	}

	template <typename T, std::size_t row, std::size_t col>
	bool	Matrix<T, row, col>::operator!=(const Matrix<T, row, col> &mat) const
	{
		return std::memcmp(this->getRawData(), mat.getRawData(), sizeof(T) * row * col);
	}

	/////////////////////////////////////
	////////// FREE FUNCTIONS ///////////
	/////////////////////////////////////

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	operator+(const Matrix<T, row, col> &lhs, const Matrix<T, row, col> &rhs)
	{
		Matrix<T, row, col>	res;

		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				res(j, i) = lhs.get(j, i) + rhs.get(j, i);
			}
		}
		return res;
	}

	template <typename T, std::size_t row, std::size_t col>
	Matrix<T, row, col>	operator-(const Matrix<T, row, col> &lhs, const Matrix<T, row, col> &rhs)
	{
		Matrix<T, row, col>	res;

		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				res(j, i) = lhs.get(j, i) - rhs.get(j, i);
			}
		}
		return res;
	}

	template <typename T, typename U, std::size_t row, std::size_t col>
	Matrix<U, row, col>	operator*(const Matrix<T, row, col> &lhs, const U &elem)
	{
		Matrix<U, row, col>	res;

		for (auto i = 0; i < col; i++) {
			for (auto j = 0; j < row; j++) {
				res(j, i) = lhs.get(j, i) * elem;
			}
		}
		return res;
	}

	template <typename T, typename U, std::size_t col, std::size_t row, std::size_t s>
	Matrix<U, row, col>	operator*(const Matrix<T, row, s> &lhs, const Matrix<U, s, col> &rhs)
	{
		Matrix<U, row, col>	res;

		for (auto i = 0; i < row; i++) {
			for (auto j = 0; j < col; j++) {
				for (auto k = 0, l = 0; k < row && l < col; k++, l++) {
					res(i,j) += lhs.get(i,l) * rhs.get(k,j);
				}
			}
		}
		return res;
	}

	template <typename T, typename U, std::size_t row, std::size_t col>
	Matrix<U, row, col>	operator/(const Matrix<T, row, col> &lhs, const U &elem)
	{
		if (elem == 0) {
			throw ez::division_by_zero("elem is not zero", __FILE__, __LINE__);
		}
		Matrix<U, row, col> res;

		for (auto i = 0; i < row; i++) {
			for (auto j = 0; j < col; j++) {
					res(i,j) = (U) lhs.get(i, j) / elem;
			}
		}
		return res;
	}

	template <typename T>
	inline T getDeterminant(const Matrix<T, 1, 1> &mat)
	{
		return mat.get(0, 0);
	}

	template <typename T>
	inline T getDeterminant(const Matrix<T, 2, 2> &mat)
	{
		return mat.get(0, 0) * mat.get(1, 1) - mat.get(0, 1) * mat.get(1, 0);
	}

	template <typename T>
	inline T getDeterminant(const Matrix<T, 3, 3> &mat)
	{
		Matrix<T, 2, 2>	subMatrix;
		T det = 0;
		auto sign = 1;

		for (auto x = 0; x < 3; x++) {
			int subj = 0;
			for (auto row = 1; row < 3; row++) {
				int subi = 0;
				for (auto col = 0; col < 3; col++) {
					if (col == x)
						continue;
					subMatrix(subi, subj) = mat.get(row, col);
					subi++;
				}
				subj++;
			}
			det += sign * mat.get(0, x) * getDeterminant(subMatrix);
			sign = -sign;
		}
		return det;
	}

	template <typename T>
	inline T getDeterminant(const Matrix<T, 4, 4> &mat)
	{
		Matrix<T, 3, 3>	subMatrix;
		T det = 0;
		auto sign = 1;

		for (auto x = 0; x < 4; x++) {
			int subj = 0;
			for (auto row = 1; row < 4; row++) {
				int subi = 0;
				for (auto col = 0; col < 4; col++) {
					if (col == x)
						continue;
					subMatrix(subi, subj) = mat.get(row, col);
					subi++;
				}
				subj++;
			}
			det += sign * mat.get(0, x) * getDeterminant(subMatrix);
			sign = -sign;
		}
		return det;
	}

	template <typename T>
	inline T				getDeterminant(const Matrix<T, 5, 5> &mat)
	{
		Matrix<T, 4, 4>	subMatrix;
		T det = 0;
		auto sign = 1;

		for (auto x = 0; x < 5; x++) {
			int subj = 0;
			for (auto row = 1; row < 5; row++) {
				int subi = 0;
				for (auto col = 0; col < 5; col++) {
					if (col == x)
						continue;
					subMatrix(subi, subj) = mat.get(row, col);
					subi++;
				}
				subj++;
			}
			det += sign * mat.get(0, x) * getDeterminant(subMatrix);
			sign = -sign;
		}
		return det;
	}

	template <typename T>
	Matrix<T, 1, 1>			getCofactor(const Matrix<T, 1, 1> &mat)
	{
		Matrix<T, 1, 1>	result;
		result(0,0) = 1;
		return result;
	}

	template <typename T>
	Matrix<T, 2, 2>			getCofactor(const Matrix<T, 2, 2> &mat)
	{
		Matrix<T, 2, 2>	result;
		result(0, 0) = mat.get(1, 1);
		result(0, 1) = - mat.get(1, 0);
		result(1, 0) = - mat.get(0, 1);
		result(1, 1) = mat.get(0, 0);
		return result;
	}

	template <typename T>
	Matrix<T, 3, 3>			getCofactor(const Matrix<T, 3, 3> &mat)
	{
		Matrix<T, 3, 3> result;

		result(0, 0) = mat.get(1, 1) * mat.get(2, 2) - mat.get(1, 2) * mat.get(2, 1);
		result(0, 1) = - mat.get(1, 0) * mat.get(2, 2) + mat.get(2, 0) * mat.get(1, 2);
		result(0, 2) = mat.get(1, 0) * mat.get(2, 1) - mat.get(2, 0) * mat.get(1, 1);

		result(1, 0) = - mat.get(0, 1) * mat.get(2, 2) + mat.get(2, 1) * mat.get(0, 2);
		result(1, 1) = mat.get(0, 0) * mat.get(2, 2) - mat.get(0, 2) * mat.get(2, 0);
		result(1, 2) = - mat.get(0, 0) * mat.get(2, 1) + mat.get(2, 0) * mat.get(0, 1);

		result(2, 0) = mat.get(0, 1) * mat.get(1, 2) - mat.get(1, 1) * mat.get(0, 2);
		result(2, 1) = - mat.get(0, 0) * mat.get(1, 2) + mat.get(1, 0) * mat.get(0, 2);
		result(2, 2) = mat.get(0, 0) * mat.get(1, 1) - mat.get(0, 1) * mat.get(1, 0);

		return result;
	}

	template <typename T>
	Matrix<T, 4, 4>			getCofactor(const Matrix<T, 4, 4> &mat)
	{
		Matrix<T, 4, 4>	result;

		//TODO
	}

	template <typename T>
	Matrix<double, 1, 1>			getInverse(const Matrix<T, 1, 1> &mat)
	{
		Matrix<double, 1, 1>	result;

		if (mat->get(0, 0) != 0) {
			result(0, 0) = 1 / mat.get(0, 0);
			return result;
		}
		throw ez::division_by_zero("", __FILE__, __LINE__);
	}

	template <typename T>
	Matrix<double, 2, 2>			getInverse(const Matrix<T, 2, 2> &mat)
	{
		T _det = getDeterminant(mat);
		if (_det != 0) {
			Matrix<double, 2, 2>	result;

			result(0, 0) = (double) mat.get(1, 1) /   (double) _det;
			result(1, 0) = (double) - mat.get(1, 0) / (double) _det;
			result(0, 1) = (double) - mat.get(0, 1) / (double) _det;
			result(1, 1) = (double) mat.get(0, 0) /   (double) _det;
			return result;
		}
		throw ez::division_by_zero("", __FILE__, __LINE__);
	}

	template <typename T>
	Matrix<double, 3, 3>			getInverse(const Matrix<T, 3, 3> &mat)
	{
		auto _det = getDeterminant(mat);
		auto transpose = getCofactor(mat).transpose();
		if (_det != 0) {
			Matrix<double, 3, 3> result;

			result(0,0) = (double) transpose.get(0, 0) / (double) _det;
			result(0, 1) = (double) transpose.get(0, 1) / (double) _det;
			result(0, 2) = (double) transpose.get(0, 2) / (double) _det;

			result(1, 0) = (double) transpose.get(1, 0) / (double) _det;
			result(1, 1) = (double) transpose.get(1, 1) / (double) _det;
			result(1, 2) = (double) transpose.get(1, 2) / (double) _det;

			result(2, 0) = (double) transpose.get(2, 0) / (double) _det;
			result(2, 1) = (double) transpose.get(2, 1) / (double) _det;
			result(2, 2) = (double) transpose.get(2, 2) / (double) _det;

			return result;
		}
		throw ez::division_by_zero("", __FILE__, __LINE__);
	}


	template <typename T>
	Matrix<T, 4, 4>			translate(const Matrix<T, 4, 4> &mat, const Matrix<T, 3, 1> &vec)
	{
		ez::Matrix<float, 4, 4> result(mat);
		result(3, 0) = mat.get(0, 0) * vec.get(0, 0) + mat.get(1, 0) * vec.get(1, 0) + mat.get(2, 0) * vec.get(2, 0) + mat.get(3, 0);
		return result;
	}

	template <typename T>
	Matrix<T, 4, 4>	rotate(const Matrix<T, 4, 4> &mat, T angle, const Matrix<T, 3, 1> &vec)
	{
		T const a = angle;
		T const c = cos(a);
		T const s = sin(a);

		ez::Matrix<float, 3, 1> axis(normalize(vec));
		ez::Matrix<float, 3, 1> temp(axis * (T(1) - c));

		ez::Matrix<float, 4, 4> rotate;
		rotate(0, 0) = c + temp(0, 0) * axis(0, 0);
		rotate(0, 1) = temp(0, 0) * axis(1, 0) + s * axis(2, 0);
		rotate(0, 2) = temp(0, 0) * axis(2, 0) - s * axis(1, 0);

		rotate(1, 0) = temp(1, 0) * axis(0, 0) - s * axis(2, 0);
		rotate(1, 1) = c + temp(1, 0) * axis(1, 0);
		rotate(1, 2) = temp(1, 0) * axis(2, 0) + s * axis(0, 0);

		rotate(2, 0) = temp(2, 0) * axis(0, 0) + s * axis(1, 0);
		rotate(2, 1) = temp(2, 0) * axis(1, 0) - s * axis(0, 0);
		rotate(2, 2) = c + temp(2, 0) * axis(2, 0);

		ez::Matrix<float, 4, 4> result;
		// result(0, 0) = mat.get(0, 0) * rotate(0, 0) + mat.get(1, 0) * rotate(0, 1) + mat.get(2, 0) * rotate(0, 2);
		// result(1, 0) = mat.get(0, 0) * rotate(1, 0) + mat.get(1, 0) * rotate(1, 1) + mat.get(2, 0) * rotate(1, 2);
		// result(2, 0) = mat.get(0, 0) * rotate(2, 0) + mat.get(1, 0) * rotate(2, 1) + mat.get(2, 0) * rotate(2, 2);
		// result(3, 0) = mat.get(3, 0);

		result = mat * rotate;
		result(3, 3) = 1.0f;

		return result;
	}

	template<typename T>
	Matrix<T, 4, 4>	lookAt(const Matrix<T, 3, 1> &eye, const Matrix<T, 3, 1> &center, const Matrix<T, 3, 1> &up)
	{
		Matrix<T, 3, 1> f(normalize(center - eye));
		Matrix<T, 3, 1> s(normalize(cross(up, f)));
		Matrix<T, 3, 1> u(cross(s, f));

		Matrix<T, 4, 4> result = makeIdentity<float, 4>();

		// RH
		// result(0, 0) = s(0, 0);
		// result(1, 0) = s(1, 0);
		// result(2, 0) = s(2, 0);
		// result(0, 1) = u(0, 0);
		// result(1, 1) = u(1, 0);
		// result(2, 1) = u(2, 0);
		// result(0, 2) =-f(0, 0);
		// result(1, 2) =-f(1, 0);
		// result(2, 2) =-f(2, 0);
		// result(3, 0) =-dot(s, eye);
		// result(3, 1) =-dot(u, eye);
		// result(3, 2) = dot(f, eye);

		// LH
		result(0, 0) = s(0, 0);
		result(1, 0) = s(1, 0);
		result(2, 0) = s(2, 0);
		result(0, 1) = -u(0, 0); // <===
		result(1, 1) = -u(1, 0); // <===
		result(2, 1) = -u(2, 0); // <===
		result(0, 2) = f(0, 0);
		result(1, 2) = f(1, 0);
		result(2, 2) = f(2, 0);
		result(3, 0) = -dot(s, eye);
		result(3, 1) = -dot(u, eye);
		result(3, 2) = -dot(f, eye);

		return result;
	}

	template<typename T>
	Matrix<T, 4, 4> 		perspective(T fovy, T aspect, T zNear, T zFar)
	{
		assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));
		T const tanHalfFovy = std::tan(fovy / static_cast<T>(2));
		
		Matrix<T, 4, 4> result;

		// RH_ZO
		// result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
		// result(1, 1) = static_cast<T>(1) / (tanHalfFovy);
		// result(2, 2) = zFar / (zNear - zFar);
		// result(2, 3) = - static_cast<T>(1);
		// result(3, 2) = -(zFar * zNear) / (zFar - zNear);

		// RH_NO
		// result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
		// result(1, 1) = static_cast<T>(1) / (tanHalfFovy);
		// result(2, 2) = - (zFar + zNear) / (zFar - zNear);
		// result(2, 3) = - static_cast<T>(1);
		// result(3, 2) = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

		// LH_ZO
		result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
		result(1, 1) = static_cast<T>(1) / (tanHalfFovy);
		result(2, 2) = zFar / (zFar - zNear);
		result(2, 3) = static_cast<T>(1);
		result(3, 2) = -(zFar * zNear) / (zFar - zNear);

		// LH_NO
		// result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
		// result(1, 1) = static_cast<T>(1) / (tanHalfFovy);
		// result(2, 2) = (zFar + zNear) / (zFar - zNear);
		// result(2, 3) = static_cast<T>(1);
		// result(3, 2) = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

		return result;
	}


	template <typename T>
	Matrix<T, 3, 1> 		normalize(const Matrix<T, 3, 1> &mat)
	{
		double mod = 0.0;

		for (int i = 0; i < 3; ++i)
			mod += mat.get(i, 0) * mat.get(i, 0);

		double mag = std::sqrt(mod);

		if (mag == 0)
			throw std::logic_error("The input vector is a zero vector");

		Matrix<T, 3, 1> result;
		for (int i = 0; i < 3; ++i)
			result(i, 0) = mat.get(i, 0) / mag;

		return result;
	}

	template <typename T>
	T 						dot(const Matrix<T, 3, 1> &vecA, const Matrix<T, 3, 1> &vecB)
	{
		T result = (T)0;

		for (int i = 0; i < 3; ++i)
			result += vecA.get(i, 0) * vecB.get(i, 0);

		return result;
	}

	template <typename T>
	Matrix<T, 3, 1> 		cross(const Matrix<T, 3, 1> &vecA, const Matrix<T, 3, 1> &vecB)
	{
		Matrix<T, 3, 1> result;

		result(0, 0) = vecA.get(1, 0) * vecB.get(2, 0) - vecA.get(2, 0) * vecB.get(1, 0);
		result(1, 0) = -(vecA.get(0, 0) * vecB.get(2, 0) - vecA.get(2, 0) * vecB.get(0, 0));
		result(2, 0) = vecA.get(0, 0) * vecB.get(1, 0) - vecA.get(1, 0) * vecB.get(0, 0);

		return result;
	}



	template <typename T, std::size_t s>
	inline Matrix<T, s, s>	makeIdentity()
	{
		Matrix<T, s, s>	mat;
		for (auto i = 0; i < s; i++) {
			mat(i, i) = 1;
		}
		return mat;
	}

	template <typename T>
	T		radians(const T& degrees)
	{
		return degrees * static_cast<T>(EZ_PI / 180.0f);
	}
}

#endif //ENGINE_MATRIX_TPP