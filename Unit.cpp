#include "Unit.hpp"

#include <iostream>

Unit::Unit() :
    target(NULL),
    m_position(Vector(0,0)),
    m_velocity(Vector(1,-1)),
    m_maxSpeed(MAX_SPEED),
    m_maxForce(MAX_FORCE),
    m_mass(DEFAULT_MASS),
    m_radius(RADIUS),
    m_arrival(true),
    m_waypointIndex(0)
{
    m_drawing.setPointCount(3);
    m_drawing.setPoint(0, sf::Vector2f(-5,5));
    m_drawing.setPoint(1, sf::Vector2f(-5,-5));
    m_drawing.setPoint(2, sf::Vector2f(10,0));
    m_drawing.setFillColor(sf::Color::Black);
}

Unit::~Unit()
{
}

/**
    défini la couleur du vehicule
*/
void Unit::setColor(sf::Color color)
{
    m_drawing.setFillColor(color);
}

/**
    implémentation de la fonction virtuelle de mise à jour de l'objet
*/
void Unit::update()
{
    if(target != NULL)
    {
        switch (m_behavior)
        {
        case SEEK :
            m_steering = seek(target->getPosition());
            break;
        case FLEE :
            m_steering = flee(target->getPosition());
            break;
        case PURSUE :
            m_steering = pursue((*target));
            break;
        case EVADE :
            m_steering = evade((*target));
            break;
        default :
            m_steering = wander();
            break;
        }
    }
    else
    {
        m_steering = wander();
    }
}

void Unit::updatePos(const float& frameTime)
{
    m_steering.truncate(m_maxForce*frameTime);

    m_velocity += (m_steering/m_mass);
    m_velocity.truncate(m_maxSpeed*frameTime);

    m_position += m_velocity;

    m_steering = Vector(0,0);
}

/**
    dessin de l'objet sur la renderTarget passée en argument
*/
void Unit::draw(sf::RenderTarget& App)
{
    m_drawing.setPosition(m_position);
    m_drawing.setRotation(m_velocity.getAngle());
    App.draw(m_drawing);
}

/// helpers ///

/**
    retourne la position de l'unité dans le futur
*/
Vector Unit::getFuturePosition(const float& futureTime)
{
    return m_position + m_velocity*futureTime;
}


///Steering Behaviors///

/**
    balade
*/
Vector Unit::wander()
{
    Vector centre = m_velocity;
    centre.normalize();
    centre.setLengh(3*m_maxForce);/// on défini le centre du cercle

    Vector random((rand()%101)-50, (rand()%101)-50);
    random.truncate(m_maxForce); /// on crée une force aléatoire;

    m_wanderFactor += random;
    m_wanderFactor.truncate(m_maxForce); /// on l'ajoute à l'ancienne force de wander

    Vector steering = m_wanderFactor+centre;
    steering.truncate(m_maxForce);

    return steering;
}

/**
    cherche à atteindre la cible
*/
Vector Unit::seek(const Vector& target, const float& offset)
{
    Vector _target = target - m_position;
    _target.setLengh(_target.norme()-offset);
    _target = _target + m_position;

    Vector desiredVelocity, steering;

    desiredVelocity = _target - m_position;
    if(desiredVelocity.norme() > APPROX_ARRIVAL)
    {
        if(m_arrival)
        {
            float slowDist = SLOW_DIST_FACTOR * m_maxSpeed * m_mass;
            float ramped_speed = m_maxSpeed * (desiredVelocity.norme() / slowDist);
            float clipped_speed;
            if (ramped_speed < m_maxSpeed)
            {
                clipped_speed = ramped_speed;
            }
            else
                clipped_speed = m_maxSpeed;

            desiredVelocity.setLengh(clipped_speed);
        }


        steering = desiredVelocity - m_velocity;
        steering.truncate(m_maxForce);
    }
    else
    {
        steering -= m_velocity;
        steering.truncate(m_maxForce);
    }

    return steering;
}

/**
    cherche à s'éloigner du point cible
*/
Vector Unit::flee(const Vector& target)
{
    Vector desiredVelocity, steering;

    desiredVelocity = -(target - m_position);
    desiredVelocity.normalize();
    desiredVelocity.setLengh(m_maxSpeed);
    steering = desiredVelocity - m_velocity;
    steering.truncate(m_maxForce);

    return steering;
}

/**
    cherche à attrapper une unité
*/
Vector Unit::pursue(Unit& target, const float& offset)
{
    Vector diff = target.getPosition() - m_position;
    float distance = diff.norme();
    float anticipation_time=0;
    if(target.getVelocity().norme()>0)
        anticipation_time = distance / target.getVelocity().norme();

    Vector futur_position = target.getPosition() + (target.getVelocity()*anticipation_time/2.f);
    return seek(futur_position, offset);
}

/**
    cherche à éviter une unité
*/
Vector Unit::evade(Unit& target)
{
    Vector diff = target.getPosition() - m_position;
    float distance = diff.norme();
    float anticipation_time=0;
    if(target.getVelocity().norme()>0)
        anticipation_time = distance / target.getVelocity().norme();

    Vector futur_position = target.getPosition() + (target.getVelocity()*anticipation_time/2.f);
    return flee(futur_position);
}

/**
    calcule la force necessaire pour éviter les collision à venir avec d'autres unitées
*/
Vector Unit::unallignedCollisionAvoidance(std::vector<Unit*>& others)
{
    Vector steering(0,0);
    int density = 0;

    for(unsigned int i=0; i<others.size(); i++)
    {
        if(others[i] != this)
        {
            /// si les deux unité sont assez proches
            if(Vector(others[i]->getPosition()-m_position).norme() < 50.f)
            {
                Vector futurDiff(others[i]->getFuturePosition(UCATIME) - getFuturePosition(UCATIME));
                if(futurDiff.norme() < 1.5*m_radius)
                {
                    if(futurDiff.norme()>m_radius)
                        futurDiff.setLengh(futurDiff.norme()-m_radius);
                    steering -= futurDiff;
                    density++;
                }
            }
        }
    }

    return steering;
}
