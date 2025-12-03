#include "map.hpp"
#include "player.hpp"
#include <SFML/Graphics.hpp>
#include <random>


Map::Map()
{
    // Create boundary walls
    sf::RectangleShape topWall(sf::Vector2f(static_cast<float>(mapSize.x), wallThickness));
    topWall.setPosition({0.f, 0.f});
    topWall.setFillColor(wallColor);
    sf::RectangleShape botWall(sf::Vector2f(static_cast<float>(mapSize.x), wallThickness));
    botWall.setPosition({0.f, static_cast<float>(mapSize.y) - wallThickness});
    botWall.setFillColor(wallColor);
    sf::RectangleShape leftWall(sf::Vector2f(wallThickness, static_cast<float>(mapSize.y)));
    leftWall.setPosition({0.f, 0.f});
    leftWall.setFillColor(wallColor);
    sf::RectangleShape rightWall(sf::Vector2f(wallThickness, static_cast<float>(mapSize.y)));
    rightWall.setPosition({static_cast<float>(mapSize.x) - wallThickness, 0.f});
    rightWall.setFillColor(wallColor);

    wallBlocks.push_back(topWall);
    wallBlocks.push_back(botWall);

    wallBlocks.push_back(leftWall);
    wallBlocks.push_back(rightWall);

    // Create some breakable blocks as an example
    unsigned int stratx = 100;
    unsigned int straty = 50;
    for (unsigned int x = stratx; x < mapSize.x - stratx; x+=(breakableThickness+5))
    {
        for (unsigned int y = straty; y < mapSize.y / 2; y+=(breakableThickness+5))
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 1);
            if (dis(gen) == 0)
                continue; // 随机跳过一些块，增加多样性
            sf::RectangleShape breakableBlock(sf::Vector2f(breakableThickness, breakableThickness));
            breakableBlock.setPosition({static_cast<float>(x), static_cast<float>(y)});
            breakableBlock.setFillColor(breakableColor);
            breakableBlocks.push_back(breakableBlock);
        }
    }
}

Map::Map(sf::Vector2u mapSize)
    : mapSize(mapSize)
{
    Map();
}


Map::Map(const std::vector<sf::RectangleShape> &wallBlocks, const std::vector<sf::RectangleShape> &breakableBlocks, const sf::Vector2u &mapSize)
    : wallBlocks(wallBlocks), breakableBlocks(breakableBlocks), mapSize(mapSize)
{
    this->wallBlocks = wallBlocks;
    this->breakableBlocks = breakableBlocks;
}

void Map::updateGameState(Ball& ball, float dt)
{
    for (const auto& wall : wallBlocks)
    {
        auto intersection = wall.getGlobalBounds().findIntersection(ball.getGlobalBounds());
        if (intersection.has_value())
        {
            // 简单处理：反转垂直和水平方向速度
            auto dir = ball.getDirection();
            if (wall.getSize().x > wall.getSize().y) // 水平墙壁
            {
                dir.y = -dir.y;
            }
            else // 垂直墙壁
            {
                dir.x = -dir.x;
            }
            ball.setDirection(dir);
            return;
        }
    }

    // Check collision with breakable blocks
    for (auto it = breakableBlocks.begin(); it != breakableBlocks.end(); )
    {
        auto intersection = it->getGlobalBounds().findIntersection(ball.getGlobalBounds());
        if (intersection.has_value())
        {
            // 简单处理：反转垂直和水平方向速度
            auto dir = ball.getDirection();
            float width = intersection->size.x;
            float height = intersection->size.y;
            if (width < height)
            {
                // 水平碰撞，反转水平方向速度
                dir.x = -dir.x;
            }
            else
            {
                // 垂直碰撞，反转垂直方向速度
                dir.y = -dir.y;
            }
            ball.setDirection(dir);
            it = breakableBlocks.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto& wall : wallBlocks)
    {
        target.draw(wall, states);
    }
    for (const auto& block : breakableBlocks)
    {
        target.draw(block, states);
    }
}
