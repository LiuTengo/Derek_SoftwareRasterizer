#pragma once

#include <cassert>
#include <iostream>
#include <math.h>
#include <array>

#pragma region Vector

template <int n, class T>
class Vector {
public:
	std::array<T, n> data;

	Vector();
	Vector(std::initializer_list<T> values);
	~Vector();

public:
	T& operator[] (const int& index) {
		assert(0 <= index && index < n);
		return data[index];
	}
	T const operator[] (const int& index) const {
		assert(0 <= index && index < n);
		return data[index];
	}
	Vector<n, T> operator+ (const Vector<n, T>& vec) {
		Vector<n, T> res = Vector();
		for (int i = 0; i < n; i++) {
			res[i] = data[i] + vec[i];
		}
		return res;
	};
	Vector<n, T> operator- (const Vector<n, T>& vec) {
		Vector<n, T> res = Vector();
		for (int i = 0; i < n; i++) {
			res[i] = data[i] - vec[i];
		}
		return res;
	};
	Vector<n, T> operator* (const Vector<n, T>& vec) {
		Vector<n, T> res = Vector();
		for (int i = 0; i < n; i++) {
			res[i] = data[i] * vec[i];
		}
		return res;
	};
	Vector<n, T>& operator*= (const Vector<n, T>& vec) {
		for (int i = 0; i < n; i++) {
			data[i] *= vec[i];
		}
		return (*this);
	};
	Vector<n, T>& operator*= (const T& scaler) {
		for (int i = 0; i < n; i++) {
			data[i] *= scaler;
		}
		return (*this);
	};

	friend std::ostream& operator<< (std::ostream& out, const Vector<n, T>& vec) {
		for (int i = 0; i < n; i++) {
			out << vec[i] << " ";
		}
		out << std::endl;
		return out;
	}
	friend Vector<n, T> operator* (const T& f, const Vector<n, T>& vec) {
		Vector<n, T> res = Vector();
		for (int i = 0; i < n; i++) {
			res[i] = vec[i] * f;
		}
		return res;
	};
	friend Vector<n, T> operator/ (const Vector<n, T>& vec, const T& f) {
		Vector<n, T> res = Vector();
		for (int i = 0; i < n; i++) {
			res[i] = vec[i] / f;
		}
		return res;
	};

public:
	static Vector<n, T> normalize(const Vector<n, T>& vec);

	T dot(const Vector<n, T>& vec);
	void normalize();
};

template<int n, class T>
inline Vector<n, T>::Vector()
{
	if (n <= 0) {
		throw std::invalid_argument("Vector dimension must be greater than 0");
	}
	data = std::array<T, n>();
}

template<int n, class T>
inline Vector<n, T>::Vector(std::initializer_list<T> values)
{
	if (n <= 0) {
		throw std::invalid_argument("Vector dimension must be greater than 0");
	}
	data = std::array<T, n>();
	int i = 0;
	for (const auto& val : values) {
		data[i++] = val;
	}
}

template<int n, class T>
inline Vector<n, T>::~Vector()
{
	//delete[] data;
}

template<int n, class T>
inline Vector<n, T> Vector<n, T>::normalize(const Vector<n, T>& vec)
{
	Vector<n, T> res = Vector<n, T>();
	T length = 0;
	for (int i = 0; i < n; i++) {
		length += vec[i] * vec[i];
	}
	length = sqrt(length);
	for (int i = 0; i < n; i++) {
		res[i] = vec[i] / length;
	}
	return res;
}

template<int n, class T>
inline T Vector<n, T>::dot(const Vector<n, T>& vec)
{
	T res = 0;
	for (int i = 0; i < n; i++) {
		res += data[i] * vec[i];
	}
	return res;
}

template<int n, class T>
inline void Vector<n, T>::normalize()
{
	T length = sqrt(this->dot(*this));
	for (int i = 0; i < n; i++) {
		(*this)[i] = (*this)[i] / length;
	}
}

#pragma endregion

#pragma region Vector<3,T>
template<class T>
class Vector<3, T> {
public:
	std::array<T, 3> data;

	Vector();
	Vector(std::initializer_list<T> values);
	Vector(const Vector& vec);
	~Vector();

