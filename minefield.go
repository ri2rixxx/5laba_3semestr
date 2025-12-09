package main

import (
	"math/rand"
	"time"
)

type MineField struct {
	width      int
	height     int
	totalMines int
	minesFound int
	gameOver   bool
	gameWon    bool
	field      [][]Cell
}

func NewMineField(width, height, mines int) *MineField {
	mf := &MineField{
		width:      width,
		height:     height,
		totalMines: mines,
		minesFound: 0,
		gameOver:   false,
		gameWon:    false,
		field:      make([][]Cell, height),
	}
	
	// Инициализация поля безопасными клетками
	for i := 0; i < height; i++ {
		mf.field[i] = make([]Cell, width)
		for j := 0; j < width; j++ {
			mf.field[i][j] = NewSafeCell()
		}
	}
	
	return mf
}

func (mf *MineField) IsValidCoord(x, y int) bool {
	return x >= 0 && x < mf.height && y >= 0 && y < mf.width
}

func (mf *MineField) CountAdjacentMines(x, y int) int {
	count := 0
	for dx := -1; dx <= 1; dx++ {
		for dy := -1; dy <= 1; dy++ {
			if dx == 0 && dy == 0 {
				continue
			}
			nx := x + dx
			ny := y + dy
			if mf.IsValidCoord(nx, ny) && mf.field[nx][ny].HasMine() {
				count++
			}
		}
	}
	return count
}

func (mf *MineField) GenerateMines(firstX, firstY int) {
	minesPlaced := 0
	rand.Seed(time.Now().UnixNano())
	
	for minesPlaced < mf.totalMines {
		x := rand.Intn(mf.height)
		y := rand.Intn(mf.width)
		
		// Не размещаем мину на первой открытой клетке и вокруг неё
		tooClose := false
		for dx := -1; dx <= 1; dx++ {
			for dy := -1; dy <= 1; dy++ {
				if x+dx == firstX && y+dy == firstY {
					tooClose = true
					break
				}
			}
			if tooClose {
				break
			}
		}
		
		if !tooClose && !mf.field[x][y].HasMine() {
			mf.field[x][y] = NewMineCell()
			minesPlaced++
		}
	}
	
	// Подсчет соседних мин для всех клеток
	for i := 0; i < mf.height; i++ {
		for j := 0; j < mf.width; j++ {
			count := mf.CountAdjacentMines(i, j)
			mf.field[i][j].SetAdjacentMines(count)
		}
	}
}

func (mf *MineField) OpenEmptyArea(x, y int) {
	// Используем стек для итеративного открытия
	stack := [][2]int{{x, y}}
	
	for len(stack) > 0 {
		current := stack[len(stack)-1]
		stack = stack[:len(stack)-1]
		cx, cy := current[0], current[1]
		
		if !mf.IsValidCoord(cx, cy) || mf.field[cx][cy].GetIsOpen() {
			continue
		}
		
		mf.field[cx][cy].SetOpen(true)
		
		// Если клетка пустая, добавляем ее соседей
		if mf.field[cx][cy].GetAdjacentMines() == 0 && !mf.field[cx][cy].HasMine() {
			for dx := -1; dx <= 1; dx++ {
				for dy := -1; dy <= 1; dy++ {
					if dx == 0 && dy == 0 {
						continue
					}
					nx := cx + dx
					ny := cy + dy
					if mf.IsValidCoord(nx, ny) && !mf.field[nx][ny].GetIsOpen() &&
						!mf.field[nx][ny].GetIsFlagged() {
						stack = append(stack, [2]int{nx, ny})
					}
				}
			}
		}
	}
}

func (mf *MineField) OpenCell(x, y int, firstMove bool) bool {
	if !mf.IsValidCoord(x, y) || mf.field[x][y].GetIsOpen() ||
		mf.field[x][y].GetIsFlagged() {
		return true // Игра продолжается
	}
	
	if firstMove {
		mf.GenerateMines(x, y)
	}
	
	if mf.field[x][y].HasMine() {
		mf.field[x][y].SetOpen(true)
		mf.gameOver = true
		return false // Игра окончена - поражение
	}
	
	mf.OpenEmptyArea(x, y)
	mf.CheckWin()
	return !mf.gameOver
}

func (mf *MineField) ToggleFlag(x, y int) {
	if !mf.IsValidCoord(x, y) || mf.field[x][y].GetIsOpen() {
		return
	}
	
	wasFlagged := mf.field[x][y].GetIsFlagged()
	mf.field[x][y].ToggleFlag()
	
	// Обновляем счетчик найденных мин
	if mf.field[x][y].GetIsFlagged() && !wasFlagged && mf.field[x][y].HasMine() {
		mf.minesFound++
	} else if !mf.field[x][y].GetIsFlagged() && wasFlagged && mf.field[x][y].HasMine() {
		mf.minesFound--
	}
	
	mf.CheckWin()
}

func (mf *MineField) CheckWin() {
	// Победа если все мины помечены или все безопасные клетки открыты
	openedSafeCells := 0
	totalSafeCells := mf.width*mf.height - mf.totalMines
	
	for i := 0; i < mf.height; i++ {
		for j := 0; j < mf.width; j++ {
			if !mf.field[i][j].HasMine() && mf.field[i][j].GetIsOpen() {
				openedSafeCells++
			}
		}
	}
	
	if openedSafeCells == totalSafeCells {
		mf.gameWon = true
		mf.gameOver = true
	}
}

func (mf *MineField) IsGameOver() bool {
	return mf.gameOver
}

func (mf *MineField) IsGameWon() bool {
	return mf.gameWon
}

func (mf *MineField) GetMinesFound() int {
	return mf.minesFound
}

func (mf *MineField) GetTotalMines() int {
	return mf.totalMines
}

func (mf *MineField) GetWidth() int {
	return mf.width
}

func (mf *MineField) GetHeight() int {
	return mf.height
}

func (mf *MineField) GetCell(x, y int) Cell {
	if mf.IsValidCoord(x, y) {
		return mf.field[x][y]
	}
	return nil
}
