#include <iostream>
#include "GraphicalMinesweeperGame.h"

using namespace std;

int main() {
    cout << "=== ГРАФИЧЕСКИЙ САПЁР ===\n";
    cout << "Введите параметры игры:\n";
    
    int width, height, mines;
    
    cout << "Ширина поля (5-40): ";
    cin >> width;
    if (width < 5) width = 5;
    if (width > 40) width = 40;
    
    cout << "Высота поля (5-30): ";
    cin >> height;
    if (height < 5) height = 5;
    if (height > 30) height = 30;
    
    int maxMines = width * height - 9;
    cout << "Количество мин (1-" << maxMines << "): ";
    cin >> mines;
    if (mines < 1) mines = 1;
    if (mines > maxMines) mines = maxMines;
    
    cout << "\nЗапуск игры с параметрами: " 
         << width << "x" << height << ", мин: " << mines << endl;
    cout << "Управление:\n";
    cout << "  ЛКМ - открыть клетку\n";
    cout << "  ПКМ - поставить/снять флаг\n";
    cout << "  R - начать новую игру\n";
    cout << "  ESC - выход\n\n";
    
    try {
        GraphicalMinesweeperGame game(width, height, mines);
        game.run();
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
