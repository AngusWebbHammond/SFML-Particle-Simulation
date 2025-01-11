#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <time.h>

int32_t main()
{
    constexpr int32_t window_width = 1000;
    constexpr int32_t window_height = 1000;

    clock_t c = clock();

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Particle Simulation",
                            sf::Style::Default, sf::State::Windowed, settings);

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Renderer renderer(window);
    Solver solver(window_width, window_height);

    const int maxParticles = 500;
    const clock_t spawnDelay = CLOCKS_PER_SEC * 0.1;
    const sf::Vector2f spawnPosition = {10, 10};

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (solver.getParticles().size() < maxParticles && clock() - c > spawnDelay)
        {
            float randomVel = rand() % 600;
            float randomSize = rand() % 20;
            solver.addParticle(spawnPosition, 15, {0, 0});
            solver.setVelocity({randomVel, 0.0f});
            c = clock();
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.update(solver);
        window.display();
    }
    return 0;
}