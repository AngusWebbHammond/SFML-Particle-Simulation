#pragma once
#include <string>
#include "solver.hpp"

class Renderer
{
public:
    Renderer(sf::RenderTarget &_target) : target{_target} {}

    void update(Solver &solver) const
    {

        sf::CircleShape circle(10.0f);
        std::vector<Particle> particles = solver.getParticles();
        circle.setPointCount(32);

        sf::Font font("C:/Windows/Fonts/arial.ttf");
        sf::Text particleNum(font);
        particleNum.setFillColor(sf::Color::Black);

        for (auto &particle : particles)
        {
            circle.setRadius(particle.radius);
            circle.setOrigin({particle.radius, particle.radius});
            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(particle.position);
            target.draw(circle);

            particleNum.setPosition(particle.position);
            particleNum.setString(std::to_string(particle.particleNum));
            target.draw(particleNum);
        }
    }

private:
    sf::RenderTarget &target;
};
