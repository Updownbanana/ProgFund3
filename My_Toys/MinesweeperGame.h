#ifndef _MINESWEEPER_GAME
#define _MINESWEEPER_GAME

#include "MinesweeperBoard.h"
class MinesweeperGame {
private:
	vector<vector<bool>> revealedGrid; 
	MinesweeperBoard board;
	bool gameWon;
	bool gameLost;
	int revealCount;
	void checkWin();
public:
	// Default constructor; creates a completely empty game
	MinesweeperGame();

	// Parameterized constructor; parameters determine size and mine count of board
	MinesweeperGame(const int width, const int height, const int mines);

	// Reveals a given square and returns true if that square contained a mine
	bool revealSquare(const int x, const int y);

	// Returns true if the given square has been revealed
	bool getSquareRevealed(const int x, const int y);

	// Getter for win state; returns true if all non-mines have been revealed
	bool getGameWon();

	// Getter for loss state; returns true if a mine has been revealed
	bool getGameLost();
};

#endif
