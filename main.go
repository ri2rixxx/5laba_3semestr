package main

import (
	"fmt"
	"os"
	"strconv"

	"github.com/hajimehoshi/ebiten/v2"
)

func main() {
	fmt.Println("=== MINESWEEPER IN GO ===")
	
	// Параметры по умолчанию
	width := 16
	height := 16
	mines := 40
	
	// Чтение аргументов командной строки
	if len(os.Args) > 1 {
		if w, err := strconv.Atoi(os.Args[1]); err == nil && w >= 5 && w <= 40 {
			width = w
		}
	}
	
	if len(os.Args) > 2 {
		if h, err := strconv.Atoi(os.Args[2]); err == nil && h >= 5 && h <= 30 {
			height = h
		}
	}
	
	if len(os.Args) > 3 {
		maxMines := width*height - 9
		if m, err := strconv.Atoi(os.Args[3]); err == nil && m >= 1 && m <= maxMines {
			mines = m
		}
	}
	
	// Проверка корректности
	if width < 5 || width > 40 {
		width = 16
		fmt.Println("Width must be between 5 and 40. Using default: 16")
	}
	
	if height < 5 || height > 30 {
		height = 16
		fmt.Println("Height must be between 5 and 30. Using default: 16")
	}
	
	maxMines := width*height - 9
	if mines < 1 || mines > maxMines {
		mines = maxMines / 2
		if mines < 10 {
			mines = 10
		}
		fmt.Printf("Mines must be between 1 and %d. Using: %d\n", maxMines, mines)
	}
	
	fmt.Printf("Starting game with: %dx%d, mines: %d\n", width, height, mines)
	fmt.Println("Controls:")
	fmt.Println("  Left Click - Open cell")
	fmt.Println("  Right Click - Toggle flag")
	fmt.Println("  R - Restart game")
	fmt.Println("  ESC - Exit")
	fmt.Println()
	
	game := NewGame(width, height, mines)
	
	ebiten.SetWindowTitle(WindowTitle)
	ebiten.SetWindowSize(width*CellSize, height*CellSize+InfoHeight)
	ebiten.SetWindowResizingMode(ebiten.WindowResizingModeDisabled)
	
	if err := ebiten.RunGame(game); err != nil {
		if err == ebiten.Termination {
			fmt.Println("Game terminated normally")
		} else {
			fmt.Printf("Error running game: %v\n", err)
		}
	}
}
