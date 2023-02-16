#include "Vector2D.h"

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

Vector2D& Vector2D::operator+(const Vector2D& v2)
{
	Vector2D result = { x + v2.x, y + v2.y };
	return result;
}

Vector2D& Vector2D::operator-(const Vector2D& v2)
{
	Vector2D result = { x - v2.x, y - v2.y };
	return result;
}

Vector2D& Vector2D::operator*(const Vector2D& v2)
{
	Vector2D result = { x * v2.x, y * v2.y };
	return result;
}

Vector2D& Vector2D::operator/(const Vector2D& v2)
{
	Vector2D result = { x / v2.x, y / v2.y };
	return result;
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

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}