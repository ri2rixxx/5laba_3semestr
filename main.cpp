#include <iostream>
#include "GraphicalMinesweeperGame.h"

using namespace std;

int main() {
    int width, height, mines;
    
    cout << "=== ГРАФИЧЕСКИЙ САПЁР ===\n";
    cout << "Введите ширину поля (рекомендуется 9-30): ";
    cin >> width;
    cout << "Введите высоту поля (рекомендуется 9-16): ";
    cin >> height;
    cout << "Введите количество мин: ";
    cin >> mines;
    
    if (mines >= width * height) {
        cout << "Ошибка: слишком много мин!\n";
        return 1;
    }
    
    try {
        GraphicalMinesweeperGame game(width, height, mines);
        game.run();
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
