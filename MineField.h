#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <vector>
#include "Cell.h"

// Класс игрового поля
class MineField {
private:
    int width;
    int height;
    int totalMines;
    int minesFound;
    bool gameOver;
    bool gameWon;
    std::vector<std::vector<Cell*>> field;
    
    // Проверка корректности координат
    bool isValidCoord(int x, int y) const;
    
    // Подсчет мин вокруг клетки
    int countAdjacentMines(int x, int y) const;
    
    // Рекурсивное открытие пустых областей
    void openEmptyArea(int x, int y);
    
public:
    MineField(int w, int h, int mines);
    ~MineField();
    
    // Генерация мин после первого хода
    void generateMines(int firstX, int firstY);
    
    // Открыть клетку
    bool openCell(int x, int y, bool firstMove = false);
    
    // Пометить/снять флаг
    void toggleFlag(int x, int y);
    
    // Проверка победы
    void checkWin();
    
    // Отображение поля
    void display(bool revealAll = false) const;
    
    bool isGameOver() const;
    bool isGameWon() const;
    int getMinesFound() const;
    int getTotalMines() const;
    
    // Новые методы для графического интерфейса
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Cell* getCell(int x, int y) const { 
        if (isValidCoord(x, y)) return field[x][y]; 
        return nullptr;
    }
};

#endif
