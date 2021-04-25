#pragma once
#include <cmath>

class Vector
{
	float x, y;

public:
	// Constructor
	Vector(float x = 0, float y = 0);

	// Functions
	float getX() const;
	float getY() const;
	void setX(float _x);
	void setY(float _y);
	float magnitude();
	void normalise();
	void setMagnitude(float scaler);

	Vector operator = (Vector rhs);
	Vector operator + (Vector rhs) const;
	Vector operator - (Vector rhs) const;
	Vector operator * (float scaler) const;
	Vector operator / (float scaler) const;
	Vector& operator *= (float scaler);
	Vector& operator /= (float scaler);
	Vector& operator += (Vector& rhs);
	Vector& operator -= (Vector& rhs);
};
