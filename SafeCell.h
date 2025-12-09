#ifndef SAFECELL_H
#define SAFECELL_H

#include "Cell.h"

// Класс для обычной клетки (без мины)
class SafeCell : public Cell {
private:
    int adjacentMines;
    
public:
    SafeCell();
    
    bool hasMine() const override;
    int getAdjacentMines() const override;
    void setAdjacentMines(int count) override;
    char getDisplayChar() const override;
    sf::Color getNumberColor() const override;  // Добавлен
};

#endif
