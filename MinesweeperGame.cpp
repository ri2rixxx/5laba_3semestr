#include "MinesweeperGame.h"
#include <iostream>

using namespace std;

MinesweeperGame::MinesweeperGame(int width, int height, int mines) 
    : firstMove(true) {
    field = new MineField(width, height, mines);
}

MinesweeperGame::~MinesweeperGame() {
    delete field;
}

void MinesweeperGame::play() {
    cout << "=== ИГРА САПЁР ===\n";
    cout << "Команды:\n";
    cout << "  o x y - открыть клетку\n";
    cout << "  f x y - поставить/снять флаг\n";
    cout << "  q - выход\n\n";
    
    while (!field->isGameOver()) {
        field->display();
        
        char command;
        int x, y;
        
        cout << "\nВведите команду: ";
        cin >> command;
        
        if (command == 'q') {
            cout << "Игра прервана.\n";
            break;
        }
        
        cin >> x >> y;
        
        if (command == 'o') {
            if (!field->openCell(x, y, firstMove)) {
                field->display(true);
                cout << "\n*** ВЗРЫВ! ВЫ ПРОИГРАЛИ! ***\n";
                cout << "Найдено мин: " << field->getMinesFound() 
                     << " из " << field->getTotalMines() << "\n";
                break;
            }
            firstMove = false;
        } else if (command == 'f') {
            field->toggleFlag(x, y);
        }
        
        if (field->isGameWon()) {
            field->display();
            cout << "\n*** ПОЗДРАВЛЯЕМ! ВЫ ВЫИГРАЛИ! ***\n";
            cout << "Все мины найдены: " << field->getTotalMines() << "\n";
            break;
        }
    }
}
