#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// ✅ 加这一行：前置声明
class Ball;
class Block;
class Player;

class Map : public sf::Drawable
{
public:

    static constexpr float wallThickness = 10.f;
    static constexpr float breakableThickness = 10.f;

    sf::Vector2u mapSize = {800, 600};
    std::vector<sf::RectangleShape> wallBlocks;
    std::vector<sf::RectangleShape> breakableBlocks;
    sf::Color wallColor = sf::Color::White;
    sf::Color breakableColor = sf::Color::Magenta;

    Map();
    Map(sf::Vector2u mapSize);
    Map(const std::vector<sf::RectangleShape>& wallBlocks, const std::vector<sf::RectangleShape>& breakableBlocks, const sf::Vector2u& mapSize = {800, 600});
    int updateGameState(Ball& ball, float dt);
    ~Map() override = default;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};