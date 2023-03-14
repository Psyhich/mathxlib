#ifndef MATRIXES_H
#define MATRIXES_H

#include <functional>
#include <array>
#include <initializer_list>
#include <stdexcept>

class Matrix
{
public:
	using size_t = std::size_t;
	using MapFunc = std::function<void(double &value)>;

	template<size_t ColsCount>
	using MatrixInitializerList = std::initializer_list<double[ColsCount]>;

	Matrix() noexcept = default;

	template<size_t ColsCount>
	explicit Matrix(const MatrixInitializerList<ColsCount> &initializer) :
		m_cols{ColsCount},
		m_rows{initializer.size()},
		m_size{m_rows * m_cols},
		m_values{new double[m_size]}
	{
		for(std::size_t rowIndex = 0; rowIndex < m_rows; rowIndex++)
		{
			for(std::size_t colIndex = 0; colIndex < ColsCount; colIndex++)
			{
				Set(rowIndex, colIndex, initializer.begin()[rowIndex][colIndex]);
			}
		}
	}
	Matrix(size_t rows, size_t cols);
	explicit Matrix(size_t dimensions);

	Matrix(const Matrix &matrixToCopy);
	Matrix &operator=(const Matrix &matrixToCopy);

	Matrix(Matrix &&matrixToMove) noexcept;
	Matrix &operator=(Matrix &&matrixToMove) noexcept;

	~Matrix() noexcept;

	Matrix &Randomise(double startValue, double endValue) noexcept;
	void Print() const;
	[[nodiscard]] Matrix Map(const MapFunc &func) const;
	void MapInplace(const MapFunc &func);

	[[nodiscard]] Matrix Transpose() const;
	[[nodiscard]] Matrix Multiply(const Matrix &matrixToMultiply) const;
	[[nodiscard]] Matrix Inverse() const;
	[[nodiscard]] Matrix Adjoint() const;
	[[nodiscard]] double Determinant() const;
	[[nodiscard]] bool IsEqualTo(const Matrix &otherMatrix, double eps=DEFAULT_ACCURACY) const noexcept;

	// Getters and setters
	[[nodiscard]] constexpr inline size_t GetCols() const noexcept { return m_cols; }
	[[nodiscard]] constexpr inline size_t GetRows() const noexcept { return m_rows; }

	constexpr inline const double &operator()(size_t row, size_t col) const noexcept
	{
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline double &operator()(size_t row, size_t col) noexcept
	{
		return m_values[CalculatePos(row, col)];
	}

	[[nodiscard]] constexpr inline const double &Get(size_t row, size_t col) const noexcept
	{
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline double &Get(size_t row, size_t col) noexcept
	{
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline void Set(size_t row, size_t col, double value) noexcept
	{
		m_values[CalculatePos(row, col)] = value;
	}
	Matrix &SetAll(double valueToSet) noexcept;

	// Operations with other matrixes
	Matrix operator*(const Matrix &matrixToDotProduct) const;

	Matrix operator+(const Matrix &matrixToAdd) const;
	Matrix operator-(const Matrix &matrixToSubstract) const;
	Matrix &operator+=(const Matrix &matrixToAdd);
	Matrix &operator-=(const Matrix &matrixToSubstract);

	// Operations with scalar values
	Matrix &operator/(double numberToDivide) noexcept;
	Matrix &operator*(double numberToMultiply) noexcept;
	Matrix &operator+(double numberToAdd) noexcept;
	Matrix &operator-(double numberToSubstract) noexcept;

	// Unaries
	Matrix &operator-() noexcept;

private:
	void MoveData(Matrix &&matrixToMove) noexcept;
	void CopyData(const Matrix& matrixToCopy);

	inline void CheckBounds(size_t requiredRows, size_t requiredColumns) const
	{
		if(GetCols() != requiredColumns)
		{
			throw std::length_error("Required columns not matched");
		}
		else if(GetRows() != requiredRows)
		{
			throw std::length_error("Required rows not matched");
		}
	}

	[[nodiscard]] constexpr inline size_t CalculatePos(size_t row, size_t col) const noexcept
	{
		return row * m_cols + col;
	}

	static double Random(double start, double end) noexcept;

	[[nodiscard]] Matrix ConstructMinor(const size_t rowToExclude, const size_t colToExclude) const;
	[[nodiscard]] double DeterminantByMinorExpansion() const;

	static inline constexpr double DEFAULT_ACCURACY{1e-6};

	size_t m_cols{ 0 };
	size_t m_rows{ 0 };
	size_t m_size{ 0 };

	double * m_values{ nullptr };
};

#endif
