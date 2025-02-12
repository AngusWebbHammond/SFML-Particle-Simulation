#pragma once
#include <string>
#include "solver.hpp"
#include <iostream>

class Renderer
{
public:
    Renderer(sf::RenderTarget &_target) : target{_target} {}

    void createGrid(sf::Vector2i size, sf::Vector2f windowSize)
    {
        sf::Vector2f boxSize = {windowSize.x / size.x, windowSize.y / size.y};
        for (int i = 0; i < size.x; i++)
        {
            for (int j = 0; j < size.y; j++)
            {
                centers.push_back({i * boxSize.x + boxSize.x / 2, j * boxSize.y + boxSize.y / 2});
            }
        }
    }

    void update(Solver &solver)
    {

        sf::CircleShape circle(10.0f);
        particles = solver.getParticles();
        circle.setPointCount(32);

        // drawGrid();

        for (auto &particle : particles)
        {
            circle.setRadius(particle.radius);
            circle.setOrigin({particle.radius, particle.radius});
            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(particle.position);
            target.draw(circle);
        }

        // addParticleNum();
    }

private:
    sf::RenderTarget &target;

    std::vector<sf::Vector2f> centers;

    std::vector<Particle> particles;

    void addParticleNum()
    {
        sf::Font font("C:/Windows/Fonts/arial.ttf");
        sf::Text particleNum(font);
        particleNum.setFillColor(sf::Color::Black);

        for (auto &particle : particles)
        {
            particleNum.setPosition(particle.position);
            particleNum.setString(std::to_string(particle.particleNum));
            target.draw(particleNum);
        }
    }

    void drawGrid()
    {
        sf::RectangleShape rect({10.0f, 10.0f});

        rect.setOutlineColor(sf::Color(0, 0, 0));
        rect.setOutlineThickness(2.0f);

        for (auto box : centers)
        {
            rect.setPosition(box);
            rect.setOrigin({5, 5});
            if (particles.back().posIndex.x == (box.x - 5) / 10 && particles.back().posIndex.y == (box.y - 5) / 10)
            {
                rect.setFillColor(sf::Color::Red);
            }
            target.draw(rect);
            rect.setFillColor(sf::Color::White);
        }
    }
};
