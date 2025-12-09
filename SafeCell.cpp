#include "SafeCell.h"
#include <SFML/Graphics/Color.hpp>

SafeCell::SafeCell() : Cell(), adjacentMines(0) {}

bool SafeCell::hasMine() const { 
    return false; 
}

int SafeCell::getAdjacentMines() const { 
    return adjacentMines; 
}

void SafeCell::setAdjacentMines(int count) { 
    adjacentMines = count; 
}

char SafeCell::getDisplayChar() const {
    if (isFlagged) return 'F';
    if (!isOpen) return '*';
    if (adjacentMines == 0) return '.';
    return '0' + adjacentMines;
}

sf::Color SafeCell::getNumberColor() const {
    if (!isOpen || isFlagged) return sf::Color::White;
    
    switch(adjacentMines) {
        case 1: return sf::Color::Blue;
        case 2: return sf::Color(0, 150, 0); // Зеленый
        case 3: return sf::Color::Red;
        case 4: return sf::Color(128, 0, 128); // Фиолетовый
        case 5: return sf::Color(128, 0, 0);   // Темно-красный
        case 6: return sf::Color(64, 224, 208); // Бирюзовый
        case 7: return sf::Color::Black;
        case 8: return sf::Color(128, 128, 128); // Серый
        default: return sf::Color::White;
    }
}
