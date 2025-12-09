#include "MineField.h"
#include "MineCell.h"
#include "SafeCell.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

bool MineField::isValidCoord(int x, int y) const {
    return x >= 0 && x < height && y >= 0 && y < width;
}

int MineField::countAdjacentMines(int x, int y) const {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (isValidCoord(nx, ny) && field[nx][ny]->hasMine()) {
                count++;
            }
        }
    }
    return count;
}

void MineField::openEmptyArea(int x, int y) {
    if (!isValidCoord(x, y) || field[x][y]->getIsOpen()) {
        return;
    }
    
    field[x][y]->setOpen(true);
    
    if (field[x][y]->getAdjacentMines() == 0 && !field[x][y]->hasMine()) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                openEmptyArea(x + dx, y + dy);
            }
        }
    }
}

MineField::MineField(int w, int h, int mines) 
    : width(w), height(h), totalMines(mines), minesFound(0), 
      gameOver(false), gameWon(false) {
    
    // Инициализация поля безопасными клетками
    field.resize(height);
    for (int i = 0; i < height; i++) {
        field[i].resize(width);
        for (int j = 0; j < width; j++) {
            field[i][j] = new SafeCell();
        }
    }
}

MineField::~MineField() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            delete field[i][j];
        }
    }
}

void MineField::generateMines(int firstX, int firstY) {
    int minesPlaced = 0;
    srand(time(0));
    
    while (minesPlaced < totalMines) {
        int x = rand() % height;
        int y = rand() % width;
        
        // Не размещаем мину на первой открытой клетке и вокруг неё
        bool tooClose = false;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (x + dx == firstX && y + dy == firstY) {
                    tooClose = true;
                    break;
                }
            }
            if (tooClose) break;
        }
        
        if (!tooClose && !field[x][y]->hasMine()) {
            delete field[x][y];
            field[x][y] = new MineCell();
            minesPlaced++;
        }
    }
    
    // Подсчет соседних мин для всех клеток
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int count = countAdjacentMines(i, j);
            field[i][j]->setAdjacentMines(count);
        }
    }
}

bool MineField::openCell(int x, int y, bool firstMove) {
    if (!isValidCoord(x, y) || field[x][y]->getIsOpen() || 
        field[x][y]->getIsFlagged()) {
        return true;  // Игра продолжается
    }
    
    if (firstMove) {
        generateMines(x, y);
    }
    
    if (field[x][y]->hasMine()) {
        field[x][y]->setOpen(true);
        gameOver = true;
        return false;  // Игра окончена - поражение
    }
    
    openEmptyArea(x, y);
    checkWin();
    return !gameOver;
}

void MineField::toggleFlag(int x, int y) {
    if (!isValidCoord(x, y) || field[x][y]->getIsOpen()) {
        return;
    }
    
    bool wasFlagged = field[x][y]->getIsFlagged();
    field[x][y]->toggleFlag();
    
    // Обновляем счетчик найденных мин
    if (field[x][y]->getIsFlagged() && !wasFlagged && field[x][y]->hasMine()) {
        minesFound++;
    } else if (!field[x][y]->getIsFlagged() && wasFlagged && field[x][y]->hasMine()) {
        minesFound--;
    }
    
    checkWin();
}

void MineField::checkWin() {
    // Победа если все мины помечены или все безопасные клетки открыты
    int openedSafeCells = 0;
    int totalSafeCells = width * height - totalMines;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!field[i][j]->hasMine() && field[i][j]->getIsOpen()) {
                openedSafeCells++;
            }
        }
    }
    
    if (openedSafeCells == totalSafeCells) {
        gameWon = true;
        gameOver = true;
    }
}

void MineField::display(bool revealAll) const {
    cout << "\n   ";
    for (int j = 0; j < width; j++) {
        cout << j % 10 << " ";
    }
    cout << "\n";
    
    for (int i = 0; i < height; i++) {
        cout << i % 10 << "  ";
        for (int j = 0; j < width; j++) {
            if (revealAll && field[i][j]->hasMine()) {
                cout << "X ";
            } else {
                cout << field[i][j]->getDisplayChar() << " ";
            }
        }
        cout << "\n";
    }
    
    cout << "\nМин всего: " << totalMines;
    cout << " | Найдено: " << minesFound << "\n";
}

bool MineField::isGameOver() const { 
    return gameOver; 
}

bool MineField::isGameWon() const { 
    return gameWon; 
}

int MineField::getMinesFound() const { 
    return minesFound; 
}

int MineField::getTotalMines() const { 
    return totalMines; 
}
