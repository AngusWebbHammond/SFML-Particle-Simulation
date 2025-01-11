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
          acceleration{10000.0f, 10000.0f},
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

class Solver
{
public:
    Solver(float width, float height) : window_width{width}, window_height{height} {}

    void update()
    {
        for (int i; i < subStep; i++)
        {
            calculateBoundary();
            applyGravity();
            updateParticles();
        }
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
    float dt = 1.0f / 60;
    int subStep = 8;
    sf::Vector2f gravity = {0.0f, 1000.0f / subStep};
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
};
