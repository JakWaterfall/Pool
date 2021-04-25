#include "Vector.h"

Vector::Vector(float x, float y) : x(x), y(y)
{
}

float Vector::getX() const
{
	return x;
}

float Vector::getY() const
{
	return y;
}

void Vector::setX(float _x)
{
	x = _x;
}

void Vector::setY(float _y)
{
	y = _y;
}

///	Calculates the magnitude of a vector.
/// The Squire root of x squired + y squired.
float Vector::magnitude()
{
	return sqrtf(x * x + y * y);
}

/// Normalises a vector thereby making its magnitude (length) 1.
/// Changes the vector into a unit vector by calculating the magnitude
/// and dividing both the x and y values by the magnitude.
void Vector::normalise()
{
	float normal = magnitude();
	x /= normal;
	y /= normal;
}

///	Sets the magnitude of a vector by normalising the magnitude (setting it to 1)
/// and times the magnitude by a scaler value thereby setting the magnitude to the scaler value.
void Vector::setMagnitude(float scaler)
{
	normalise();
	*this *= scaler;
}

Vector Vector::operator=(Vector rhs)
{
	if (this != &rhs)
	{
		x = rhs.getX();
		y = rhs.getY();
	}
	return *this;
}

Vector Vector::operator+(Vector rhs) const
{
	return Vector(x + rhs.getX(), y + rhs.getY());
}

Vector Vector::operator-(Vector rhs) const
{
	return Vector(x - rhs.getX(), y - rhs.getY());
}

Vector Vector::operator*(float scaler) const
{
	return Vector(x * scaler, y * scaler);
}

Vector Vector::operator/(float scaler) const
{
	return Vector(x / scaler, y / scaler);
}

Vector& Vector::operator*=(float scaler)
{
	this->x *= scaler;
	this->y *= scaler;

	return *this;
}

Vector& Vector::operator/=(float scaler)
{
	this->x /= scaler;
	this->y /= scaler;

	return *this;
}

Vector& Vector::operator+=(Vector& rhs)
{
	this->x += rhs.getX();
	this->y += rhs.getY();

	return *this;
}

Vector& Vector::operator-=(Vector& rhs)
{
	this->x -= rhs.getX();
	this->y -= rhs.getY();

	return *this;
}