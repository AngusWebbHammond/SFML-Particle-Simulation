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
        Particle particles = solver.getParticles();
        circle.setPointCount(32);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition(particles.position);
        target.draw(circle);
    }

private:
    sf::RenderTarget &target;
};
