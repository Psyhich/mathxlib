#ifndef MATRIXES_H
#define MATRIXES_H

#include <functional>

class Matrix
{
public:
	using size_t = std::size_t;
	using MapFunc = std::function<void(double &value, size_t row, size_t col)>;
	using ReturningMapFunc = std::function<double(double)>;

	Matrix() noexcept = default;
	explicit Matrix(const std::initializer_list<std::initializer_list<double>> &initializer);
	Matrix(size_t rows, size_t cols);
	explicit Matrix(size_t dimensions);

	Matrix(const Matrix &matrixToCopy);
	Matrix &operator=(const Matrix &matrixToCopy);

	Matrix(Matrix &&matrixToMove) noexcept;
	Matrix &operator=(Matrix &&matrixToMove) noexcept;

	~Matrix() noexcept;

	Matrix &Randomise(double startValue, double endValue) noexcept;
	void Print() const;
	Matrix Map(MapFunc func) const noexcept;
	Matrix Map(ReturningMapFunc func) const noexcept;

	Matrix Transpose() const;
	Matrix Multiply(const Matrix &matrixToMultiply) const;
	Matrix Inverse() const;
	Matrix Adjoint() const;
	double Determinant() const noexcept;
	bool IsEqualTo(const Matrix &otherMatrix, double eps=1e-6) const noexcept;

	// Getters and setters
	constexpr inline size_t GetCols() const noexcept { return m_cols; }
	constexpr inline size_t GetRows() const noexcept { return m_rows; }

	constexpr inline const double &operator()(size_t row, size_t col) const noexcept
	{
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline double &operator()(size_t row, size_t col) noexcept
	{
		return m_values[CalculatePos(row, col)];
	}

	constexpr inline const double &Get(size_t row, size_t col) const noexcept
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
	Matrix &operator+=(const Matrix &matrixToAdd) noexcept;
	Matrix &operator-=(const Matrix &matrixToSubstract) noexcept;

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
	void CleanData() noexcept;

	constexpr inline size_t CalculatePos(size_t row, size_t col) const noexcept
	{
		return row * m_cols + col;
	}

	double Random(double start, double end) noexcept;
private:
	size_t m_cols{ 0 };
	size_t m_rows{ 0 };
	size_t m_size{ 0 };

	double *m_values{ nullptr };
};

#endif
