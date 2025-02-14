#include <math.h>
#include "grid.hpp"
#include "particle.hpp"
#include <algorithm>

class Solver
{
public:
    Solver(float width, float height) : window_width{width}, window_height{height} {}

    void initialiseSolverGrid(sf::Vector2i size)
    {
        gridSize = {size.x - 1, size.y - 1};
        grid.createGrid(size.x, size.y);
    }

    void update()
    {
        for (int i = 0; i < subStep; i++)
        {
            applyGravity();
            calculateBoundary();
            checkCollisions();
            updateParticles();
        }
    }

    std::vector<Particle> getParticles()
    {
        return particles;
    }

    void addParticle(sf::Vector2f position, float size, sf::Vector2f acceleration, int particleNum)
    {
        Particle particle(position, size, particleNum);
        particle.posIndex = grid.getIndex(particle.position, gridSize, {window_width, window_height});
        particles.push_back(particle);
        particle.acceleration = acceleration;
        int indexValue = grid.addNode(particle.posIndex, particle.particleNum);
        particle.indexValue = indexValue;
    }

    void setVelocity(sf::Vector2f velocity)
    {
        Particle &finalParticle = particles.back();
        sf::Vector2f vel = {velocity.x / subStep, velocity.y / subStep};
        finalParticle.setVelocity(vel, dt);
    }

private:
    std::vector<Particle> particles;
    Grid grid;

    sf::Vector2i gridSize;

    float dt = 1.0f / 60;
    int subStep = 8;
    sf::Vector2f gravity = {0.0f, 200.0f / subStep};

    float window_height;
    float window_width;

    void applyGravity()
    {
        for (auto &particle : particles)
        {
            particle.accelerate(gravity);
        }
    }

    void updateParticles()
    {
        sf::Vector2i pos;
        for (auto &particle : particles)
        {
            particle.update(dt);

            pos = grid.getIndex(particle.position, gridSize, {window_width, window_height});
            if (pos.x != particle.posIndex.x || pos.y != particle.posIndex.y)
            {
                particle.indexValue = grid.moveNode(particle.posIndex, pos, particle.indexValue, particle.particleNum);
                particle.posIndex = pos;
            }
        }
    }

    void calculateBoundary()
    {
        for (auto &particle : particles)
        {
            float damp = 0.7f;
            const sf::Vector2f pos = particle.position;
            sf::Vector2f vel = particle.getVelocity();
            sf::Vector2f newPosition = particle.position;
            sf::Vector2f dx = {vel.x * damp, -vel.y * damp};
            sf::Vector2f dy = {-vel.x * damp, vel.y * damp};
            if (pos.y > window_height - particle.radius || pos.y < particle.radius)
            {
                if (pos.y < particle.radius)
                    newPosition.y = particle.radius;
                if (pos.y + particle.radius > window_height)
                    newPosition.y = window_height - particle.radius;
                particle.position = newPosition;
                particle.setVelocity(dx, 1.0);
            }
            if (pos.x > window_width - particle.radius || pos.x < particle.radius)
            {
                if (pos.x < particle.radius)
                    newPosition.x = particle.radius;
                if (pos.x + particle.radius > window_width)
                    newPosition.x = window_width - particle.radius;
                particle.position = newPosition;
                particle.setVelocity(dy, 1.0);
            }
        }
    }

    std::vector<int> getGridParticles(sf::Vector2i indexGrid)
    {
        std::vector<sf::Vector2i> coords = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
        for (int i = 0; i < 9; i++)
        {
            coords[i] += indexGrid;
        }
        std::vector<int> particlesGridIndexs;
        for (int i = 0; i < coords.size(); i++)
        {
            if (coords[i].x != -1 && coords[i].y != -1 && coords[i].x != 50 && coords[i].y != 50)
            {
                std::vector<int> values = grid.grid[coords[i].x][coords[i].y];
                for (int j = 0; j < values.size(); j++)
                {
                    particlesGridIndexs.push_back(values[j]);
                }
            }
        }
        return particlesGridIndexs;
    }

    void checkCollisions()
    {
        int particlesLen = particles.size();
        for (int p1Index = 0; p1Index < particlesLen; p1Index++)
        {
            Particle &particle1 = particles[p1Index];

            std::vector<int> particleGridIndexs = getGridParticles(particle1.posIndex);

            for (int p2Index = 0; p2Index < particleGridIndexs.size(); p2Index++)
            {
                if (particle1.particleNum != particleGridIndexs[p2Index])
                {
                    Particle &particle2 = particles[particleGridIndexs[p2Index] - 1];
                    float summedRadius = particle1.radius + particle2.radius;
                    sf::Vector2f diff = particle1.position - particle2.position;
                    float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
                    if (summedRadius > distance)
                    {
                        sf::Vector2f normal = diff / distance;
                        float delta = 0.5f * (summedRadius - distance);

                        particle1.position += 0.5f * normal * delta;
                        particle2.position -= 0.5f * normal * delta;
                    }
                }
            }
        }
    }
};
