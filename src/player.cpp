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
    // 1. 获取两个物体的包围盒
    auto ballBounds = ball.getGlobalBounds();
    auto blockBounds = getGlobalBounds();

    // 2. 检测碰撞
    auto intersection = blockBounds.findIntersection(ballBounds);

    if (intersection.has_value())
    {
        float overlapWidth = intersection->size.x;
        float overlapHeight = intersection->size.y;
        auto dir = ball.getDirection(); // 获取当前方向

        // --- 核心修复逻辑 ---

        if (overlapWidth < overlapHeight)
        {
            // --- 情况 A: 水平碰撞 (撞了侧面) ---
            
            // 1. 反转 X 速度
            dir.x = -dir.x;
            ball.setDirection(dir);

            // 2. 【关键】修正位置：把球推出来
            // 判断球是在板子的左边还是右边？
            if (ballBounds.position.x < blockBounds.position.x)
            {
                // 球在左边 -> 往左推 overlapWidth
                ball.move({-overlapWidth, 0.f});
            }
            else
            {
                // 球在右边 -> 往右推 overlapWidth
                ball.move({overlapWidth, 0.f});
            }
        }
        else
        {
            // --- 情况 B: 垂直碰撞 (撞了顶/底) ---

            // 1. 反转 Y 速度
            dir.y = -dir.y;
            ball.setDirection(dir);

            // 2. 【关键】修正位置
            // 判断球是在板子的上面还是下面？
            if (ballBounds.position.y < blockBounds.position.y)
            {
                // 球在上面 -> 往上推 overlapHeight
                ball.move({0.f, -overlapHeight});
            }
            else
            {
                // 球在下面 -> 往下推 overlapHeight
                ball.move({0.f, overlapHeight});
            }
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

bool Ball::updateGameState(float dt, float validY)
{
    updatePosition(dt);
    if (getPosition().y > validY)
    {
        return false; // 掉出界外，游戏结束
    }
    return true; // 继续游戏
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

bool Player::updateGameState(float dt)
{
    int scoreIncrement = map->updateGameState(*ball, dt);
    score += scoreIncrement;
    block->updateGameState(*ball, dt);
    return ball->updateGameState(dt,block->getPosition().y);
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

