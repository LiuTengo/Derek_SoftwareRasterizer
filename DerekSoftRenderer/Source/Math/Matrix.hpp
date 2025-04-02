#pragma once

#include <iostream>
#include <array>

#include "Vector.hpp"

//m行 n列
template<class T, int m, int n>
class Matrix {
public:
	Matrix();
	Matrix(std::initializer_list<std::initializer_list<T>> list);
	~Matrix();
public:
	static Matrix<T, m, n> Identity();
	static void PrintMatrix(const Matrix<T, m, n>& mat);
public:
	std::array<std::array<T, n>, m> data;

public:
	bool IsSameDimension(int r, int c);
	bool CanMultiply(int r);
	bool IsSquare();
	bool CanInverse();
	int GetRow() const { return m; };
	int GetCol() const { return n; };

	Matrix<T, m, n> operator + (const Matrix<T, m, n>& mat);
	Matrix<T, m, n> operator - (const Matrix<T, m, n>& mat);

	template<int bCol>
	Matrix<T, m, bCol> operator * (const Matrix<T, n, bCol>& mat);
	template<int bCol>
	Matrix<T, m, bCol> operator * (const Matrix<T, n, bCol>& mat) const;
	Vector<m, T> operator * (const Vector<n, T>& vec);
	Vector<m, T> operator * (const Vector<n, T>& vec) const;

	std::array<T, n>& operator [](int index);
	const std::array<T, n>& operator [](int index) const;

	Matrix<T, m, n> operator * (const T& p);
	Matrix<T, m, n> operator / (const T& p);

	Matrix<T, n, m> Transpose() const;
	Matrix<T, m, n> Inverse() const;
};

#pragma region Construction & Deconstruction=
template<class T, int m, int n>
inline Matrix<T, m, n>::Matrix()
{
	data.fill(std::array<T, m>());
}

template<class T, int m, int n>
inline Matrix<T, m, n>::Matrix(std::initializer_list<std::initializer_list<T>> list)
{
	if (list.size() != m) {
		throw std::invalid_argument("Incorrect number of rows");
	}
	else {
		auto rowIter = list.begin();
		for (int i = 0; i < n; i++, rowIter++) {
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
inline Matrix<T, m, n> Matrix<T, m, n>::Identity()
{
	Matrix<T, m, n> res;
	int min = m<n ? m : n;
	for (int i = 0; i < min;i++) {
		res[i][i] = 1;
	}
	return res;
}
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
template<int bCol>
inline Matrix<T, m, bCol> Matrix<T, m, n>::operator*(const Matrix<T, n, bCol>& mat) const
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
inline Vector<m, T> Matrix<T, m, n>::operator*(const Vector<n, T>& vec)
{
	Vector<m, T> res = Vector<m, T>();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			res[i] += data[i][j] * vec[j];
		}
	}
	return res;
}

template<class T, int m, int n>
inline Vector<m, T> Matrix<T, m, n>::operator*(const Vector<n, T>& vec) const
{
	Vector<m, T> res = Vector<m, T>();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			res[i] += data[i][j] * vec[j];
		}
	}
	return res;
}

template<class T, int m, int n>
inline std::array<T, n>& Matrix<T, m, n>::operator[](int index)
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
	Matrix<T, n, m> transpose = Matrix<T, m, n>();;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			transpose[j][i] = data[i][j];
		}
	}

	return transpose;
}

//TODO: 
//template<class T, int m, int n>
//inline Matrix<T, m, n> Matrix<T, m, n>::Inverse() const
//{
//	if (!IsSquare()) {
//		throw std::invalid_argument("Only square matrix can be inversed");
//	}
//
//	// 对于 3x3 矩阵，使用伴随矩阵法
//	if (m == 3 && n == 3) {
//		Matrix<T, 3, 3> result;
//		T det = this->Determinant();
//		if (det == 0) {
//			throw std::invalid_argument("Matrix is singular and cannot be inverted");
//		}
//
//		result = this->Adjugate() * (1 / det);
//		return result;
//	}
//	// 对于 4x4 矩阵，使用高斯-约旦消元法
//	else if (m == 4 && n == 4) {
//		Matrix<T, 4, 4> augmentedMatrix = *this;
//		Matrix<T, 4, 4> identity = Matrix<T, 4, 4>::Identity();
//
//		// 执行高斯-约旦消元法
//		for (int i = 0; i < 4; ++i) {
//			T diagElement = augmentedMatrix[i][i];
//			if (diagElement == 0) {
//				// 交换行
//				for (int j = i + 1; j < 4; ++j) {
//					if (augmentedMatrix[j][i] != 0) {
//						std::swap(augmentedMatrix[i], augmentedMatrix[j]);
//						std::swap(identity[i], identity[j]);
//						break;
//					}
//				}
//				diagElement = augmentedMatrix[i][i];
//			}
//
//			// 归一化当前行
//			for (int col = 0; col < 4; ++col) {
//				augmentedMatrix[i][col] /= diagElement;
//				identity[i][col] /= diagElement;
//			}
//
//			// 消去其他行的当前列
//			for (int row = 0; row < 4; ++row) {
//				if (row != i) {
//					T factor = augmentedMatrix[row][i];
//					for (int col = 0; col < 4; ++col) {
//						augmentedMatrix[row][col] -= augmentedMatrix[i][col] * factor;
//						identity[row][col] -= identity[i][col] * factor;
//					}
//				}
//			}
//		}
//
//		return identity;
//	}
//	else {
//		throw std::invalid_argument("Matrix inversion only implemented for 3x3 and 4x4 matrices");
//	}
//}

//// 3x3矩阵的行列式
//template<class T>
//inline T Matrix<T, 3, 3>::Determinant() const {
//	return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
//		data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
//		data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
//}
//
//// 3x3矩阵的伴随矩阵
//template<class T>
//inline Matrix<T, 3, 3> Matrix<T, 3, 3>::Adjugate() const {
//	Matrix<T, 3, 3> adjugate;
//	adjugate[0][0] = data[1][1] * data[2][2] - data[1][2] * data[2][1];
//	adjugate[0][1] = data[0][2] * data[2][0] - data[0][0] * data[2][2];
//	adjugate[0][2] = data[0][1] * data[1][0] - data[0][0] * data[1][1];
//
//	adjugate[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
//	adjugate[1][1] = data[0][0] * data[2][2] - data[0][2] * data[2][0];
//	adjugate[1][2] = data[0][1] * data[1][0] - data[0][0] * data[1][1];
//
//	adjugate[2][0] = data[1][0] * data[2][1] - data[1][1] * data[2][0];
//	adjugate[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
//	adjugate[2][2] = data[0][0] * data[1][1] - data[0][1] * data[1][0];
//
//	return adjugate;
//}

#pragma endregion

typedef Matrix<int, 3, 3> Matrix3X3i;
typedef Matrix<int, 4, 4> Matrix4X4i;

typedef Matrix<float, 3, 3> Matrix3X3f;
typedef Matrix<float, 4, 4> Matrix4X4f;

typedef Matrix<double, 3, 3> Matrix3X3d;
typedef Matrix<double, 4, 4> Matrix4X4d;