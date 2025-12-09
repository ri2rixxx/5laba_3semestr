#include "GraphicalMinesweeperGame.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

// Определения битмапов для простого шрифта 5x7
const uint8_t GraphicalMinesweeperGame::SimpleFont::digits[10][7] = {
    {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}, // 0
    {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}, // 1
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}, // 2
    {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E}, // 3
    {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}, // 4
    {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E}, // 5
    {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E}, // 6
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}, // 7
    {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}, // 8
    {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C}  // 9
};

const uint8_t GraphicalMinesweeperGame::SimpleFont::letters[26][7] = {
    {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}, // A
    {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E}, // B
    {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E}, // C
    {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E}, // D
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}, // E
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}, // F
    {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F}, // G
    {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}, // H
    {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E}, // I
    {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C}, // J
    {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}, // K
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}, // L
    {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}, // M
    {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11}, // N
    {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}, // O
    {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}, // P
    {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}, // Q
    {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}, // R
    {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}, // S
    {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}, // T
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}, // U
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04}, // V
    {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A}, // W
    {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11}, // X
    {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}, // Y
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F}  // Z
};

const uint8_t GraphicalMinesweeperGame::SimpleFont::colon[7] = {
    0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00
};

const uint8_t GraphicalMinesweeperGame::SimpleFont::slash[7] = {
    0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10
};

const uint8_t GraphicalMinesweeperGame::SimpleFont::dash[7] = {
    0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00
};

const uint8_t GraphicalMinesweeperGame::SimpleFont::space[7] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t* GraphicalMinesweeperGame::SimpleFont::getBitmap(char c) {
    if (c >= '0' && c <= '9') return digits[c - '0'];
    if (c >= 'A' && c <= 'Z') return letters[c - 'A'];
    if (c >= 'a' && c <= 'z') return letters[c - 'a'];
    if (c == ':') return colon;
    if (c == '/') return slash;
    if (c == '-') return dash;
    if (c == ' ') return space;
    return space; // Для неизвестных символов возвращаем пробел
}

// Метод для рисования простого текста
void GraphicalMinesweeperGame::drawSimpleText(sf::RenderWindow& window, const string& text, 
                                              float x, float y, float scale, sf::Color color) {
    sf::RectangleShape pixel(sf::Vector2f(scale, scale));
    pixel.setFillColor(color);
    
    float currentX = x;
    
    for (char c : text) {
        const uint8_t* bitmap = SimpleFont::getBitmap(c);
        
        for (int row = 0; row < SimpleFont::CHAR_HEIGHT; row++) {
            uint8_t rowData = bitmap[row];
            for (int col = 0; col < SimpleFont::CHAR_WIDTH; col++) {
                if (rowData & (1 << (4 - col))) { // Проверяем бит (от старшего к младшему)
                    pixel.setPosition(currentX + col * scale, y + row * scale);
                    window.draw(pixel);
                }
            }
        }
        
        currentX += (SimpleFont::CHAR_WIDTH + SimpleFont::CHAR_SPACING) * scale;
    }
}

GraphicalMinesweeperGame::GraphicalMinesweeperGame(int width, int height, int mines) 
    : firstMove(true), gameRunning(true), elapsedSeconds(0),
      hiddenCellColor(100, 100, 100),      // Темно-серый
      openCellColor(200, 200, 200),        // Светло-серый
      borderColor(50, 50, 50),             // Очень темно-серый
      flagColor(255, 0, 0),                // Красный
      mineColor(0, 0, 0),                  // Черный
      explodedMineColor(255, 0, 0),        // Красный
      infoPanelColor(70, 70, 70),          // Темно-серый для панели
      textColor(255, 255, 255)             // Белый
{
    // Простая проверка размеров
    if (width > 40) width = 40;
    if (height > 30) height = 30;
    
    int totalCells = width * height;
    if (mines >= totalCells) {
        mines = totalCells - 9;
    }
    
    cout << "Создание поля: " << width << "x" << height << ", мин: " << mines << endl;
    
    // Создание окна
    window.create(sf::VideoMode(width * CELL_SIZE, height * CELL_SIZE + INFO_HEIGHT), 
                  "Сапер", sf::Style::Close);
    
    window.setFramerateLimit(60);
    
    // Создание игрового поля
    field = new MineField(width, height, mines);
    
    gameClock.restart();
}

GraphicalMinesweeperGame::~GraphicalMinesweeperGame() {
    delete field;
}

void GraphicalMinesweeperGame::run() {
    while (window.isOpen() && gameRunning) {
        handleEvents();
        
        // Обновление таймера
        elapsedSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        
        // Отрисовка
        window.clear(sf::Color(50, 50, 50)); // Темно-серый фон
        
        drawField();
        drawUI();
        
        window.display();
    }
    
    // Пауза перед закрытием при завершении игры
    if (field->isGameOver()) {
        sf::Clock pauseClock;
        while (pauseClock.getElapsedTime().asSeconds() < 3.0f && window.isOpen()) {
            handleEvents();
            window.clear(sf::Color(50, 50, 50));
            drawField();
            drawUI();
            window.display();
        }
    }
}

