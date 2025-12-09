#ifndef GRAPHICALMINESWEEPERGAME_H
#define GRAPHICALMINESWEEPERGAME_H

#include <SFML/Graphics.hpp>
#include "MineField.h"
#include <vector>
#include <string>

// Класс графической игры
class GraphicalMinesweeperGame {
private:
    MineField* field;
    bool firstMove;
    bool gameRunning;
    
    // Графические компоненты
    sf::RenderWindow window;
    
    // Размеры
    static const int CELL_SIZE = 32;
    static const int INFO_HEIGHT = 100;
    static const int INFO_MARGIN = 10;
    
    // Таймер
    sf::Clock gameClock;
    int elapsedSeconds;
    
    // Цвета
    sf::Color hiddenCellColor;
    sf::Color openCellColor;
    sf::Color borderColor;
    sf::Color flagColor;
    sf::Color mineColor;
    sf::Color explodedMineColor;
    sf::Color infoPanelColor;
    sf::Color textColor;
    
    // Простой точечный шрифт 5x7 пикселей
    struct SimpleFont {
        static const int CHAR_WIDTH = 5;
        static const int CHAR_HEIGHT = 7;
        static const int CHAR_SPACING = 1;
        
        // Битмапы для символов (0-9, A-Z, некоторые символы)
        static const uint8_t digits[10][7];
        static const uint8_t letters[26][7];
        static const uint8_t colon[7];
        static const uint8_t slash[7];
        static const uint8_t dash[7];
        static const uint8_t space[7];
        
        // Получить битмап для символа
        static const uint8_t* getBitmap(char c);
    };
    
    // Методы для рисования простого текста
    void drawSimpleText(sf::RenderWindow& window, const std::string& text, 
                        float x, float y, float scale, sf::Color color);
    
    // Инициализация
    void initUI();
    
    // Обработка событий
    void handleEvents();
    void handleMouseClick(int x, int y, bool rightClick);
    
    // Отрисовка
    void drawField();
    void drawUI();
    
public:
    GraphicalMinesweeperGame(int width, int height, int mines);
    ~GraphicalMinesweeperGame();
    
    void run();
};

#endif
