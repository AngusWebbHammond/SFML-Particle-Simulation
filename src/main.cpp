#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"

int32_t main()
{
    constexpr int32_t window_width = 840;
    constexpr int32_t window_height = 840;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Particle Simulation",
                            sf::Style::Default, sf::State::Windowed, settings);

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    Renderer renderer(window);
    Solver solver;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        solver.update();
        window.clear(sf::Color::White);
        renderer.update(solver);
        window.display();
    }
    return 0;
}