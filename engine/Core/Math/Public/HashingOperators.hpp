/*
** EPITECH PROJECT, 2020
** engine
** File description:
** HashingOperators
*/

namespace ez
{
	namespace details
	{
		inline void hash_combine(size_t& seed, size_t hash)
		{
			hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= hash;
		}

		template <typename T, std::size_t row, std::size_t col>
		struct MatrixHasher
		{
			std::size_t operator()(const ez::Matrix<T, row, col>& matrix) const
			{
				size_t seed = 0;
				std::hash<T> hasher;

				for (size_t i = 0; i < row; ++i)
					for (size_t j = 0; j < col; ++j)
						hash_combine(seed, hasher(matrix.get(static_cast<int>(i),static_cast<int>(j))));

				return seed;
			}
		};

		struct VertexHasher
		{
			std::size_t operator()(const ez::Vertex& vertex) const
			{
				return (
					(MatrixHasher<float, 3, 1>()(vertex.pos) ^
					(MatrixHasher<float, 3, 1>()(vertex.color) << 1)) >> 1) ^
					(MatrixHasher<float, 2, 1>()(vertex.texCoord) << 1
				);
			}
		};
	}
}

// namespace std
// {
// 	template <typename T, std::size_t row, std::size_t col>
// 	size_t hash<ez::Matrix<T, row, col>>::operator()(ez::Matrix<T, row, col> const& m) const
// 	{
// 		size_t seed = 0;
// 		hash<T> hasher;

// 		for (size_t i = 0; i < row; ++i)
// 			for (size_t j = 0; j < col; ++j)
// 				ez::details::hash_combine(seed, hasher(m.get(i, j)));

// 		return seed;
// 	}

// 	template <typename T>
// 	inline size_t hash<ez::Vector2<T>>::operator()(ez::Vector2<T> const& v) const
// 	{
// 		size_t seed = 0;
// 		hash<T> hasher;
// 		ez::details::hash_combine(seed, hasher(v.x));
// 		ez::details::hash_combine(seed, hasher(v.y));
// 		return seed;
// 	}

// 	template <typename T>
// 	inline size_t hash<ez::Vector3<T>>::operator()(ez::Vector3<T> const& v) const
// 	{
// 		size_t seed = 0;
// 		hash<T> hasher;
// 		ez::details::hash_combine(seed, hasher(v.x));
// 		ez::details::hash_combine(seed, hasher(v.y));
// 		ez::details::hash_combine(seed, hasher(v.z));
// 		return seed;
// 	}
// }