#pragma once
#include <cmath>

class Vector
{
	float x, y;
	bool b_limit = false;
	float limiter = 0;
public:
	Vector(float x = 0, float y = 0);

	float getX() const { return x; }
	float getY() const { return y; }
	void setX(float _x) { x = _x; }
	void setY(float _y) { y = _y; }
	float magnitude();
	void normalise();
	void setMagnitude(float scaler);
	void limit(float limiter = 0);

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

