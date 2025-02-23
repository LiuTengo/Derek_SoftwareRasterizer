#pragma once
#include <iostream>
#include <array>

#include "Vector.hpp"

//m–– n¡–
template<class T,int m,int n>
class Matrix {
public:
	Matrix();
	Matrix(std::initializer_list<std::initializer_list<T>> list);
	~Matrix();
public:
	static void PrintMatrix(const Matrix<T, m, n>& mat);
public:
	std::array<std::array<T,n>,m> data;

public:
	bool IsSameDimension(int r,int c);
	bool CanMultiply(int r);
	bool IsSquare();
	bool CanInverse();
	int GetRow() const { return m; };
	int GetCol() const { return n; };

	Matrix<T, m, n> operator + (const Matrix<T, m, n>& mat);
	Matrix<T, m, n> operator - (const Matrix<T, m, n>& mat);

	template<int bCol>
	Matrix<T, m, bCol> operator * (const Matrix<T, n, bCol>& mat);
	Vector<m, T> operator * (const Vector<n, T>& vec);
	
	std::array<T, n>& operator [](int index);
	const std::array<T, n>& operator [](int index) const;

	Matrix<T, m, n> operator * (const T& p);
	Matrix<T, m, n> operator / (const T& p);

	Matrix<T, n, m> Transpose() const;
	Matrix<T, m, n> Inverse() const;
};

#pragma region Construction & Deconstruction=
template<class T, int m, int n>
inline Matrix<T,m,n>::Matrix()
{
	data.fill(std::array<T,m>());
}

template<class T, int m, int n>
inline Matrix<T, m, n>::Matrix(std::initializer_list<std::initializer_list<T>> list)
{
	if (list.size() != m) {
		throw std::invalid_argument("Incorrect number of rows");
	}
	else {
		auto rowIter = list.begin();
		for (int i = 0; i < n;i++,rowIter++) {
			if (rowIter->size() != n) {
				throw std::invalid_argument("Incorrect number of columns");
			}
			else {
				std::copy(rowIter->begin(), rowIter->end(), data[i].begin());
			}
		}
	}
}

template<class T, int m, int n>
inline Matrix<T, m, n>::~Matrix()
{
	std::cout << "Delete Matrix" << std::endl;
}

#pragma endregion

#pragma region Static Functions
template<class T, int m, int n>
inline void Matrix<T, m, n>::PrintMatrix(const Matrix<T, m, n>& mat)
{
	std::cout << "Print Matrix:" << std::endl;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << mat[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}
#pragma endregion

#pragma region Helper Functions
template<class T, int m, int n>
inline bool Matrix<T, m, n>::IsSameDimension(int r, int c)
{
	return (c == n) && (r == m);
}

template<class T, int m, int n>
inline bool Matrix<T, m, n>::CanMultiply(int r)
{
	return n == r;
}
template<class T, int m, int n>
inline bool Matrix<T, m, n>::IsSquare()
{
	return  m == n;
}
template<class T, int m, int n>
inline bool Matrix<T, m, n>::CanInverse()
{
	return false;
}
#pragma endregion

#pragma region Operator Override
template<class T, int m, int n>
inline Matrix<T, m, n> Matrix<T, m, n>::operator+(const Matrix<T, m, n>& mat)
{
	Matrix<T, m, n> res = Matrix<T, m, n>();
	if (IsSameDimension(mat.GetRow(), mat.GetCol())) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				res[i][j] = data[i][j] + mat[i][j];
			}
		}
	}
	else {
		throw std::invalid_argument("Matrices dimensions must match for addition");
	}
	return res;
}

template<class T, int m, int n>
inline Matrix<T, m, n> Matrix<T, m, n>::operator-(const Matrix<T, m, n>& mat)
{
	Matrix<T, m, n> res = Matrix<T, m, n>();
	if (IsSameDimension(mat.GetRow(), mat.GetCol())) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				res[i][j] = data[i][j] - mat[i][j];
			}
		}
	}
	else {
		throw std::invalid_argument("Matrices dimensions must match for addition");
	}
	return res;
}

template<class T, int m, int n>
template<int bCol>
inline Matrix<T, m, bCol> Matrix<T, m, n>::operator*(const Matrix<T, n, bCol>& mat)
{
	Matrix<T, m, bCol> res = Matrix<T, m, bCol>();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < bCol; j++) {
			T sum = T();
			for (int k = 0; k < n; k++) {
				sum += data[i][k] * mat[k][j];
			}
			res[i][j] = sum;
		}
	}
	return res;
}

template<class T, int m, int n>
inline Vector<m, T> Matrix<T, m, n>::operator*(const Vector<n,T> &vec)
{
	Vector<m, T> res = Vector<m, T>();
	for (int i = 0; i < m;i++) {
		for (int j = 0; j < n;j++) {
			res[i] += data[i][j] * vec[j];
		}
	}
	return res;
}

template<class T, int m, int n>
inline std::array<T,n>& Matrix<T, m, n>::operator[](int index)
{
	if (index >= m || index < 0) {
		std::out_of_range("Row index out of range");
	}
	return data[index];
}

template<class T, int m, int n>
inline const std::array<T, n>& Matrix<T, m, n>::operator[](int index) const
{
	if (index >= m || index < 0) {
		std::out_of_range("Row index out of range");
	}
	return data[index];
}

template<class T, int m, int n>
inline Matrix<T, m, n> Matrix<T, m, n>::operator*(const T& p)
{
	Matrix<T, m, n> res = Matrix<T, m, n>();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			res[i][j] = data[i][j] * p;
		}
	}
	return res;
}

template<class T, int m, int n>
inline Matrix<T, m, n> Matrix<T, m, n>::operator/(const T& p)
{
	Matrix<T, m, n> res = Matrix<T, m, n>();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			res[i][j] = data[i][j] / p;
		}
	}
	return res;
}

#pragma endregion

#pragma region Matrix Properties
template<class T, int m, int n>
inline Matrix<T, n, m> Matrix<T, m, n>::Transpose() const
{
	Matrix<T,n,m> transpose = Matrix<T, m, n>();;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			transpose[j][i] = data[i][j];
		}
	}

	return transpose;
}

//TODO: 
template<class T, int m, int n>
inline Matrix<T, m, n> Matrix<T, m, n>::Inverse() const
{
	if (IsSquare()) {

	}
	else {
		throw std::invalid_argument("Only square matrix can be inversed");
		return Matrix<T>();
	}
}

#pragma endregion

typedef Matrix<int, 3, 3> Matrix3X3i;
typedef Matrix<int, 4, 4> Matrix4X4i;

typedef Matrix<float, 3, 3> Matrix3X3f;
typedef Matrix<float, 4, 4> Matrix4X4f;

typedef Matrix<double, 3, 3> Matrix3X3d;
typedef Matrix<double, 4, 4> Matrix4X4d;