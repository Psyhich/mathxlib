#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <stdexcept>
#include <array>


#include <fmt/format.h>

#include "matrix_concepts.h"
#include "operations_deduction.h"

namespace MxLib
{
	template<typename ContainedT>
	class Matrix
	{
	public:
		using contained = ContainedT;

		Matrix() noexcept = default;

		Matrix(const std::initializer_list<std::initializer_list<ContainedT>> &initializer) :
			m_cols{initializer.size() > 0 ? initializer.begin()[0].size() : 0},
			m_rows{initializer.size()},
			m_size{m_rows * m_cols},
			m_values{m_size > 0 ? new ContainedT[m_size] : nullptr}
		{
			for(std::size_t rowIndex = 0; rowIndex < m_rows; rowIndex++)
			{
				if (initializer.begin()[rowIndex].size() != m_cols)
				{
					throw std::out_of_range{"Matrix contains inconsistent amount of columns"};
				}

				for (std::size_t colIndex = 0; colIndex < m_cols; colIndex++)
				{
					(*this)(rowIndex, colIndex) = initializer.begin()[rowIndex].begin()[colIndex];
				}
			}
		}
		Matrix(std::size_t rows, std::size_t cols) :
			m_cols{cols},
			m_rows{rows},
			m_size{rows * cols},
			m_values{new ContainedT[rows * cols]}
		{
		}

		explicit Matrix(std::size_t dimensions) :
			m_cols{dimensions},
			m_rows{dimensions},
			m_size{dimensions * dimensions},
			m_values{new ContainedT[m_size]}
		{}

		template<ReadonlyMatrixT M>
			requires std::convertible_to<typename M::contained, ContainedT>
		explicit Matrix(const M &matrixToCopy) :
			m_cols{matrixToCopy.Cols()},
			m_rows{matrixToCopy.Rows()},
			m_size{m_cols * m_rows},
			m_values{m_size > 0 ? new ContainedT[m_size] : nullptr}
		{
			for (std::size_t row = 0; row < m_rows; ++row)
			{
				for (std::size_t col = 0; col < m_cols; ++col)
				{
					(*this)(row, col) = static_cast<ContainedT>(matrixToCopy(row, col));
				}
			}
		}

		template<ReadonlyMatrixT M>
			requires std::convertible_to<typename M::contained, ContainedT>
		Matrix &operator=(const M &matrixToCopy)
		{
			if(this != &matrixToCopy)
			{
				delete[] m_values;

				CopyData(matrixToCopy);
			}

			return *this;
		}


		Matrix(const ContainedT *data, std::size_t rows, std::size_t cols) :
			Matrix{rows, cols}
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

		Matrix(const ContainedT **data, std::size_t rows, std::size_t cols) :
			Matrix{rows, cols}
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

		Matrix(const Matrix &matrixToCopy)
		{
			CopyData(matrixToCopy);
		}

		Matrix &operator=(const Matrix &matrixToCopy)
		{
			if(this != &matrixToCopy)
			{
				delete[] m_values;

				CopyData(matrixToCopy);
			}

			return *this;
		}

		Matrix(Matrix &&matrixToMove) noexcept
		{
			MoveData(std::move(matrixToMove));
		}

		Matrix &operator=(Matrix &&matrixToMove) noexcept
		{
			if (this != &matrixToMove)
			{
				MoveData(std::move(matrixToMove));
			}
			return *this;
		}

		~Matrix() noexcept
		{
			delete[](m_values);
		}

		// Getters and setters
		[[nodiscard]] constexpr inline std::size_t Cols() const noexcept { return m_cols; }
		[[nodiscard]] constexpr inline std::size_t Rows() const noexcept { return m_rows; }

		constexpr inline const ContainedT &operator()(std::size_t row, std::size_t col) const
		{
			CheckBounds(*this, row, col);
			return m_values[CalculatePos(row, col)];
		}
		constexpr inline ContainedT &operator()(std::size_t row, std::size_t col)
		{
			CheckBounds(*this, row, col);
			return m_values[CalculatePos(row, col)];
		}

