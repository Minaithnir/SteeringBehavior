#include "Vector.hpp"

Vector::Vector()
{
    //ctor
}

Vector::Vector(float X, float Y) : sf::Vector2f(X, Y)
{
    //ctor
}

Vector::Vector(const Vector& v) : sf::Vector2f(v)
{
    //ctor
}

Vector::Vector(const sf::Vector2f& v) : sf::Vector2f(v)
{
    //ctor
}

Vector::~Vector()
{
    //dtor
}

/** @brief Truncate
  *
  * @todo: document this function
  */
Vector Vector::truncate(const float& maxLengh)
{
    if(norme()>maxLengh)
        setLengh(maxLengh);
    return (*this);
}

/** @brief Normalize
  *
  * @todo: document this function
  */
Vector Vector::normalize()
{
    float len = norme();
    x /= len;
    y /= len;
    return (*this);
}

/** @brief SetAngle
  *
  * @todo: document this function
  */
Vector Vector::setAngle(const float& angle)
{
    float len = norme();
    x = cos(angle) * len;
    y = sin(angle) * len;

    return (*this);
}

/** @brief GetAngle
  *
  * @todo: document this function
  */
float Vector::getAngle()
{
    return atan2(y,x)/(2.f*(float)M_PI)*360.f;
}

/** @brief SetLengh
  *
  * @todo: document this function
  */
Vector Vector::setLengh(const float& lengh)
{
    normalize();
    x *= lengh;
    y *= lengh;
    return (*this);
}

/** @brief Norme
  *
  * @todo: document this function
  */
float Vector::norme()
{
    return sqrt(x*x+y*y);
}

float Vector::dot(Vector& vect)
{
    return x * vect.x + y * vect.y;
}

Vector Vector::cross()
{
    return Vector(-y,x);
}

std::ostream& operator<<( std::ostream &flux, Vector const& vect )
{
    flux << vect.x << "," << vect.y;
    return flux;
}
