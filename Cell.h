#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics/Color.hpp>

// Базовый класс для клетки поля
class Cell {
protected:
    bool isOpen;      // Открыта ли клетка
    bool isFlagged;   // Помечена ли флагом
    
public:
    Cell();
    virtual ~Cell();
    
    // Чисто виртуальные методы
    virtual bool hasMine() const = 0;
    virtual int getAdjacentMines() const = 0;
    virtual void setAdjacentMines(int count) = 0;
    virtual char getDisplayChar() const = 0;
    virtual sf::Color getNumberColor() const = 0;  // Новый метод
    
    bool getIsOpen() const;
    bool getIsFlagged() const;
    
    void setOpen(bool open);
    void toggleFlag();
};

#endif