		[[nodiscard]] constexpr inline ContainedT *data() { return m_values; }
		[[nodiscard]] constexpr inline const ContainedT *data() const { return m_values; }
		[[nodiscard]] constexpr inline std::size_t size() const { return m_size; }

		static Matrix Identity(std::size_t rank)
		{
			Matrix identityMatrix{rank};
			for ( std::size_t i = 0; i < rank; ++i)
			{
				identityMatrix(i, i) = 1;
			}
			return identityMatrix;
		}

	private:
		void MoveData(Matrix &&matrixToMove) noexcept
		{
			delete[] m_values;

			m_rows = matrixToMove.m_rows;
			m_cols = matrixToMove.m_cols;
			m_size = matrixToMove.m_size;

			m_values = matrixToMove.m_values;

			matrixToMove.m_values = nullptr;
			matrixToMove.m_rows = matrixToMove.m_cols = matrixToMove.m_size = 0;
		}

		template<ReadonlyMatrixT M>
			requires std::convertible_to<typename M::contained, ContainedT>
		void CopyData(const M& matrixToCopy)
		{
			m_cols = matrixToCopy.m_cols;
			m_rows = matrixToCopy.m_rows;
			m_size = matrixToCopy.m_size;

			m_values = new ContainedT[m_size];

			for(size_t index = 0; index < m_size; index++)
			{
				m_values[index] = static_cast<ContainedT>(matrixToCopy.m_values[index]);
			}
		}

		[[nodiscard]] constexpr inline std::size_t CalculatePos(std::size_t row, std::size_t col) const noexcept
		{
			return row * m_cols + col;
		}

		std::size_t m_cols{ 0 };
		std::size_t m_rows{ 0 };
		std::size_t m_size{ 0 };

		ContainedT *m_values{ nullptr };
	};

	using MatrixF = Matrix<float>;
	using MatrixD = Matrix<double>;

	template<typename LContainedT, typename RContainedT, typename ResultT>
	struct OperationDeducer<Matrix<LContainedT>, Matrix<RContainedT>, ResultT>
	{
		using contained = ResultT;
		using value = Matrix<ResultT>;
	};

	static_assert(ReadonlyMatrixT<Matrix<float>>, "Basic matrix type doesn't follow the MatrixT concept");
	static_assert(MatrixT<Matrix<float>>, "Basic matrix type doesn't follow the MatrixT concept");
	static_assert(ContinuousStorageMatrix<Matrix<float>>, "Basic matrix type doesn't follow the ContinuousStorageMatrix concept");

	static_assert(ContinuousStorageMatrix<OperationDeducer<Matrix<float>, Matrix<float>, float>::value>, "Basic matrix type doesn't follow the MatrixT concept");

	template<typename ContainedT, std::size_t rows, std::size_t cols>
	class SMatrix
	{
	public:
		using contained = ContainedT;
		const constexpr static std::size_t ROWS = rows;
		const constexpr static std::size_t COLS = rows;

		constexpr SMatrix() = default;
		constexpr ~SMatrix() = default;

		constexpr SMatrix(const SMatrix &matrixToCopy) = default;
		constexpr SMatrix &operator=(const SMatrix &matrixToCopy) = default;

		constexpr SMatrix(SMatrix &&matrixToMove) noexcept = default;
		constexpr SMatrix &operator=(SMatrix &&matrixToMove) noexcept = default;

		constexpr SMatrix(const std::initializer_list<std::initializer_list<ContainedT>> &list)
		{
			if(list.size() != rows)
			{
				throw std::length_error{"Rows inconsistency detected"};
			}

			for (std::size_t row = 0; row < rows; ++row)
			{
				if(list.begin()[row].size() != cols)
				{
					throw std::length_error{"Cols inconsistency detected"};
				}
				for (std::size_t col = 0; col < cols; ++col)
				{
					m_values[CalculatePos(row, col)] = list.begin()[row].begin()[col];
				}
			}
		}

		template<ReadonlyMatrixT M>
		explicit SMatrix(const M &matrixToCopy)
		{
			CheckDimensions(*this, matrixToCopy);
			for (std::size_t row = 0; row < rows; ++row)
			{
				for (std::size_t col = 0; col < cols; ++col)
				{
					m_values[CalculatePos(row, col)] = matrixToCopy(row, col);
				}
			}
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
		[[nodiscard]] constexpr static inline std::size_t CalculatePos(std::size_t row, std::size_t col) noexcept
		{
			return row * cols + col;
		}

	private:
		std::array<ContainedT, rows * cols> m_values;
	};

