package main

import (
	"fmt"
	"image/color"
	"strconv"
	"time"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
	"github.com/hajimehoshi/ebiten/v2/inpututil"
	"github.com/hajimehoshi/ebiten/v2/text"
	"golang.org/x/image/font"
	"golang.org/x/image/font/basicfont"
)

const (
	CellSize    = 32
	InfoHeight  = 80
	WindowTitle = "Minesweeper Go"
)

type Game struct {
	field       *MineField
	firstMove   bool
	gameRunning bool
	startTime   time.Time
	elapsed     time.Duration
	
	// Цвета
	bgColor         color.RGBA
	hiddenCellColor color.RGBA
	openCellColor   color.RGBA
	borderColor     color.RGBA
	flagColor       color.RGBA
	mineColor       color.RGBA
	explodedColor   color.RGBA
	infoPanelColor  color.RGBA
	textColor       color.RGBA
	
	// Шрифт
	font font.Face
}

func NewGame(width, height, mines int) *Game {
	// Проверка и корректировка параметров
	if width > 40 {
		width = 40
	}
	if height > 30 {
		height = 30
	}
	
	totalCells := width * height
	if mines >= totalCells {
		mines = totalCells - 9
	}
	
	fmt.Printf("Creating field: %dx%d, mines: %d\n", width, height, mines)
	
	g := &Game{
		field:       NewMineField(width, height, mines),
		firstMove:   true,
		gameRunning: true,
		startTime:   time.Now(),
		
		// Инициализация цветов
		bgColor:         color.RGBA{50, 50, 50, 255},
		hiddenCellColor: color.RGBA{180, 180, 180, 255}, // Более светлый серый
		openCellColor:   color.RGBA{220, 220, 220, 255}, // Очень светлый серый
		borderColor:     color.RGBA{100, 100, 100, 255}, // Темно-серый
		flagColor:       color.RGBA{255, 0, 0, 255},
		mineColor:       color.RGBA{0, 0, 0, 255},
		explodedColor:   color.RGBA{255, 0, 0, 255},
		infoPanelColor:  color.RGBA{70, 70, 70, 255},
		textColor:       color.RGBA{255, 255, 255, 255},
		
		// Шрифт
		font: basicfont.Face7x13,
	}
	
	return g
}

func (g *Game) Update() error {
	if !g.gameRunning {
		// Если игра окончена, ждем немного перед закрытием
		if time.Since(g.startTime.Add(g.elapsed)) > 3*time.Second {
			return ebiten.Termination
		}
		return nil
	}
	
	g.elapsed = time.Since(g.startTime)
	
	// Обработка ввода с клавиатуры
	if inpututil.IsKeyJustPressed(ebiten.KeyEscape) {
		return ebiten.Termination
	}
	
	if inpututil.IsKeyJustPressed(ebiten.KeyR) {
		// Рестарт игры
		g.field = NewMineField(g.field.GetWidth(), g.field.GetHeight(), g.field.GetTotalMines())
		g.firstMove = true
		g.gameRunning = true
		g.startTime = time.Now()
	}
	
	// Обработка кликов мыши
	if inpututil.IsMouseButtonJustPressed(ebiten.MouseButtonLeft) {
		x, y := ebiten.CursorPosition()
		
		// Проверяем, что клик внутри игрового поля
		if y < g.field.GetHeight()*CellSize {
			cellX := y / CellSize
			cellY := x / CellSize
			
			if !g.field.IsGameOver() {
				if !g.field.OpenCell(cellX, cellY, g.firstMove) {
					// Игра окончена - поражение
					g.gameRunning = false
				}
				g.firstMove = false
				
				if g.field.IsGameWon() {
					g.gameRunning = false
				}
			}
		}
	}
	
	// Обработка ПРАВОГО клика (флаг)
	if inpututil.IsMouseButtonJustPressed(ebiten.MouseButtonRight) {
		x, y := ebiten.CursorPosition()
		
		if y < g.field.GetHeight()*CellSize {
			cellX := y / CellSize
			cellY := x / CellSize
			
			if !g.field.IsGameOver() {
				g.field.ToggleFlag(cellX, cellY)
			}
		}
	}
	
	return nil
}

