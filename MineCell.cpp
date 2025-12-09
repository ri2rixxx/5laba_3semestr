#include "MineCell.h"
#include <SFML/Graphics/Color.hpp>

MineCell::MineCell() : Cell(), adjacentMines(0) {}

bool MineCell::hasMine() const { 
    return true; 
}

int MineCell::getAdjacentMines() const { 
    return adjacentMines; 
}

void MineCell::setAdjacentMines(int count) { 
    adjacentMines = count; 
}

char MineCell::getDisplayChar() const {
    if (isFlagged) return 'F';
    if (!isOpen) return '*';
    return 'X';  // Взорванная мина
}

sf::Color MineCell::getNumberColor() const {
    return sf::Color::Red;
}
