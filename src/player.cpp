#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"

// Ball Class Implementation
Block::Block()
    : Block({50.f, 10.f}, sf::Color::Blue, {375.f, 550.f})
{
}

Block::Block(const sf::Vector2f &size)
    : sf::RectangleShape(size), length(size.x)
{
    setFillColor(sf::Color::White); // 默认白色
}

Block::Block(const sf::Vector2f& size, const sf::Color& color)
    : sf::RectangleShape(size), length(size.x)
{
    setFillColor(color);
}

Block::Block(const sf::Vector2f& size, const sf::Color& color, const sf::Vector2f& position)
    : sf::RectangleShape(size), length(size.x)
{
    setFillColor(color);
    setPosition(position);
}

void Block::updateGameState(Ball &ball, float dt)
{
    auto intersection = getGlobalBounds().findIntersection(ball.getGlobalBounds());
    if (intersection.has_value())
    {
        float width = intersection->size.x;
        float height = intersection->size.y;
        if (width < height)
        {
            // 水平碰撞，反转水平方向速度
            auto dir = ball.getDirection();
            dir.x = -dir.x;
            ball.setDirection(dir);
        }
        else
        {
            // 垂直碰撞，反转垂直方向速度
            auto dir = ball.getDirection();
            dir.y = -dir.y;
            ball.setDirection(dir);
        }
    }
}

// Ball Class Implementation
Ball::Ball()
    : Ball(Map::breakableThickness)
{
    
}

Ball::Ball(float radius) : sf::CircleShape(radius)
{
    this->radius = radius;
    setFillColor(sf::Color::White); // 默认白色
}

Ball::Ball(float radius, const sf::Color& color) : sf::CircleShape(radius)
{
    this->radius = radius;
    setFillColor(color);
}

Ball::Ball(float radius, const sf::Color& color, const sf::Vector2f& position) : sf::CircleShape(radius)
{
    this->radius = radius;
    setFillColor(color);
    setPosition(position);
}

void Ball::setDirection(const sf::Vector2f &dir)
{
    direction = dir;
}

void Ball::setSpeed(float spd)
{
    speed = spd;
}

void Ball::updatePosition(float dt)
{
    move(direction * speed * dt);
}

void Ball::updateGameState(float dt)
{
    updatePosition(dt);
}

// Player Class Implementation
Player::Player(Map &map)
    : map(std::make_shared<Map>(map))
{
    block = std::make_unique<Block>(Block({50.f, 10.f}, sf::Color::White, {375.f, 550.f}));
    ball = std::make_unique<Ball>(Ball(5.f, sf::Color::White, {400.f, 540.f}));
}

Player::Player(Block& block,Ball& ball, Map& map)
{
    this->block = std::make_unique<Block>(block);
    this->ball =  std::make_unique<Ball>(ball);
    this->map = std::make_shared<Map>(map);
}

void Player::updateGameState(float dt)
{
    map->updateGameState(*ball, dt);
    block->updateGameState(*ball, dt);
    ball->updateGameState(dt);
}

void Player::handleInput(float dt)
{
    // left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        block->move({-block->speed * dt, 0.f});
        // 限制挡板不出界
        if (block->getPosition().x < 0.f)
            block->setPosition({0.f, block->getPosition().y});
    }
    // right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        block->move({block->speed * dt, 0.f});
        // 限制挡板不出界
        if (block->getPosition().x + block->getSize().x > map->mapSize.x)
            block->setPosition({map->mapSize.x - block->getSize().x, block->getPosition().y});
    }
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*block, states);
    target.draw(*ball, states);
    target.draw(*map, states);
}

