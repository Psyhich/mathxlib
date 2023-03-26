#include <algorithm>
#include <random>
#include <stdexcept>
#include <ctime>
#include <cassert>
#include <iostream>
#include <cstring>
#include <stack>

#include "internal_use_only/config.hpp"
#include "matrixes.h"

void Matrix::MoveData(Matrix &&matrixToMove) noexcept
{
	delete[] m_values;

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

	static random_device generator;
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
		delete[] m_values;

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
	m_cols{cols},
	m_rows{rows},
	m_size{rows * cols},
	m_values{new double[rows * cols]}
{
}
Matrix::Matrix(size_t dimensions) :
	m_cols{dimensions},
	m_rows{dimensions},
	m_size{dimensions * dimensions},
	m_values{new double[m_size]}
{
}

Matrix::~Matrix() noexcept
{
	delete[](m_values);
}

// Methods
Matrix &Matrix::Randomise(double startValue, double endValue)
{
	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < GetCols(); col++)
		{
			(*this)(row, col) = Random(startValue, endValue);
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

Matrix Matrix::Map(const MapFunc &func) const
{
	Matrix outMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		func(outMatrix.m_values[index]);
	}

	return outMatrix;
}
void Matrix::MapInplace(const MapFunc &func)
{
	for (size_t index = 0; index < m_size; index++)
	{
		func(m_values[index]);
	}
}

Matrix Matrix::Transpose() const
{
	Matrix transposed(GetCols(), GetRows());
	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < GetCols(); col++)
		{
			transposed(col, row) = (*this)(row, col);
		}
	}
	return transposed;
}
Matrix Matrix::Multiply(const Matrix &matrixToMultiply) const
{
	CheckDimensions(matrixToMultiply.GetRows(), matrixToMultiply.GetCols());

	Matrix outMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] *= matrixToMultiply.m_values[index];
	}
	return outMatrix;
}

Matrix Matrix::Inverse() const
{
	if(GetRows() != GetCols())
	{
		throw std::runtime_error("Inverse is not implemented for non square matrixes");
	}
	const double determinant = Determinant();
	if(std::abs(determinant) <= DEFAULT_ACCURACY)
	{
		throw std::runtime_error("Matrix is triangular, no inverse can be found");
	}
	return (Adjoint().Transpose() / determinant).Transpose();
}

Matrix Matrix::Adjoint() const
{
	if(GetRows() != GetCols())
	{
		throw std::runtime_error("Adjoint is not implemented for non square matrixes");
	}

	Matrix outMatrix{*this};
	size_t const rank = GetRows();
	for (size_t currRow = 0; currRow < rank; currRow++)
	{
		for (size_t currCol = 0; currCol < rank; currCol++)
		{
			Matrix minor = ConstructMinor(currRow, currCol);
			// After we constructed minor calculating it's cofactor and assigning
			// it to outMatrix
			outMatrix(currRow, currCol) = minor.Determinant() * ((currRow + currCol) % 2 == 0 ? 1 : -1);
		}
	}

	return outMatrix.Transpose();
}
double Matrix::DeterminantByMinorExpansion() const
{

	std::stack<std::pair<double, Matrix>> matrixesToCalculate;

	matrixesToCalculate.push({ 1, *this });

	double determinant = 0;
	while(!matrixesToCalculate.empty())
	{
		const double coefficient = matrixesToCalculate.top().first;
		const Matrix currentMatrix = std::move(matrixesToCalculate.top().second);
		matrixesToCalculate.pop();

		const size_t currentMatrixRank = currentMatrix.GetRows();
		if(currentMatrixRank == 1)
		{
			determinant += coefficient * currentMatrix(0, 0);
		}
		else if(currentMatrixRank == 2)
		{
			determinant += coefficient * (currentMatrix(0, 0) * currentMatrix(1, 1) -
				currentMatrix(0, 1) * currentMatrix(1, 0));
		}
		else
		{
			for(size_t i = 0; i < currentMatrixRank; i++)
			{
				matrixesToCalculate.push(
					{
						(*this)(0, i) * (i % 2 == 0 ? 1 : -1),
						currentMatrix.ConstructMinor(0, i)
					});
			}
		}
	}

	return determinant;
}

