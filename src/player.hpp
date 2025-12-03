#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Map;

class Ball : public sf::CircleShape
{
public:
    sf::Vector2f direction = {-1.0f, -1.f};
    float speed = 100.f;
    float radius = 10.f;
    Ball();
    Ball(float radius);
    Ball(float radius, const sf::Color& color);
    Ball(float radius, const sf::Color& color, const sf::Vector2f& position);

    Ball(const Ball& other) = default;

    void setDirection(const sf::Vector2f& dir);
    void setSpeed(float spd);
    sf::Vector2f getDirection() const { return direction; }
    
    void updatePosition(float dt);
    void updateGameState(float dt);
private:

};

class Block : public sf::RectangleShape
{
public:
    int length = 50;
    float speed = 200.f;
    Ball ball = Ball();

    Block();
    Block(const sf::Vector2f& size);
    Block(const sf::Vector2f& size, const sf::Color& color);
    Block(const sf::Vector2f& size, const sf::Color& color, const sf::Vector2f& position);
    
    Block(const Block& other) = default;
    void updateGameState(Ball& ball, float dt);

private:

};

class Player : public sf::Drawable
{
public:
    std::unique_ptr<Block> block;
    std::unique_ptr<Ball> ball;
    std::shared_ptr<Map> map;

    Player(Map& map);
    Player(Block& block,Ball& ball, Map& map);

    void updateGameState(float dt);
    void handleInput(float dt);
    ~Player() override = default;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};