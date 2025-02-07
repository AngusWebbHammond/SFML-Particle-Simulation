#include <iostream>
#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <time.h>

int32_t main()
{
    // sf::Clock clock_frames = sf::Clock();

    // Grid grid;
    // sf::Vector2i pos = {1, 2};

    // grid.createGrid(10, 10);
    // grid.addNode(pos, 10);
    // grid.addNode(pos, 20);

    // grid.printGrid();
    // sf::Time previousTime = clock_frames.getElapsedTime();

    // grid.getIndex({120.343, 23.5678});

    // sf::Time currentTime = clock_frames.getElapsedTime();

    // std::cout << currentTime.asSeconds() - previousTime.asSeconds() << "\n";

    constexpr int32_t window_width = 1000;
    constexpr int32_t window_height = 1000;

    constexpr sf::Vector2i gridSize = {50, 50};

    clock_t c = clock();
    float fps;
    sf::Clock clock_frames = sf::Clock();
    sf::Time previousTime = clock_frames.getElapsedTime();
    sf::Time currentTime;

    sf::Font font("C:/Windows/Fonts/arial.ttf");

    sf::Text fpsCounter(font);
    fpsCounter.setFillColor(sf::Color::Black);
    fpsCounter.setPosition({0.f, 0.f});

    sf::Text particleCount(font);
    particleCount.setFillColor(sf::Color::Black);
    particleCount.setPosition({0.f, 25.f});

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 1;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Particle Simulation",
                            sf::Style::Default, sf::State::Windowed, settings);

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Renderer renderer(window);
    Solver solver(window_width, window_height);
    solver.initialiseSolverGrid(gridSize);

    const int maxParticles = 100;
    const clock_t spawnDelay = CLOCKS_PER_SEC * 0.1;
    const sf::Vector2f spawnPosition = {10, 10};

    int particleNum = 0;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (solver.getParticles().size() < maxParticles && clock() - c > spawnDelay)
        {
            particleNum++;
            float randomVel = rand() % 600;
            float randomSize = rand() % 20;
            solver.addParticle(spawnPosition, 10, {0, 0}, particleNum);
            solver.setVelocity({randomVel, 0.0f});
            c = clock();
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.update(solver);

        currentTime = clock_frames.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        fpsCounter.setString("FPS = " + std::to_string(round(fps)));

        particleCount.setString("Particles: " + std::to_string(solver.getParticles().size()));
        window.draw(fpsCounter);
        window.draw(particleCount);

        previousTime = currentTime;
        window.display();
    }
    return 0;
}