void GraphicalMinesweeperGame::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
                
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.y < field->getHeight() * CELL_SIZE) {
                    int cellX = event.mouseButton.y / CELL_SIZE;
                    int cellY = event.mouseButton.x / CELL_SIZE;
                    bool rightClick = (event.mouseButton.button == sf::Mouse::Right);
                    handleMouseClick(cellX, cellY, rightClick);
                }
                break;
                
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::R) {
                    // Рестарт игры
                    delete field;
                    field = new MineField(field->getWidth(), field->getHeight(), field->getTotalMines());
                    firstMove = true;
                    gameClock.restart();
                    elapsedSeconds = 0;
                }
                break;
                
            default:
                break;
        }
    }
}

void GraphicalMinesweeperGame::handleMouseClick(int x, int y, bool rightClick) {
    if (field->isGameOver()) return;
    
    if (rightClick) {
        field->toggleFlag(x, y);
    } else {
        if (!field->openCell(x, y, firstMove)) {
            // Игра окончена - поражение
            gameRunning = false;
        }
        firstMove = false;
        
        if (field->isGameWon()) {
            gameRunning = false;
        }
    }
}

void GraphicalMinesweeperGame::drawField() {
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    
    for (int x = 0; x < field->getHeight(); x++) {
        for (int y = 0; y < field->getWidth(); y++) {
            Cell* cell = field->getCell(x, y);
            if (!cell) continue;
            
            float posX = y * CELL_SIZE + 1;
            float posY = x * CELL_SIZE + 1;
            
            // Рисуем фон клетки
            cellShape.setPosition(posX, posY);
            
            if (!cell->getIsOpen() && !cell->getIsFlagged()) {
                // Скрытая клетка
                cellShape.setFillColor(hiddenCellColor);
                cellShape.setOutlineColor(borderColor);
                cellShape.setOutlineThickness(1);
                window.draw(cellShape);
            } else if (cell->getIsFlagged()) {
                // Клетка с флагом
                cellShape.setFillColor(hiddenCellColor);
                cellShape.setOutlineColor(borderColor);
                cellShape.setOutlineThickness(1);
                window.draw(cellShape);
                
                // Рисуем красный треугольник как флаг
                sf::ConvexShape flag;
                flag.setPointCount(3);
                flag.setPoint(0, sf::Vector2f(posX + 8, posY + 8));
                flag.setPoint(1, sf::Vector2f(posX + 24, posY + 16));
                flag.setPoint(2, sf::Vector2f(posX + 8, posY + 24));
                flag.setFillColor(flagColor);
                window.draw(flag);
                
            } else if (cell->getIsOpen()) {
                // Открытая клетка
                cellShape.setFillColor(openCellColor);
                cellShape.setOutlineColor(borderColor);
                cellShape.setOutlineThickness(1);
                window.draw(cellShape);
                
                if (cell->hasMine()) {
                    // Мина
                    sf::CircleShape mine(CELL_SIZE / 4);
                    mine.setPosition(posX + CELL_SIZE/4, posY + CELL_SIZE/4);
                    
                    // Если игра проиграна и это мина, которая взорвалась
                    if (field->isGameOver() && !field->isGameWon()) {
                        mine.setFillColor(explodedMineColor);
                    } else {
                        mine.setFillColor(mineColor);
                    }
                    window.draw(mine);
                } else {
                    // Число соседних мин
                    int adjacent = cell->getAdjacentMines();
                    if (adjacent > 0) {
                        // Используем простой шрифт для чисел
                        string numStr = to_string(adjacent);
                        sf::Color numColor = cell->getNumberColor();
                        drawSimpleText(window, numStr, 
                                     posX + CELL_SIZE/2 - 5, 
                                     posY + CELL_SIZE/2 - 8, 
                                     2.0f, numColor);
                    }
                }
            }
        }
    }
}

void GraphicalMinesweeperGame::drawUI() {
    // Рисуем панель информации
    sf::RectangleShape infoPanel(sf::Vector2f(window.getSize().x, INFO_HEIGHT));
    infoPanel.setPosition(0, field->getHeight() * CELL_SIZE);
    infoPanel.setFillColor(infoPanelColor);
    window.draw(infoPanel);
    
    // Рисуем разделительную линию
    sf::RectangleShape divider(sf::Vector2f(window.getSize().x, 2));
    divider.setPosition(0, field->getHeight() * CELL_SIZE);
    divider.setFillColor(textColor);
    window.draw(divider);
    
    // Отображаем статус игры (английский текст)
    string status;
    if (field->isGameOver()) {
        if (field->isGameWon()) {
            status = "YOU WIN!";
        } else {
            status = "GAME OVER!";
        }
    } else {
        status = "PLAYING...";
    }
    drawSimpleText(window, status, 10, field->getHeight() * CELL_SIZE + 15, 2.0f, textColor);
    
    // Отображаем счетчик мин (английский текст)
    string minesText = "MINES: " + to_string(field->getMinesFound()) + "/" + to_string(field->getTotalMines());
    drawSimpleText(window, minesText, 200, field->getHeight() * CELL_SIZE + 15, 1.5f, sf::Color::Green);
    
    // Отображаем таймер (английский текст)
    string timerTextStr = "TIME: " + to_string(elapsedSeconds) + " SEC";
    drawSimpleText(window, timerTextStr, 400, field->getHeight() * CELL_SIZE + 15, 1.5f, sf::Color::Yellow);
    
    // Инструкция (английский текст)
    string instruction = "LMB - OPEN | RMB - FLAG | R - RESTART | ESC - EXIT";
    drawSimpleText(window, instruction, 10, field->getHeight() * CELL_SIZE + 50, 1.0f, textColor);
}
