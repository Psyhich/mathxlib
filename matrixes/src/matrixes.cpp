#include <algorithm>
#include <random>
#include <stdexcept>
#include <ctime>
#include <cassert>
#include <iostream>
#include <cstring>
#include <stack>

#include "internal_use_only/config.hpp"
#include "matrixes/matrix.h"

namespace MxLib
{

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
Matrix::Matrix(std::size_t rows, std::size_t cols) :
	m_cols{cols},
	m_rows{rows},
	m_size{rows * cols},
	m_values{new double[rows * cols]}
{
}
Matrix::Matrix(std::size_t dimensions) :
	m_cols{dimensions},
	m_rows{dimensions},
	m_size{dimensions * dimensions},
	m_values{new double[m_size]}
{
}

Matrix::Matrix(const double *data, std::size_t rows, std::size_t cols) :
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
Matrix::Matrix(const double **data, std::size_t rows, std::size_t cols) :
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

Matrix::~Matrix() noexcept
{
	delete[](m_values);
}

Matrix Matrix::Identity(std::size_t rank)
{
	Matrix identityMatrix{rank};
	for ( std::size_t i = 0; i < rank; ++i)
	{
		identityMatrix(i, i) = 1;
	}
	return identityMatrix;
}

}