	using Matrix2f = SMatrix<float, 2, 2>;
	using Matrix3f = SMatrix<float, 3, 3>;
	using Matrix4f = SMatrix<float, 4, 4>;

	using Matrix2d = SMatrix<double, 2, 2>;
	using Matrix3d = SMatrix<double, 3, 3>;
	using Matrix4d = SMatrix<double, 4, 4>;

	using Matrix2i = SMatrix<int, 2, 2>;
	using Matrix3i = SMatrix<int, 3, 3>;
	using Matrix4i = SMatrix<int, 4, 4>;

	static_assert(ContinuousStorageMatrix<Matrix2f>);
	static_assert(ContinuousStorageMatrix<Matrix2d>);

	static_assert(ContinuousStorageMatrix<Matrix3f>);
	static_assert(ContinuousStorageMatrix<Matrix3d>);

	static_assert(ContinuousStorageMatrix<Matrix4f>);
	static_assert(ContinuousStorageMatrix<Matrix4d>);

	template<typename ContainedLT, std::size_t lRows, std::size_t lCols,
		typename ContainedRT, std::size_t rRows, std::size_t rCols,
		typename ResultT>
	struct OperationDeducer<SMatrix<ContainedLT, lRows, lCols>, SMatrix<ContainedRT, rRows, rCols>, ResultT>
	{
		static_assert(lCols == rCols, "Left cols count should be equal to right cols count");
		static_assert(lRows == rRows, "Left rows count should be equal to right rows count");

		using value = SMatrix<ResultT, lRows, lCols>;
	};

	template<typename ContainedLT,
		typename ContainedRT, std::size_t rRows, std::size_t rCols,
		typename ResultT>
	struct OperationDeducer<Matrix<ContainedLT>, SMatrix<ContainedRT, rRows, rCols>, ResultT>
	{
		using value = Matrix<ResultT>;
	};

	template<typename ContainedLT, std::size_t lRows, std::size_t lCols,
		typename ContainedRT, typename ResultT>
	struct OperationDeducer<SMatrix<ContainedLT, lRows, lCols>, Matrix<ContainedRT>, ResultT>
	{
		using value = Matrix<ResultT>;
	};

	template<std::size_t LRows, std::size_t LCols, typename LContainedT,
		std::size_t RRows, std::size_t RCols, typename RContainedT>
	struct DotProductResultDeducer<SMatrix<LContainedT, LRows, LCols>, SMatrix<RContainedT, RRows, RCols>>
	{
		static_assert(LCols == RRows, "Left matrix cols is not the same as rows in the right one");

		using contained = decltype(LContainedT{} * RContainedT{} + LContainedT{} * RContainedT{});
		using value = SMatrix<contained, LRows, RCols>;
	};

	template<typename LContainedT,
		std::size_t RRows, std::size_t RCols, typename RContainedT>
	struct DotProductResultDeducer<Matrix<LContainedT>, SMatrix<RContainedT, RRows, RCols>>
	{
		using contained = decltype(LContainedT{} * RContainedT{} + LContainedT{} * RContainedT{});
		using value = Matrix<contained>;
	};

	template<std::size_t LRows, std::size_t LCols, typename LContainedT, typename RContainedT>
	struct DotProductResultDeducer<SMatrix<LContainedT, LRows, LCols>, Matrix<RContainedT>>
	{
		using contained = decltype(LContainedT{} * RContainedT{} + LContainedT{} * RContainedT{});
		using value = Matrix<contained>;
	};

	template<typename ContainedT, std::size_t rows, std::size_t cols>
	struct MatrixConstructor<SMatrix<ContainedT, rows, cols>>
	{
		static constexpr SMatrix<ContainedT, rows, cols> Create(std::size_t /*unused*/, std::size_t /*unused*/)
		{
			return SMatrix<ContainedT, rows, cols>{};
		}
	};
}

#endif // MATRIX_H
