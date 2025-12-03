#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Break Brick");
    window.setFramerateLimit(144);

    // 创建一个时钟，用来计算两帧之间的时间差
    sf::Clock clock;
    auto map = Map();
    auto player = Player(map);

    // --- 游戏主循环 (Game Loop) ---
    while (window.isOpen())
    {
        // [关键] 计算 Delta Time (dt)
        // restart() 会返回距离上一次调用经过的时间，并重置时钟
        // asSeconds() 把它转换成秒 (比如 0.016 秒)
        float dt = clock.restart().asSeconds();

        // --- A. 处理系统事件 (Input - System) ---
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 按键处理
        player.handleInput(dt);

        // --- B. 更新游戏状态 (Update Game State) ---
        player.updateGameState(dt);

        // --- D. 渲染画面 (Render) ---
        window.clear(sf::Color::Black); // 1. 清空上一帧 (刷黑)
        window.draw(player);            // 2. 画上玩家
        window.display();               // 3. 显示出来
    }

    return 0;
}