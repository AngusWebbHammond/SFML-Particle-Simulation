#include <math.h>
struct Particle
{
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    int index;
    int gridIndex;
    sf::Vector2i coords;

    Particle() = default;
    Particle(sf::Vector2f position_, float radius_)
        : last_position{position_},
          position{position_},
          acceleration{10000.0f, 0.0f},
          radius{radius_} {}

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
struct Grid
{
    sf::Vector2i axisLength;
    float boxSize;
    std::vector<int> grid[100][100];

    Grid() = default;
    Grid(sf::Vector2i axisLength_, float boxSize_)
        : axisLength{axisLength_},
          boxSize{boxSize_} {}

    void createGrid(std::vector<Particle> particles)
    {
        int x;
        int y;

        int numParticles = particles.size();

        for (int i = 0; i < numParticles; i++)
        {
            sf::Vector2f pos = particles[i].position;
            x = floor(pos.x / boxSize);
            y = floor(pos.y / boxSize);
            grid[x][y].emplace_back(i);
            particles[i].index = grid[x][y].size();
        }
    }

    void moveParticle(Particle p)
    {
        sf::Vector2f lastPos = p.last_position;
        sf::Vector2f pos = p.position;

        int lastX = floor(lastPos.x / boxSize);
        int lastY = floor(lastPos.y / boxSize);
        int x = floor(pos.x / boxSize);
        int y = floor(pos.y / boxSize);

        grid[lastX][lastY].erase(grid[lastX][lastY].begin() + p.gridIndex);

        grid[x][y].emplace_back(p.index);

        p.gridIndex = grid[x][y].size();
    }

    void addParticle(Particle p)
    {
        sf::Vector2f pos = p.position;
        int x = floor(pos.x / boxSize);
        int y = floor(pos.y / boxSize);
        grid[x][y].emplace_back(p.index);
        p.gridIndex = grid[x][y].size();
    }

    std::vector<int> returnCell(int x, int y)
    {
        return grid[x][y];
    }
};

class Solver
{
public:
    Solver(float width, float height) : window_width{width}, window_height{height} {}

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

    void addParticle(sf::Vector2f position, float size, sf::Vector2f acceleration)
    {
        particles.emplace_back(Particle(position, size));
        particles.back().acceleration = acceleration;
        particles.back().index = particles.size();
        particleGrid.addParticle(particles.back());
    }

    void setVelocity(sf::Vector2f velocity)
    {
        Particle &finalParticle = particles.back();
        sf::Vector2f vel = {velocity.x / subStep, velocity.y / subStep};
        finalParticle.setVelocity(vel, dt);
    }

private:
    std::vector<Particle> particles;
    float dt = 1.0f / 60;
    int subStep = 8;
    sf::Vector2f gravity = {0.0f, 200.0f / subStep};
    float window_height;
    float window_width;

    Grid particleGrid = Grid({window_width, window_height}, 10.f);

    void applyGravity()
    {
        for (auto &particle : particles)
        {
            particle.accelerate(gravity);
        }
    }

    void updateParticles()
    {
        for (auto &particle : particles)
        {
            particle.update(dt);
            particleGrid.moveParticle(particle);
        }
    }

    void calculateBoundary()
    {
        for (auto &particle : particles)
        {
            float damp = 0.8f;
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

    void checkCollisions()
    {
        int particlesLen = particles.size();
        for (int p1Index = 0; p1Index < particlesLen; p1Index++)
        {
            Particle &particle1 = particles[p1Index];
            for (int p2Index = 0; p2Index < particlesLen; p2Index++)
            {
                if (p1Index != p2Index)
                {
                    Particle &particle2 = particles[p2Index];
                    sf::Vector2f p1Pos = particle1.position;
                    sf::Vector2f p2pos = particle2.position;
                    float summedRadius = particle1.radius + particle2.radius;
                    sf::Vector2f diff = p1Pos - p2pos;
                    float distanceSqr = diff.x * diff.x + diff.y * diff.y;
                    float distance = sqrt(distanceSqr);
                    if (summedRadius > distance)
                    {
                        sf::Vector2f normal = diff / distance;
                        float totalMass = particle1.radius * particle1.radius + particle2.radius * particle2.radius;
                        float massRatio = particle1.radius * particle1.radius / totalMass;
                        float delta = 0.7f * (summedRadius - distance);

                        particle1.position += (1 - massRatio) * normal * delta;
                        particle2.position -= massRatio * normal * delta;
                    }
                }
            }
        }
    }

    void checkCollisionsOptimized()
    {
    }
};