	T& x() { return data[0]; }
	T& y() { return data[1]; }
	T& z() { return data[2]; }
	T const& x() const { return data[0]; }
	T const& y() const { return data[1]; }
	T const& z() const { return data[2]; }

public:
	T& operator[] (const int& index) {
		assert(0 <= index && index < 3);
		return data[index];
	}
	T const operator[] (const int& index) const {
		assert(0 <= index && index < 3);
		return data[index];
	}
	Vector<3, T> operator+ (const Vector<3, T>& vec) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = data[i] + vec[i];
		}
		return res;
	};
	Vector<3, T> operator- (const Vector<3, T>& vec) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = data[i] - vec[i];
		}
		return res;
	};
	Vector<3, T> operator* (const Vector<3, T>& vec) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = data[i] * vec[i];
		}
		return res;
	};
	Vector<3, T>& operator*= (const Vector<3, T>& vec) {
		for (int i = 0; i < 3; i++) {
			data[i] *= vec[i];
		}
		return (*this);
	};
	Vector<3, T>& operator*= (const T& scaler) {
		for (int i = 0; i < 3; i++) {
			data[i] *= scaler;
		}
		return (*this);
	};

	friend std::ostream& operator<< (std::ostream& out, const Vector<3, T>& vec) {
		for (int i = 0; i < 3; i++) {
			out << vec[i] << " ";
		}
		out << std::endl;
		return out;
	}
	friend Vector<3, T> operator* (const T& f, const Vector<3, T>& vec) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = vec[i] * f;
		}
		return res;
	};
	friend Vector<3, T> operator/ (const Vector<3, T>& vec, const T& f) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = vec[i] / f;
		}
		return res;
	};

public:
	static Vector<3, T> normalize(const Vector<3, T>& vec);
	Vector<3, T> cross(const Vector<3, T>& vec);
	T dot(const Vector<3, T>& vec);
	void normalize();
	Vector<4, T> expand() const;
};

template<class T>
inline Vector<3, T>::Vector()
{
	data.fill(T());
}

template<class T>
inline Vector<3, T>::Vector(std::initializer_list<T> values)
{
	data.fill(T());
	int i = 0;
	for (const auto& val : values) {
		data[i++] = val;
	}
}

template<class T>
inline Vector<3, T>::Vector(const Vector& vec)
{
	data = vec.data;
}

template<class T>
inline Vector<3, T>::~Vector()
{
	std::cout << "Delete Vector" << std::endl;
	std::cout << data[0] << " " << data[1] << " " << data[2] << std::endl;
}

template<class T>
inline Vector<3, T> Vector<3, T>::normalize(const Vector<3, T>& vec)
{
	Vector<3, T> res = Vector<3, T>();
	T length = 0;
	for (int i = 0; i < 3; i++) {
		length += vec[i] * vec[i];
	}
	length = sqrt(length);
	for (int i = 0; i < 3; i++) {
		res[i] = vec[i] / length;
	}
	return res;
}

template<class T>
inline Vector<3, T> Vector<3, T>::cross(const Vector<3, T>& vec)
{
	T x = data[1] * vec[2] - data[2] * vec[1];
	T y = -data[0] * vec[2] + data[2] * vec[0];
	T z = data[0] * vec[1] - data[1] * vec[0];
	return Vector<3, T>{x, y, z};
}

template<class T>
inline T Vector<3, T>::dot(const Vector<3, T>& vec)
{
	T res = 0;
	for (int i = 0; i < 3; i++) {
		res += data[i] * vec[i];
	}
	return res;
}

template<class T>
inline void Vector<3, T>::normalize()
{
	T length = sqrt(this->dot(*this));
	for (int i = 0; i < 3; i++) {
		(*this)[i] = (*this)[i] / length;
	}
}
template<class T>
inline Vector<4, T> Vector<3, T>::expand() const
{
	return Vector<4, T>{data[0], data[1], data[2], 1};
}
#pragma endregion


#pragma region Vector<4,T>
template<class T>
class Vector<4, T> {
public:
	std::array<T, 4> data;

	Vector();
	Vector(std::initializer_list<T> values);
	Vector(const Vector& vec);
	~Vector();

