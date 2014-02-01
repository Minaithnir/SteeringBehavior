#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

class Vector : public sf::Vector2f
{
    public:
        Vector();
        Vector(float X, float Y);
        Vector(const Vector& v);
        Vector(const sf::Vector2f& v);
        ~Vector();

        float norme();
        Vector setLengh(const float& lengh);

        float getAngle();
        Vector setAngle(const float& angle);

        Vector normalize();

        Vector truncate(const float& maxLengh);

        float dot(Vector& vect);
        Vector cross();

    protected:
    private:
};

std::ostream& operator<<( std::ostream &flux, Vector const& vect );

#endif // VECTOR_HPP
