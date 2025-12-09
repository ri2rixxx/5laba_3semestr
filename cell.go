package main

import "image/color"

// Cell интерфейс для всех типов клеток
type Cell interface {
	HasMine() bool
	GetAdjacentMines() int
	SetAdjacentMines(count int)
	GetDisplayChar() string
	GetNumberColor() color.Color
	
	GetIsOpen() bool
	GetIsFlagged() bool
	SetOpen(open bool)
	ToggleFlag()
}

// BaseCell базовая структура клетки
type BaseCell struct {
	isOpen    bool
	isFlagged bool
}

func (c *BaseCell) GetIsOpen() bool {
	return c.isOpen
}

func (c *BaseCell) GetIsFlagged() bool {
	return c.isFlagged
}

func (c *BaseCell) SetOpen(open bool) {
	if !c.isFlagged {
		c.isOpen = open
	}
}

func (c *BaseCell) ToggleFlag() {
	if !c.isOpen {
		c.isFlagged = !c.isFlagged
	}
}

// SafeCell безопасная клетка
type SafeCell struct {
	BaseCell
	adjacentMines int
}

func NewSafeCell() *SafeCell {
	return &SafeCell{
		BaseCell: BaseCell{},
	}
}

func (c *SafeCell) HasMine() bool {
	return false
}

func (c *SafeCell) GetAdjacentMines() int {
	return c.adjacentMines
}

func (c *SafeCell) SetAdjacentMines(count int) {
	c.adjacentMines = count
}

func (c *SafeCell) GetDisplayChar() string {
	if c.isFlagged {
		return "F"
	}
	if !c.isOpen {
		return "*"
	}
	if c.adjacentMines == 0 {
		return "."
	}
	return string('0' + byte(c.adjacentMines))
}

func (c *SafeCell) GetNumberColor() color.Color {
	if !c.isOpen || c.isFlagged {
		return color.White
	}
	
	switch c.adjacentMines {
	case 1:
		return color.RGBA{0, 0, 255, 255} // Blue
	case 2:
		return color.RGBA{0, 150, 0, 255} // Green
	case 3:
		return color.RGBA{255, 0, 0, 255} // Red
	case 4:
		return color.RGBA{128, 0, 128, 255} // Purple
	case 5:
		return color.RGBA{128, 0, 0, 255} // Dark Red
	case 6:
		return color.RGBA{64, 224, 208, 255} // Turquoise
	case 7:
		return color.Black
	case 8:
		return color.RGBA{128, 128, 128, 255} // Gray
	default:
		return color.White
	}
}

// MineCell клетка с миной
type MineCell struct {
	BaseCell
	adjacentMines int
}

func NewMineCell() *MineCell {
	return &MineCell{
		BaseCell: BaseCell{},
	}
}

func (c *MineCell) HasMine() bool {
	return true
}

func (c *MineCell) GetAdjacentMines() int {
	return c.adjacentMines
}

func (c *MineCell) SetAdjacentMines(count int) {
	c.adjacentMines = count
}

func (c *MineCell) GetDisplayChar() string {
	if c.isFlagged {
		return "F"
	}
	if !c.isOpen {
		return "*"
	}
	return "X"
}

func (c *MineCell) GetNumberColor() color.Color {
	return color.RGBA{255, 0, 0, 255} // Red
}
