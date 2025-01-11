struct Particle
{
    sf::Vector2f position;
    sf::Vector2f last_position;
    sf::Vector2f acceleration;
    float radius = 10.0f;

    Particle() = default;
    Particle(sf::Vector2f position_, float radius_)
        : last_position{position_},
          position{position_},
          acceleration{10.0f, 10.0f},
          radius{radius_} {}

    void update(float dt)
    {
        // Verlet Intergration
        sf::Vector2f displacement = position - last_position;
        last_position = position;
        position = position + displacement + acceleration * dt * dt;
        acceleration = {};
    }

    sf::Vector2f getVelocity(float dt)
    {
        sf::Vector2f velocity = (position - last_position) / dt;
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
    void update()
    {
        applyGravity();
        calculateBoundary();
        updateParticles();
    }

    std::vector<Particle> getParticles()
    {
        return particles;
    }

    void addParticle(sf::Vector2f position, float size)
    {
        particles.emplace_back(Particle(position, size));
    }

private:
    std::vector<Particle> particles;
    sf::Vector2f gravity = {0.0f, 1000.0f};
    float dt = 1.0f / 60;

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
            if (particle.position.y >= 840.0f - particle.radius)
            {
                sf::Vector2f velocity = particle.getVelocity(dt);
                particle.setVelocity(-velocity, dt);
                particle.accelerate(-particle.acceleration);
            }
        }
    }
};
