#include <random>
#include <stdexcept>
#include <ctime>
#include <cassert>
#include <iostream>

#include "matrixes.h"

void Matrix::MoveData(Matrix &&matrixToMove) noexcept
{
	CleanData();

	m_rows = matrixToMove.m_rows;
	m_cols = matrixToMove.m_cols;
	m_size = matrixToMove.m_size;

	m_values = matrixToMove.m_values;

	matrixToMove.m_values = nullptr;
	matrixToMove.m_rows = matrixToMove.m_cols = matrixToMove.m_size = 0;
}

void Matrix::CopyData(const Matrix& matrixToCopy)
{
	m_cols = matrixToCopy.m_cols;
	m_rows = matrixToCopy.m_rows;
	m_size = matrixToCopy.m_size;

	m_values = new double[m_size];

	for(size_t index = 0; index < m_size; index++)
	{
		m_values[index] = matrixToCopy.m_values[index];
	}
}

void Matrix::CleanData() noexcept
{
	delete[] m_values;
}

Matrix &Matrix::SetAll(double valueToSet) noexcept
{
	for(std::size_t index = 0; index < m_size; index++)
	{
		m_values[index] = valueToSet;
	}

	return *this;
}

double Matrix::Random(double start, double end) noexcept
{
	using namespace std;

	static default_random_engine generator(unsigned(time(nullptr)));
	uniform_real_distribution<double> distribution(start, end);

	return distribution(generator);
}

Matrix::Matrix(const Matrix &matrixToCopy)
{
	CopyData(matrixToCopy);
}
Matrix &Matrix::operator=(const Matrix &matrixToCopy)
{
	if(this != &matrixToCopy)
	{
		CleanData();

		CopyData(matrixToCopy);
	}

	return *this;
}

Matrix::Matrix(Matrix &&matrixToMove) noexcept
{
	MoveData(std::move(matrixToMove));
}
Matrix &Matrix::operator=(Matrix &&matrixToMove) noexcept
{
	if (this != &matrixToMove)
	{
		MoveData(std::move(matrixToMove));
	}
	return *this;
}

// Simple constructors
Matrix::Matrix(size_t rows, size_t cols) :
	m_rows{rows},
	m_cols{cols},
	m_size{rows * cols},
	m_values{new double[rows * cols]}
{
}
Matrix::Matrix(size_t dimensions) :
	m_rows{dimensions},
	m_cols{dimensions},
	m_size{dimensions * dimensions},
	m_values{new double[m_size]}
{
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &initializer) :
	m_rows{initializer.size()},
	m_cols{initializer.begin()->size()},
	m_size{m_rows * m_cols},
	m_values{new double[m_size]}
{
	for(const auto &list : initializer)
	{
		assert(list.size() == m_cols);
	}

	size_t index = 0;
	for(const auto &list : initializer)
	{
		size_t internalIndex = 0;
		for(const auto &val : list)
		{
			Get(index, internalIndex) = val;
			internalIndex++;
		}

		index++;
	}
}

Matrix::~Matrix() noexcept
{
	delete[](m_values);
}

// Methods
Matrix &Matrix::Randomise(double startValue, double endValue) noexcept
{
	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < GetCols(); col++)
		{
			Get(row, col) = Random(startValue, endValue);
		}
	}
	return *this;
}

void Matrix::Print() const
{
	for (size_t i = 0; i < GetRows(); i++)
	{
		std::cout << "| ";
		for (size_t j = 0; j < GetCols(); j++)
		{
			std::cout << (*this)(i, j) << " ";
		}
		std::cout << "|\n";
	}
	std::cout << std::endl;
}

Matrix Matrix::Map(MapFunc func) const noexcept
{
	Matrix outMatr{*this};
	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < GetCols(); col++)
		{
			func(outMatr.Get(row, col), row, col);
		}
	}

	return outMatr;
}

Matrix Matrix::Map(ReturningMapFunc func) const noexcept
{
	Matrix outMatr{*this};
	for(size_t index = 0; index < m_size; index++)
	{
		outMatr.m_values[index] = func(m_values[index]);
	}

	return outMatr;
}

Matrix Matrix::Transpose() const
{
	Matrix transposed(GetCols(), GetRows());
	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < GetCols(); col++)
		{
			transposed(col, row) = Get(row, col);
		}
	}
	return transposed;
}
Matrix Matrix::Multiply(const Matrix &matrixToMultiply) const
{
	assert(GetRows() == matrixToMultiply.GetRows());
	assert(GetCols() == matrixToMultiply.GetCols());
	
	Matrix outMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] *= matrixToMultiply.m_values[index];
	}
	return outMatrix;
}

Matrix Matrix::Inverse() const
{
	assert(GetRows() == GetCols());
	double determ = Determinant();
	if (determ == 0)
	{
		return Matrix(0);
	}

	return (Adjoint().Transpose() / determ).Transpose();
}

