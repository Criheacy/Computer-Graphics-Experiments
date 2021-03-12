#pragma once

#include <vector>

class Vector3
{
public:

	double x, y, z;

	Vector3()
	{
		x = 0; y = 0; z = 0;
	}

	Vector3(double x, double y)
	{
		this->x = x; this->y = y; this->z = 0;
	}

	Vector3(double x, double y, double z)
	{
		this->x = x; this->y = y; this->z = z;
	}

	// Use for certain OpenGL program
	Vector3(std::pair<float, float> from, std::pair<float, float> to)
	{
		this->x = (double)to.first - from.first;
		this->y = (double)to.second - from.second;
		this->z = 0;
	}

	Vector3 operator + (Vector3 rhs)
	{
		return Vector3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
	}

	Vector3 operator - (Vector3 rhs)
	{
		return Vector3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	double operator * (Vector3 rhs)
	{
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	}

	double modulus()
	{
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	Vector3 operator ^ (Vector3 rhs)
	{
		return Vector3(this->y * rhs.z - this->z * rhs.y,
			this->z * rhs.x - this->x * rhs.z,
			this->x * rhs.y - this->y * rhs.x);
	}
	
	static double InnerProduct(Vector3 a, Vector3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Vector3 OuterProduct(Vector3 a, Vector3 b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
};