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
        for (auto &particle : particles)
        {
            circle.setRadius(particle.radius);
            circle.setOrigin({particle.radius, particle.radius});

            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(particle.position);
            target.draw(circle);
        }
    }

private:
    sf::RenderTarget &target;
};