	T& x() { return data[0]; }
	T& y() { return data[1]; }
	T& z() { return data[2]; }
	T& w() { return data[3]; }
	T const& x() const { return data[0]; }
	T const& y() const { return data[1]; }
	T const& z() const { return data[2]; }
	T const& w() const { return data[3]; }

public:
	T& operator[] (const int& index) {
		assert(0 <= index && index < 4);
		return data[index];
	}
	T const operator[] (const int& index) const {
		assert(0 <= index && index < 4);
		return data[index];
	}
	Vector<4, T> operator+ (const Vector<4, T>& vec) {
		Vector<4, T> res = Vector();
		for (int i = 0; i < 4; i++) {
			res[i] = data[i] + vec[i];
		}
		return res;
	};
	Vector<3, T> operator- (const Vector<3, T>& vec) {
		Vector<3, T> res = Vector();
		for (int i = 0; i < 3; i++) {
			res[i] = data[i] - vec[i];
		}
		return res;
	};
	Vector<4, T> operator* (const Vector<4, T>& vec) {
		Vector<4, T> res = Vector();
		for (int i = 0; i < 4; i++) {
			res[i] = data[i] * vec[i];
		}
		return res;
	};
	Vector<4, T>& operator*= (const Vector<4, T>& vec) {
		for (int i = 0; i < 4; i++) {
			data[i] *= vec[i];
		}
		return (*this);
	};
	Vector<4, T>& operator*= (const T& scaler) {
		for (int i = 0; i < 4; i++) {
			data[i] *= scaler;
		}
		return (*this);
	};

	friend std::ostream& operator<< (std::ostream& out, const Vector<4, T>& vec) {
		for (int i = 0; i < 4; i++) {
			out << vec[i] << " ";
		}
		out << std::endl;
		return out;
	}
	friend Vector<4, T> operator* (const T& f, const Vector<4, T>& vec) {
		Vector<4, T> res = Vector();
		for (int i = 0; i < 4; i++) {
			res[i] = vec[i] * f;
		}
		return res;
	};
	friend Vector<4, T> operator/ (const Vector<4, T>& vec, const T& f) {
		Vector<4, T> res = Vector();
		for (int i = 0; i < 4; i++) {
			res[i] = vec[i] / f;
		}
		return res;
	};

public:
	static Vector<4, T> normalize(const Vector<4, T>& vec);
	T dot(const Vector<4, T>& vec);
	void normalize();
};

template<class T>
inline Vector<4, T>::Vector()
{
	data.fill(T());
}

template<class T>
inline Vector<4, T>::Vector(std::initializer_list<T> values)
{
	data.fill(T());
	int i = 0;
	for (const auto& val : values) {
		data[i++] = val;
	}
}

template<class T>
inline Vector<4, T>::Vector(const Vector& vec)
{
	data = vec.data;
}

template<class T>
inline Vector<4, T>::~Vector()
{
	std::cout << "Delete Vector" << std::endl;
	std::cout << data[0] << " " << data[1] << " " << data[2] << " " << data[3]<< std::endl;
}

template<class T>
inline Vector<4, T> Vector<4, T>::normalize(const Vector<4, T>& vec)
{
	Vector<4, T> res = Vector<4, T>();
	T length = 0;
	for (int i = 0; i < 4; i++) {
		length += vec[i] * vec[i];
	}
	length = sqrt(length);
	for (int i = 0; i < 4; i++) {
		res[i] = vec[i] / length;
	}
	return res;
}

template<class T>
inline T Vector<4, T>::dot(const Vector<4, T>& vec)
{
	T res = 0;
	for (int i = 0; i < 4; i++) {
		res += data[i] * vec[i];
	}
	return res;
}

template<class T>
inline void Vector<4, T>::normalize()
{
	T length = sqrt(this->dot(*this));
	for (int i = 0; i < 4; i++) {
		(*this)[i] = (*this)[i] / length;
	}
}
#pragma endregion

typedef Vector<2, float> Vector2f;
typedef Vector<3, float> Vector3f;
typedef Vector<4, float> Vector4f;

typedef Vector<2, double> Vector2d;
typedef Vector<3, double> Vector3d;
typedef Vector<4, double> Vector4d;