func (g *Game) Draw(screen *ebiten.Image) {
	screen.Fill(g.bgColor)
	
	// Рисуем игровое поле
	g.drawField(screen)
	
	// Рисуем панель информации
	g.drawUI(screen)
}

func (g *Game) drawField(screen *ebiten.Image) {
	for x := 0; x < g.field.GetHeight(); x++ {
		for y := 0; y < g.field.GetWidth(); y++ {
			cell := g.field.GetCell(x, y)
			if cell == nil {
				continue
			}
			
			posX := float64(y * CellSize)
			posY := float64(x * CellSize)
			
			// Рисуем фон клетки
			if !cell.GetIsOpen() && !cell.GetIsFlagged() {
				// Скрытая клетка - светлый серый с 3D эффектом
				ebitenutil.DrawRect(screen, posX, posY, CellSize, CellSize, g.hiddenCellColor)
				
				// Верхняя и левая границы - светлее
				ebitenutil.DrawLine(screen, posX, posY, posX+CellSize, posY, 
					color.RGBA{220, 220, 220, 255})
				ebitenutil.DrawLine(screen, posX, posY, posX, posY+CellSize, 
					color.RGBA{220, 220, 220, 255})
				
				// Нижняя и правая границы - темнее
				ebitenutil.DrawLine(screen, posX, posY+CellSize, posX+CellSize, posY+CellSize, 
					color.RGBA{100, 100, 100, 255})
				ebitenutil.DrawLine(screen, posX+CellSize, posY, posX+CellSize, posY+CellSize, 
					color.RGBA{100, 100, 100, 255})
				
			} else if cell.GetIsFlagged() {
				// Клетка с флагом
				ebitenutil.DrawRect(screen, posX, posY, CellSize, CellSize, g.hiddenCellColor)
				
				// Рисуем красный флаг (треугольник)
				ebitenutil.DrawLine(screen, posX+8, posY+8, posX+24, posY+16, g.flagColor)
				ebitenutil.DrawLine(screen, posX+24, posY+16, posX+8, posY+24, g.flagColor)
				ebitenutil.DrawLine(screen, posX+8, posY+8, posX+8, posY+24, g.flagColor)
				
				// Рисуем палку флага
				ebitenutil.DrawLine(screen, posX+8, posY+8, posX+8, posY+CellSize-8, 
					color.RGBA{139, 69, 19, 255})
				
				// Границы
				ebitenutil.DrawLine(screen, posX, posY, posX+CellSize, posY, 
					color.RGBA{220, 220, 220, 255})
				ebitenutil.DrawLine(screen, posX, posY, posX, posY+CellSize, 
					color.RGBA{220, 220, 220, 255})
				ebitenutil.DrawLine(screen, posX, posY+CellSize, posX+CellSize, posY+CellSize, 
					color.RGBA{100, 100, 100, 255})
				ebitenutil.DrawLine(screen, posX+CellSize, posY, posX+CellSize, posY+CellSize, 
					color.RGBA{100, 100, 100, 255})
				
			} else if cell.GetIsOpen() {
				// Открытая клетка
				ebitenutil.DrawRect(screen, posX, posY, CellSize, CellSize, g.openCellColor)
				
				// Тонкая темная граница
				ebitenutil.DrawRect(screen, posX, posY, CellSize, 1, g.borderColor)
				ebitenutil.DrawRect(screen, posX, posY, 1, CellSize, g.borderColor)
				ebitenutil.DrawRect(screen, posX, posY+CellSize-1, CellSize, 1, g.borderColor)
				ebitenutil.DrawRect(screen, posX+CellSize-1, posY, 1, CellSize, g.borderColor)
				
				if cell.HasMine() {
					// Мина - черный круг
					centerX := posX + CellSize/2
					centerY := posY + CellSize/2
					radius := float64(CellSize) / 4
					
					mineColor := g.mineColor
					if g.field.IsGameOver() && !g.field.IsGameWon() {
						mineColor = g.explodedColor
					}
					
					// Рисуем круг
					for angle := 0.0; angle < 360; angle += 10 {
						rad1 := angle * 3.14159 / 180
						rad2 := (angle + 10) * 3.14159 / 180
						x1 := centerX + radius*float64(mathCos(rad1))
						y1 := centerY + radius*float64(mathSin(rad1))
						x2 := centerX + radius*float64(mathCos(rad2))
						y2 := centerY + radius*float64(mathSin(rad2))
						
						ebitenutil.DrawLine(screen, x1, y1, x2, y2, mineColor)
					}
					
					// Детали мины
					ebitenutil.DrawLine(screen, centerX-3, centerY, centerX+3, centerY, mineColor)
					ebitenutil.DrawLine(screen, centerX, centerY-3, centerX, centerY+3, mineColor)
					
				} else {
					// Число соседних мин
					adjacent := cell.GetAdjacentMines()
					if adjacent > 0 {
						numStr := strconv.Itoa(adjacent)
						numColor := cell.GetNumberColor()
						
						// Рисуем текст в центре клетки
						textWidth := len(numStr) * 7
						textHeight := 13
						textX := int(posX) + (CellSize-textWidth)/2
						textY := int(posY) + (CellSize+textHeight)/2 - 2
						
						text.Draw(screen, numStr, g.font, textX, textY, numColor)
					}
				}
			}
		}
	}
}

