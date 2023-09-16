#ifndef STATIC_MATRIXES_H
#define STATIC_MATRIXES_H

#include <array>

#include "matrixes/matrix_concepts.h"

namespace MxLib
{
	namespace impl
	{
		template<typename FirstT, typename ...Args>
		struct ArraysListTraits
		{
			inline static const std::size_t Count{FirstT{}.size()};
			using Type = FirstT::value_type;
		};
	}

	template<std::size_t rows, std::size_t cols, typename ContainedT>
	class SMatrix
	{
	public:
		using contained = ContainedT;

		constexpr SMatrix() = default;
		constexpr ~SMatrix() = default;

		constexpr SMatrix(const SMatrix &matrixToCopy) = default;
		constexpr SMatrix &operator=(const SMatrix &matrixToCopy) = default;

		constexpr SMatrix(SMatrix &&matrixToCopy) noexcept = default;
		constexpr SMatrix &operator=(SMatrix &&matrixToCopy) noexcept = default;


		constexpr SMatrix(const std::initializer_list<std::initializer_list<ContainedT>> &list)
		{
			for (std::size_t row = 0; row < rows; ++row)
			{
				for (std::size_t col = 0; col < cols; ++col)
				{
					m_values[CalculatePos(row, col)] = list.begin()[row].begin()[col];
				}
			}
		}

		template<typename  ...Args>
			requires (std::is_same_v<Args, std::array<ContainedT, cols>> || ...)
		constexpr explicit SMatrix(Args... list)
		{
			std::size_t row = 0;
			([&] ()
			{
				for (std::size_t col = 0; col < cols; ++col)
				{
					m_values[CalculatePos(row, col)] = list[col];
				}
				++row;
			}(), ...);
		}

		constexpr explicit SMatrix(const ContainedT *data)
		{
			for(std::size_t row = 0; row < rows; ++row)
			{
				for(std::size_t col = 0; col < rows; ++col)
				{
					const std::size_t pos = CalculatePos(row, col); 
					m_values[pos] = data[pos];
				}
			}
		}

		constexpr explicit SMatrix(const ContainedT **data)
		{
			for(std::size_t row = 0; row < rows; ++row)
			{
				for(std::size_t col = 0; col < rows; ++col)
				{
					const std::size_t pos = CalculatePos(row, col); 
					m_values[pos] = data[row][col];
				}
			}
		}

		[[nodiscard]] constexpr inline std::size_t Cols() const noexcept { return cols; }
		[[nodiscard]] constexpr inline std::size_t Rows() const noexcept { return rows; }

		[[nodiscard]] constexpr inline ContainedT *data()
		{
			return m_values;
		}
		[[nodiscard]] constexpr inline const ContainedT *data() const
		{
			return m_values;
		}

		[[nodiscard]] constexpr inline std::size_t size() const
		{
			return m_values.size();
		}

		[[nodiscard]] constexpr inline const ContainedT &operator()(std::size_t row, std::size_t col) const
		{
			CheckBounds(*this, row, col);
			return m_values[CalculatePos(row, col)];
		}

		[[nodiscard]] constexpr inline ContainedT &operator()(std::size_t row, std::size_t col)
		{
			CheckBounds(*this, row, col);
			return m_values[CalculatePos(row, col)];
		}
	
	private:
		[[nodiscard]] constexpr inline std::size_t CalculatePos(std::size_t row, std::size_t col) const noexcept
		{
			return row * cols + col;
		}

	private:
		std::array<ContainedT, rows * cols> m_values;
	};

	template<typename ...Args>
		requires (std::is_same_v<Args, std::array<typename impl::ArraysListTraits<Args...>::Type, impl::ArraysListTraits<Args...>::Count>> || ...)
	SMatrix(Args...) -> SMatrix<sizeof...(Args), impl::ArraysListTraits<Args...>::Count, typename impl::ArraysListTraits<Args...>::Type>;

	template<typename ContainedT, std::size_t cols>
	SMatrix(std::array<ContainedT, cols>) -> SMatrix<1, cols, ContainedT>;

	using Matrix2f = SMatrix<2, 2, float>;
	using Matrix2d = SMatrix<2, 2, double>;

	using Matrix3f = SMatrix<3, 3, float>;
	using Matrix3d = SMatrix<3, 3, double>;

	using Matrix4f = SMatrix<4, 4, float>;
	using Matrix4d = SMatrix<4, 4, double>;

	static_assert(ContinuousStorageMatrix<Matrix2f>);
	static_assert(ContinuousStorageMatrix<Matrix2d>);

	static_assert(ContinuousStorageMatrix<Matrix3f>);
	static_assert(ContinuousStorageMatrix<Matrix3d>);

	static_assert(ContinuousStorageMatrix<Matrix4f>);
	static_assert(ContinuousStorageMatrix<Matrix4d>);
}

#endif // STATIC_MATRIXES_H
