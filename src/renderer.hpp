class Renderer
{
public:
    Renderer(sf::RenderTarget &_target) : target{_target} {}

    void update()
    {
        sf::CircleShape circle(10.0f);
        circle.setPointCount(32);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition({420.0f, 420.0f});
        target.draw(circle);
    }

private:
    sf::RenderTarget &target;
};
