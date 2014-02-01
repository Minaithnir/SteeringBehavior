#include <SFML/Graphics.hpp>
#include "Unit.hpp"
#include <iostream>
#include <time.h>

#define WIDTH 1900
#define HEIGHT 1000
#define DEMO_UNIT_COUNT 1000

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    sf::Clock frameClock;
    float frameTime = 0;
    Unit target;

    std::vector<Unit> units;
    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
    {
        units.push_back(Unit());
        units.back().setPosition(Vector(rand()%WIDTH, rand()%HEIGHT));
        units.back().target = &target;
        units.back().setUnitBehavior(SEEK);
    }


    while (window.isOpen())
    {
sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed :
                window.close();
                break;
            case sf::Event::MouseButtonPressed :
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Left :
                    break;
                default :
                    break;
                }
                break;
            case sf::Event::KeyPressed :
                switch(event.key.code)
                {
                case sf::Keyboard::S :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setUnitBehavior(SEEK);
                    }
                    break;
                case sf::Keyboard::F :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setUnitBehavior(FLEE);
                    }
                    break;
                case sf::Keyboard::W :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setUnitBehavior(WANDER);
                    }
                    break;
                case sf::Keyboard::P :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setUnitBehavior(PURSUE);
                    }
                    break;
                case sf::Keyboard::E :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setUnitBehavior(EVADE);
                    }
                    break;
                case sf::Keyboard::R :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setPosition(Vector(rand()%WIDTH, rand()%HEIGHT));
                    }
                    break;
                default :
                    break;
                }
                break;
            default :
                break;
            }
        }

        Vector tOldPosition = target.getPosition();
        target.setPosition(Vector(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
        target.setVelocity(target.getPosition()-tOldPosition);

        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].update();
        }

        window.clear(sf::Color::White);

        frameTime = frameClock.getElapsedTime().asSeconds();
        frameClock.restart();
        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].updatePos(frameTime);
            Vector v = units[i].getPosition() - target.getPosition();
            if(v.norme() < 10)
            {
                units[i].setPosition(Vector(rand()%WIDTH, rand()%HEIGHT));
            }
            units[i].draw(window);
        }

        window.display();
    }

    return 0;
}
