#ifndef MINECELL_H
#define MINECELL_H

#include "Cell.h"

// Класс для клетки с миной
class MineCell : public Cell {
private:
    int adjacentMines;
    
public:
    MineCell();
    
    bool hasMine() const override;
    int getAdjacentMines() const override;
    void setAdjacentMines(int count) override;
    char getDisplayChar() const override;
    sf::Color getNumberColor() const override;  // Добавлен
};

#endif