Matrix Matrix::Adjoint() const
{
	assert(GetRows() == GetCols());

	Matrix outMatrix{*this};
	size_t const rank = GetRows();
	for (size_t currRow = 0; currRow < rank; currRow++)
	{
		for (size_t currCol = 0; currCol < rank; currCol++)
		{
			Matrix minor(rank - 1);

			// Building minor
			size_t rowIndex = 0; // real minor row index
			// Loop goes through all matrix and adds only rows and cols that are not
			// currRow and currCol
			for (size_t row = 0; row < rank; row++)
			{
				if (row != currRow)
				{
					// If we get simillar we just skip and don't add row/col
					size_t colIndex = 0;
					for (size_t col = 0; col < rank; col++)
					{
						if (col != currCol)
						{
							minor(rowIndex, colIndex) = Get(row, col);
							++colIndex;
						}
					}
					++rowIndex;
				}
			}
			// After we constructed minor calculating it's cofactor and assigning
			// it to outMatrix
			outMatrix(currRow, currCol) = minor.Determinant() * ((currRow + currCol) % 2 == 0 ? 1 : -1);
		}
	}

	return outMatrix.Transpose();
}

double Matrix::Determinant() const noexcept
{
	assert(GetRows() == GetCols());

	const size_t rank = GetRows();
	if (rank == 1)
	{
		return Get(0, 0);
	}
	else if(rank == 2)
	{
		return Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0);
	}
	else
	{
		double determinant = 0;
		Matrix minor(rank - 1);
		for (size_t i = 0; i < rank; i++)
		{
			// Constructing minor
			for (size_t row = 1; row < rank; row++)
			{
				size_t colIndex = 0;
				for (size_t col = 0; col < rank; col++)
				{
					if (col != i)
					{
						minor(row - 1, colIndex) = Get(row, col);
						++colIndex;
					}
				}
			}
			determinant += minor.Determinant() * 
				Get(0, i) * (i % 2 == 0 ? 1 : -1);
		}
		return determinant;
	}
}

bool Matrix::IsEqualTo(const Matrix &otherMatrix, double eps) const noexcept
{
	if(GetCols() != otherMatrix.GetCols() ||
		GetRows() != otherMatrix.GetRows())
	{
		return false;
	}

	bool isEqual{true};
	for(std::size_t index = 0;
		index < m_size && isEqual; index++)
	{
		isEqual &= eps >= std::abs(m_values[index] - otherMatrix.m_values[index]);
	}

	return isEqual;
}

// Operations with other matrixes
Matrix Matrix::operator*(const Matrix &matrixToDotProduct) const
{
	assert(GetCols() == matrixToDotProduct.GetRows());

	Matrix out{GetRows(), matrixToDotProduct.GetCols()};

	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < matrixToDotProduct.GetCols(); col++) 
		{
			double &val = out.Get(row, col);
			val = 0;
			for (size_t iter = 0; iter < matrixToDotProduct.GetRows(); iter++)
			{
				val += Get(row, iter) * matrixToDotProduct.Get(iter, col);
			}
		}
	}

	return out;
}

Matrix Matrix::operator+(const Matrix &matrixToAdd) const
{
	assert(GetRows() == matrixToAdd.GetRows());
	assert(GetCols() == matrixToAdd.GetCols());

	Matrix outMatrix = *this;

	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] += matrixToAdd.m_values[index];
	}

	return outMatrix;
}
Matrix Matrix::operator-(const Matrix &matrixToSubstract) const
{
	assert(GetRows() == matrixToSubstract.GetRows());
	assert(GetCols() == matrixToSubstract.GetCols());

	Matrix outMatrix = *this;

	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] -= matrixToSubstract.m_values[index];
	}

	return outMatrix;
}

Matrix &Matrix::operator+=(const Matrix &matrixToAdd) noexcept
{
	assert(GetRows() == matrixToAdd.GetRows());
	assert(GetCols() == matrixToAdd.GetCols());

	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] += matrixToAdd.m_values[index];
	}
	return *this;
}
Matrix &Matrix::operator-=(const Matrix &matrixToSubstract) noexcept
{
	assert(GetRows() == matrixToSubstract.GetRows());
	assert(GetCols() == matrixToSubstract.GetCols());

	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] -= matrixToSubstract.m_values[index];
	}

	return *this;
}

// Operations with scalar values
Matrix &Matrix::operator/(double numberToDivide) noexcept
{
	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] /= numberToDivide;
	}

	return *this;
}
Matrix &Matrix::operator*(double numberToMultiply) noexcept
{
	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] *= numberToMultiply;
	}

	return *this;
}

Matrix &Matrix::operator+(double numberToAdd) noexcept
{
	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] += numberToAdd;
	}

	return *this;
}

Matrix &Matrix::operator-(double numberToSubstract) noexcept
{
	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] -= numberToSubstract;
	}

	return *this;
}

// Unaries
Matrix &Matrix::operator-() noexcept
{
	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] *= -1;
	}

	return *this;
}
