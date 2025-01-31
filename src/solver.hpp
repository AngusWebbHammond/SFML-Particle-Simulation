#include <math.h>

struct Node;

struct Node
{
    int index;
    Node *ptr;

    Node(int index_) : index{index_}, ptr{nullptr} {};
};

struct Grid
{
    std::vector<Node *> grid;

    void createGrid(int rows, int columns)
    {
        for (int i = 0; i < rows * columns; i++)
        {
            grid.emplace_back(nullptr);
        }
    }

    void addNode(int indexGrid, Node *node)
    {
        if (grid[indexGrid] == nullptr)
        {
            grid[indexGrid] = node;
        }
        else
        {
            for (Node *nPtr = grid[indexGrid]; nPtr != nullptr; nPtr = nPtr->ptr)
            {
                if (nPtr->ptr == nullptr)
                {
                    nPtr->ptr = node;
                    break;
                }
            }
        }
    }

    Node *returnNodeAtIndex(int index)
    {
        return grid[index];
    }

    void removeNode(int indexGrid, Node *node)
    {
        if (grid[indexGrid] == node)
        {
            grid[indexGrid] = node->ptr;
            node->ptr = nullptr;
        }
        else
        {
            for (Node *nPtr = grid[indexGrid]; nPtr != nullptr; nPtr = nPtr->ptr)
            {
                if (nPtr->ptr == node)
                {
                    nPtr->ptr = node->ptr;
                    node->ptr = nullptr;
                    break;
                }
            }
        }
    }

    void moveNode(int prevIndexGrid, int newIndexGrid, Node *node)
    {
        removeNode(prevIndexGrid, node);
        addNode(newIndexGrid, node);
    }

    void printGrid()
    {
        for (auto *node : grid)
        {
            if (node == nullptr)
            {
                std::cout << node << ", ";
            }
            else
            {
                if (node->ptr == nullptr)
                {
                    std::cout << node << " : Index: " << node->index << ", ";
                }
                else
                {
                    std::cout << "[";
                    Node *nodePtr = node;
                    while (nodePtr != nullptr)
                    {
                        std::cout << nodePtr << " : Index: " << nodePtr->index << ", ";
                        nodePtr = nodePtr->ptr;
                    }

                    std::cout << "], ";
                }
            }
        }
        std::cout << "\n";
    }
};

struct Particle
{
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    int particleNum;

    Particle() = default;
    Particle(sf::Vector2f position_, float radius_, int particleNum_)
        : last_position{position_},
          position{position_},
          acceleration{10000.0f, 0.0f},
          radius{radius_},
          particleNum{particleNum_} {}

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

    void addParticle(sf::Vector2f position, float size, sf::Vector2f acceleration, int particleNum)
    {
        particles.emplace_back(Particle(position, size, particleNum));
        particles.back().acceleration = acceleration;
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
};
