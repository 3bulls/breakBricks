#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "map.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int renderGame(sf::RenderWindow& window);
int renderScoreAndMenu(sf::RenderWindow& window, int score);

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Break Brick");
    window.setFramerateLimit(144);

    bool InGame = true;
    while (InGame && window.isOpen())
    {
        // Handle window events
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        int score = renderGame(window);
        InGame = renderScoreAndMenu(window, score);
    }

    return 0;
}

int renderScoreAndMenu(sf::RenderWindow& window, int score)
{

    window.clear(sf::Color::Black);
    std::filesystem::path fontPath = "../../font/arial.ttf";
    sf::Font font = sf::Font(fontPath);
    std::string GameOverStr = "Game Over!\nYour Score: " + std::to_string(score);
    sf::Text scoreText = sf::Text(font, GameOverStr, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({WINDOW_WIDTH / 2.f - scoreText.getLocalBounds().size.x / 2.f, WINDOW_HEIGHT / 3.f - scoreText.getLocalBounds().size.y / 2.f});
    
    std::string InstructionStr = "Press Enter to Play Again\nPress Esc to Exit";
    sf::Text instructionText = sf::Text(font, InstructionStr, 20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({WINDOW_WIDTH / 2.f - instructionText.getLocalBounds().size.x / 2.f, WINDOW_HEIGHT / 2.f - instructionText.getLocalBounds().size.y / 2.f + 50.f});
    window.draw(scoreText);
    window.draw(instructionText);
    window.display();
    // 等待用户输入
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return 0;
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter)
                {
                    return 1; // 重新开始游戏
                }
                else if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                    return 0; // 退出游戏
                }
            }
        }
        sf::sleep(sf::milliseconds(100)); // 减少CPU占用
    }
    return 0;
}

int renderGame(sf::RenderWindow& window)
{
    // 创建一个时钟，用来计算两帧之间的时间差
    sf::Clock clock;
    auto map = Map();
    auto player = Player(map);

    bool isPlaying = true;
    // --- 游戏主循环 (Game Loop) ---
    while (isPlaying && window.isOpen())
    {
        // [关键] 计算 Delta Time (dt)
        // restart() 会返回距离上一次调用经过的时间，并重置时钟
        // asSeconds() 把它转换成秒 (比如 0.016 秒)
        float dt = clock.restart().asSeconds();
        // 按键处理
        player.handleInput(dt);

        // --- 更新游戏状态 (Update Game State) ---
        isPlaying = player.updateGameState(dt);

        // --- 渲染画面 (Render) ---
        window.clear(sf::Color::Black); // 1. 清空上一帧 (刷黑)
        window.draw(player);            // 2. 画上玩家
        window.display();               // 3. 显示出来
        sf::sleep(sf::milliseconds(1000/60)); // 防止 CPU 占用过高
    }

    return player.score;
}
