#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"

int32_t main()
{
    constexpr int32_t window_width = 2000;
    constexpr int32_t window_height = 1000;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Particle Simulation",
                            sf::Style::Default, sf::State::Windowed, settings);

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Renderer renderer(window);
    Solver solver(window_width, window_height);

    for (float i = 0; i < 20; i++)
    {
        float x = i * 30.0f;
        for (float j = 0; j < 20; j++)
        {
            float y = j * 30.0f;
            std::cout << x << " ," << y << "\n";
            solver.addParticle({x, y}, 10.0f, {10000.0f, 0.0f});
        }
    }

    for (float i = 0; i < 5; i++)
    {
        float x = window_width - i * 30.0f;
        for (float j = 0; j < 20; j++)
        {
            float y = j * 30.0f;
            std::cout << x << " ," << y << "\n";
            solver.addParticle({x, y}, 10.0f, {-10000.0f, 0.0f});
        }
    }

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