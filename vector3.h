#ifndef VECTOR_H
#define VECTOR_H

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3(float _x, float _y, float _z) : x{ _x }, y{ _y }, z{ _z } {}

	//// Vector addition assignment
	//Vector3& operator+=(const Vector3& other) {
	//	x += other.x;
	//	y += other.y;
	//	z += other.z;
	//	return *this;
	//}

	//// Scaler addition assignment
	//Vector3& operator+=(float num) {
	//	x += num;
	//	y += num;
	//	z += num;
	//	return *this;
	//}

	//// Vector subtraction assignment
	//Vector3& operator-=(const Vector3& other) {
	//	x -= other.x;
	//	y -= other.y;
	//	z -= other.z;
	//	return *this;
	//}

	//// Scaler subtraction assignment
	//Vector3& operator-=(float num) {
	//	x -= num;
	//	y -= num;
	//	z -= num;
	//	return *this;
	//}

	Vector3 cross(const Vector3 other) const {
		return {
			y*other.z-z*other.y,
			z*other.x-x*other.z,
			x*other.y-y*other.x
		};
	}
};

// Vector addition
Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.x
	};
}

// Scaler addition
Vector3 operator+(const Vector3& v, float num) {
	return {
		v.x + num,
		v.y + num,
		v.z + num
	};
}
Vector3 operator+(float num, const Vector3& vector) { return vector + num; }

// Vector subtraction
Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.x
	};
}

// Scaler subtraction
Vector3 operator-(const Vector3& v, float num) {
	return v + (-num);
}

// Dot product of two vectors
float operator*(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Scaler multiplication
Vector3 operator*(const Vector3& v, float num) {
	return {
		v.x * num,
		v.y * num,
		v.z * num
	};
}
Vector3 operator*(float num, const Vector3& v) { return v * num; }

// Scaler division
Vector3 operator/(const Vector3& v, float num) {
	return {
		v.x / num,
		v.y / num,
		v.z / num
	};
}
Vector3 operator/(float num, const Vector3& v) { return v / num; }

#endif