// Простые тригонометрические функции для избежания импорта math
func mathSin(x float64) float64 {
	// Приближение sin через ряд Тейлора
	return x - (x*x*x)/6 + (x*x*x*x*x)/120
}

func mathCos(x float64) float64 {
	// Приближение cos через ряд Тейлора
	return 1 - (x*x)/2 + (x*x*x*x)/24
}

func (g *Game) drawUI(screen *ebiten.Image) {
	// Рисуем панель информации
	infoY := g.field.GetHeight() * CellSize
	ebitenutil.DrawRect(screen, 0, float64(infoY), 
		float64(screen.Bounds().Dx()), InfoHeight, g.infoPanelColor)
	
	// Разделительная линия
	ebitenutil.DrawLine(screen, 0, float64(infoY),
		float64(screen.Bounds().Dx()), float64(infoY),
		g.textColor)
	
	// Отображаем статус игры
	status := "PLAYING..."
	statusColor := g.textColor
	if g.field.IsGameOver() {
		if g.field.IsGameWon() {
			status = "YOU WIN!"
			statusColor = color.RGBA{0, 255, 0, 255}
		} else {
			status = "GAME OVER!"
			statusColor = color.RGBA{255, 0, 0, 255}
		}
	}
	text.Draw(screen, status, g.font, 10, infoY+20, statusColor)
	
	// Отображаем счетчик мин
	minesText := fmt.Sprintf("MINES: %d/%d",
		g.field.GetMinesFound(), g.field.GetTotalMines())
	text.Draw(screen, minesText, g.font, 200, infoY+20, color.RGBA{0, 255, 0, 255})
	
	// Отображаем таймер
	seconds := int(g.elapsed.Seconds())
	timerText := fmt.Sprintf("TIME: %d SEC", seconds)
	text.Draw(screen, timerText, g.font, 400, infoY+20, color.RGBA{255, 255, 0, 255})
	
	// Инструкция
	instruction := "LMB - OPEN | RMB - FLAG | R - RESTART | ESC - EXIT"
	text.Draw(screen, instruction, g.font, 10, infoY+50, g.textColor)
	
	// Отладочная информация
	debugInfo := fmt.Sprintf("Cells: %dx%d | First: %v", 
		g.field.GetWidth(), g.field.GetHeight(), g.firstMove)
	text.Draw(screen, debugInfo, g.font, 600, infoY+20, color.RGBA{200, 200, 255, 255})
}

func (g *Game) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return g.field.GetWidth() * CellSize,
		g.field.GetHeight()*CellSize + InfoHeight
}
