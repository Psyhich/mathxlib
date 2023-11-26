#ifndef VECTOR_H
#define VECTOR_H

#include "matrixes/matrix.h"
#include "matrixes/algorithms.h"

namespace MxLib
{
	template<typename ContainedT>
	class Vector : private Matrix<ContainedT>
	{
	public:
		Vector() = default;
		explicit Vector(std::size_t cols) :
			Matrix<ContainedT>{1, cols}
		{}

		Vector(std::size_t cols, const ContainedT *dataToSet) :
			Matrix<ContainedT>{dataToSet, 1, cols}
		{}

		Vector(const std::initializer_list<ContainedT> &values) :
			Matrix<ContainedT>{1, values.size()}
		{
			for (std::size_t i = 0; i < values.size(); ++i)
			{
				(*this)[i] = values.begin()[i];
			}
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col)
		{
			return (*this)(0, col);
		}
		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col) const
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline std::size_t size() const
		{
			return Matrix<ContainedT>::size();
		}

		[[nodiscard]] const Matrix<ContainedT> &toMatrix() const
		{
			return *static_cast<Matrix<ContainedT> *>(*this);
		}
	};

	using VectorF = Vector<float>;
	using VectorD = Vector<double>;
	using VectorI = Vector<int>;

	template<typename ContainedT, std::size_t _size>
	class SVector : private SMatrix<ContainedT, 1, _size>
	{
	public:
		SVector() = default;

		explicit SVector(const ContainedT *dataToSet)
			: SMatrix<ContainedT, 1, _size>{dataToSet}
		{}

		SVector(const ContainedT (&values)[_size]) :
			SMatrix<ContainedT, 1, _size>{}
		{
			std::memcpy(this->data(), values, _size);
		}
		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col)
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col) const
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline std::size_t size() const
		{
			return SMatrix<ContainedT, 1, _size>::size();
		}

		[[nodiscard]] const SMatrix<ContainedT, 1, _size> &toMatrix() const
		{
			return *static_cast<SMatrix<ContainedT, 1, _size> *>(*this);
		}
	};

	template<typename ContainedT>
	class SVector<ContainedT, 2> : private SMatrix<ContainedT, 1, 2>
	{
	public:
		SVector() = default;

		explicit SVector(const ContainedT *dataToSet)
			: SMatrix<ContainedT, 1, 2>{dataToSet}
			, x{(*this)[0]}
			, y{(*this)[1]}
		{}

		SVector(const ContainedT (&values)[2])
			: SMatrix<ContainedT, 1, 2>{}
			, x{(*this)[0]}
			, y{(*this)[1]}
		{
			std::memcpy(this->data(), values, 2);
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col)
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col) const
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline std::size_t size() const
		{
			return SMatrix<ContainedT, 1, 2>::size();
		}

		[[nodiscard]] const SMatrix<ContainedT, 1, 2> &toMatrix() const
		{
			return *static_cast<SMatrix<ContainedT, 1, 2> *>(*this);
		}

		ContainedT &x;
		ContainedT &y;
	};

	template<typename ContainedT>
	class SVector<ContainedT, 3> : private SMatrix<ContainedT, 1, 3>
	{
	public:
		SVector() = default;

		explicit SVector(const ContainedT *dataToSet)
			: SMatrix<ContainedT, 1, 3>{dataToSet}
			, x{(*this)[0]}
			, y{(*this)[1]}
		{}

		SVector(const ContainedT (&values)[3])
			: SMatrix<ContainedT, 1, 3>{}
			, x{(*this)[0]}
			, y{(*this)[1]}
			, z{(*this)[2]}
		{
			std::memcpy(this->data(), values, 3);
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col)
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline ContainedT &operator[](std::size_t col) const
		{
			return (*this)(0, col);
		}

		[[nodiscard]] constexpr inline std::size_t size() const
		{
			return SMatrix<ContainedT, 1, 3>::size();
		}

		[[nodiscard]] const SMatrix<ContainedT, 1, 3> &toMatrix() const
		{
			return *static_cast<SMatrix<ContainedT, 1, 3> *>(*this);
		}

		ContainedT &x;
		ContainedT &y;
		ContainedT &z;
	};



	using Vector2F = SVector<float, 2>;
	using Vector3F = SVector<float, 3>;
	using Vector2D = SVector<double, 2>;
	using Vector3D = SVector<double, 3>;
	using Vector2I = SVector<int, 2>;
	using Vector3I = SVector<int, 3>;
}

#endif // VECTOR_H
