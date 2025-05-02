#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "Utility.h"

template <typename T>
class SquareMatrix
{
public:
	SquareMatrix(const SquareMatrix&) = default;
	SquareMatrix(SquareMatrix&&) = default;
	SquareMatrix& operator=(const SquareMatrix&) = default;
	SquareMatrix& operator=(SquareMatrix&&) = default;
	~SquareMatrix() = default;
	SquareMatrix(int size, const T& value);
	SquareMatrix(int size);

	void checkValidValue(int value) const;
	int checkInteger(std::istream& istr) const;
	int size() const;

	T& operator()(int i, int j);
	const T& operator()(int i, int j) const;
	SquareMatrix& operator+=(const SquareMatrix& rhs);
	SquareMatrix& operator-=(const SquareMatrix& rhs);
	SquareMatrix operator+(const SquareMatrix& rhs) const;
	SquareMatrix operator-(const SquareMatrix& rhs) const;
	SquareMatrix operator*(const T& scalar) const;
	SquareMatrix Transpose() const;

private:
	int m_size;
	std::vector<std::vector<T>> m_matrix;

};

//-----------------------------------------------------------------------------

template <typename T>
int SquareMatrix<T>::size() const
{
	return m_size;
}

//-----------------------------------------------------------------------------

template <typename T>
void SquareMatrix<T>::checkValidValue(int value) const
{
	if (value > MAX_ALLOWED_VALUE || value < MIN_ALLOWED_VALUE)
	{
		throw std::out_of_range("Value is out of the allowed range!");
	}
}

//-----------------------------------------------------------------------------

template <typename T>
int SquareMatrix<T>::checkInteger(std::istream& istr) const
{
	std::string input;
	std::size_t pos;

	istr >> input;
	int value = std::stoi(input, &pos);

	if (pos != input.size())
	{
		throw std::out_of_range("Input is not a valid number.");
	}
	return value;
}

//-----------------------------------------------------------------------------

template <typename T>
const T& SquareMatrix<T>::operator()(int i, int j) const
{
	return m_matrix[i][j];
}

//-----------------------------------------------------------------------------

template <typename T>
T& SquareMatrix<T>::operator()(int i, int j)
{
	return m_matrix[i][j];
}

//-----------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& ostr, const SquareMatrix<int>& matrix)
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			ostr << matrix(i, j) << ' ';
		}
		ostr << '\n';
	}
	return ostr;
}

//-----------------------------------------------------------------------------

inline std::istream& operator>>(std::istream& istr, SquareMatrix<int>& matrix)
{
	int input;

	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			input = matrix.checkInteger(istr);
			matrix.checkValidValue(input);

			matrix(i, j) = input;
		}
	}

	return istr;
}

//-----------------------------------------------------------------------------

// Implementation must be in .h file for the compiler to see it and instantiate
// the relevant function
template <typename T>
SquareMatrix<T>::SquareMatrix(int size, const T& value)
	: m_size(size), m_matrix(size, std::vector<T>(size, value))
{
	for (int i = 0; i < size * size; ++i)
	{
		m_matrix[i / size][i % size] = value;
	}
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T>::SquareMatrix(int size)
	: m_size(size), m_matrix(size, std::vector<T>(size))
{
	for (int i = 0; i < size * size; ++i)
	{
		m_matrix[i / size][i % size] = i;
	}
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator+(const SquareMatrix& rhs) const
{
	SquareMatrix result(*this);
	return result += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator-(const SquareMatrix& rhs) const
{
	SquareMatrix result(*this);
	return result -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator+=(const SquareMatrix& rhs)
{
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			m_matrix[i][j] += rhs.m_matrix[i][j];
			checkValidValue(m_matrix[i][j]);
		}
	}
	return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator-=(const SquareMatrix& rhs)
{
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			m_matrix[i][j] -= rhs.m_matrix[i][j];
			checkValidValue(m_matrix[i][j]);
		}
	}
	return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T> SquareMatrix<T>::Transpose() const
{
	SquareMatrix result(m_size);
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			result(i, j) = m_matrix[j][i];
		}
	}
	return result;
}

//-----------------------------------------------------------------------------

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator*(const T& scalar) const
{
	SquareMatrix result(*this);
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			result(i, j) *= scalar;
			checkValidValue(result(i, j));
		}
	}
	return result;
}