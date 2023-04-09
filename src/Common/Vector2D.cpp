#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D() : x(0.f), y(0.f) {}
Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	x += vec.x; y += vec.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	x -= vec.x; y -= vec.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	x *= vec.x; y *= vec.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	x /= vec.x; y /= vec.y;
	return *this;
}

Vector2D Vector2D::operator+(const Vector2D& v2) const
{
	return Vector2D(x + v2.x, y + v2.y);
}

Vector2D Vector2D::operator-(const Vector2D& v2) const
{
	return Vector2D(x - v2.x, y - v2.y);
}

Vector2D Vector2D::operator*(const Vector2D& v2) const
{
	return Vector2D (x * v2.x, y * v2.y);
}

Vector2D Vector2D::operator/(const Vector2D& v2) const
{
	return Vector2D(x / v2.x, y / v2.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);
}

Vector2D& Vector2D::operator*(const float& i)
{
	x *= i; y *= i;
	return *this;
}

Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

Vector2D& Vector2D::Normalize()
{
	float length = sqrt(x * x + y * y);
	if (length != 0.0f)
	{
		x /= length;
		y /= length;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}

float Vector2D::Distance(const Vector2D& v1, const Vector2D& v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return std::sqrt(dx * dx + dy * dy);
}

Vector2D Vector2D::VectorBetween(const Vector2D& pos1, const Vector2D& pos2)
{
	float x = pos2.x - pos1.x;
	float y = pos2.y - pos1.y;
	return Vector2D(x, y);
}