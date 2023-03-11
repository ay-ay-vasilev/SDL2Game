#pragma once
#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);

	Vector2D& operator+(const Vector2D& v2);
	Vector2D& operator-(const Vector2D& v2);
	Vector2D& operator*(const Vector2D& v2);
	Vector2D& operator/(const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	Vector2D& operator*(const float& i);
	Vector2D& Zero();
	Vector2D& Normalize();

	static float Distance(const Vector2D& v1, const Vector2D& v2);
	static Vector2D VectorBetween(const Vector2D& pos1, const Vector2D& pos2);
};

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);