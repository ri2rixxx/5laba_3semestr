#ifndef MINESWEEPERGAME_H
#define MINESWEEPERGAME_H

#include "MineField.h"

// Класс игры
class MinesweeperGame {
private:
    MineField* field;
    bool firstMove;
    
public:
    MinesweeperGame(int width, int height, int mines);
    ~MinesweeperGame();
    
    void play();
};

#endif
