#include <SFML/Graphics.hpp>

struct Particle
{
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    int particleNum;
    int indexValue = 0;
    sf::Vector2i posIndex;

    Particle() = default;
    Particle(sf::Vector2f position_, float radius_, int particleNum_)
        : last_position{position_},
          position{position_},
          acceleration{5000.0f, 0.0f},
          radius{radius_},
          particleNum{particleNum_} {}

    void update(float dt)
    {
        // Verlet Intergration
        sf::Vector2f displacement = position - last_position;
        last_position = position;
        position = position + displacement + acceleration * dt * dt;
        acceleration = {};
    }

    sf::Vector2f getVelocity()
    {
        sf::Vector2f velocity = position - last_position;
        return velocity;
    }

    void addVelcity(sf::Vector2f v, float dt)
    {
        last_position -= v * dt;
    }

    void setVelocity(sf::Vector2f v, float dt)
    {
        last_position = position - v * dt;
    }

    void accelerate(sf::Vector2f a)
    {
        acceleration += a;
    }
};