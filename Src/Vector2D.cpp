#include "Vector2D.h"

// Default constructor: initializes the Vector2D with zero values
Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

// Constructor: initializes the Vector2D with specified x and y values
Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

// Add a vector to the current vector
Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

// Subtract a vector from the current vector
Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

// Multiply the current vector by another vector
Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

// Divide the current vector by another vector
Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

// Overload the + operator to add two vectors
Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}

// Overload the - operator to subtract two vectors
Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtract(v2);
}

// Overload the * operator to multiply two vectors
Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiply(v2);
}

// Overload the / operator to divide two vectors
Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Divide(v2);
}

// Overload the += operator to add a vector to the current vector
Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}

// Overload the -= operator to subtract a vector from the current vector
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);
}

// Overload the *= operator to multiply the current vector by another vector
Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);
}

// Overload the /= operator to divide the current vector by another vector
Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);
}

// Overload the * operator to multiply the vector by an integer
Vector2D& Vector2D::operator*(const int& i)
{
	this->x = i;
	this->y = i;
	return *this;
}

// Set the vector values to zero
Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

// Overload the << operator to output the vector values
std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}
