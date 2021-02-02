#include "Vector.h"

Vector::Vector(float x, float y): x(x), y(y)
{
}

float Vector::magnitude()
{
    return sqrtf(x * x + y * y);
}

void Vector::normalise()
{
    float normal = magnitude();
    x /= normal;
    y /= normal;
}

void Vector::setMagnitude(float scaler)
{
    normalise();
    *this *= scaler;
}

// NOT USED?? REMOVE?
void Vector::limit(float _limiter)
{
    if (!b_limit)
    {
        b_limit = true;
        limiter = _limiter;
    }
    if (limiter == 0)
    {
        x = 0;
        y = 0;
    }
    if (limiter > 0)
    {
        if (x > limiter)
        {
            x = limiter;
        }
        if (y > limiter)
        {
            y = limiter;
        }
    }
    if (limiter < 0)
    {
        if (x < limiter)
        {
            x = limiter;
        }
        if (y < limiter)
        {
            y = limiter;
        }
    }
    
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
    return Vector(x*scaler, y*scaler);
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