Matrix Matrix::ConstructMinor(const size_t rowToExclude, const size_t colToExclude) const
{
	Matrix minor{GetRows() - 1, GetCols() - 1};
	size_t rowIndex{0};
	for (size_t row = 0; row < GetRows(); row++)
	{
		if(row != rowToExclude)
		{
			size_t colIndex{0};
			for (size_t col = 0; col < GetCols(); col++)
			{
				if (col != colToExclude)
				{
					minor(rowIndex, colIndex) = (*this)(row, col);
					++colIndex;
				}
			}
			++rowIndex;
		}
	}
	return minor;
}

double Matrix::Determinant() const
{
	if(GetRows() != GetCols())
	{
		throw std::runtime_error("Determinant is not implemented for non square matrixes");
	}
	return DeterminantByMinorExpansion();
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
	CheckDimensions(GetRows(), matrixToDotProduct.GetRows());

	Matrix out{GetRows(), matrixToDotProduct.GetCols()};

	for (size_t row = 0; row < GetRows(); row++)
	{
		for (size_t col = 0; col < matrixToDotProduct.GetCols(); col++) 
		{
			double &val = out(row, col);
			val = 0;
			for (size_t iter = 0; iter < matrixToDotProduct.GetRows(); iter++)
			{
				val += (*this)(row, iter) * matrixToDotProduct(iter, col);
			}
		}
	}

	return out;
}

Matrix Matrix::operator+(const Matrix &matrixToAdd) const
{
	CheckDimensions(matrixToAdd.GetRows(), matrixToAdd.GetCols());

	Matrix outMatrix = *this;

	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] += matrixToAdd.m_values[index];
	}

	return outMatrix;
}
Matrix Matrix::operator-(const Matrix &matrixToSubstract) const
{
	CheckDimensions(matrixToSubstract.GetRows(), matrixToSubstract.GetCols());

	Matrix outMatrix = *this;

	for (size_t index = 0; index < m_size; index++)
	{
		outMatrix.m_values[index] -= matrixToSubstract.m_values[index];
	}

	return outMatrix;
}

Matrix &Matrix::operator+=(const Matrix &matrixToAdd)
{
	CheckDimensions(matrixToAdd.GetRows(), matrixToAdd.GetCols());

	for (size_t index = 0; index < m_size; index++)
	{
		m_values[index] += matrixToAdd.m_values[index];
	}
	return *this;
}
Matrix &Matrix::operator-=(const Matrix &matrixToSubstract)
{
	CheckDimensions(matrixToSubstract.GetRows(), matrixToSubstract.GetCols());

	for(size_t index = 0; index < m_size; index++)
	{
		m_values[index] -= matrixToSubstract.m_values[index];
	}

	return *this;
}

// Operations with scalar values
Matrix Matrix::operator/(double numberToDivide) const
{
	Matrix resultedMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		resultedMatrix.m_values[index] /= numberToDivide;
	}

	return resultedMatrix;
}
Matrix Matrix::operator*(double numberToMultiply) const
{
	Matrix resultedMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		resultedMatrix.m_values[index] *= numberToMultiply;
	}

	return resultedMatrix;
}

Matrix Matrix::operator+(double numberToAdd) const
{
	Matrix resultedMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		resultedMatrix.m_values[index] += numberToAdd;
	}
	return resultedMatrix;
}

Matrix Matrix::operator-(double numberToSubstract) const
{
	Matrix resultedMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		resultedMatrix.m_values[index] -= numberToSubstract;
	}
	return resultedMatrix;
}

// Unaries
Matrix Matrix::operator-() const
{
	Matrix negativeMatrix{*this};
	for (size_t index = 0; index < m_size; index++)
	{
		negativeMatrix.m_values[index] *= -1;
	}
	return negativeMatrix;
}
