#include "MinesweeperGame.h"

MinesweeperGame::MinesweeperGame() {
	board = MinesweeperBoard();
	revealedGrid = { {} };
	gameWon = false;
	gameLost = false;
}

MinesweeperGame::MinesweeperGame(const int width, const int height, const int mineCount) {
	board = MinesweeperBoard(width, height, mineCount);
	for (int i = 0; i < height; i++) {
		vector<bool> new_row(width, false);
		revealedGrid.push_back(new_row);
	}
	gameWon = false;
	gameLost = false;
}

bool MinesweeperGame::revealSquare(const int x, const int y) {
	revealedGrid[x][y] = true;
	if (board.getSquare(x, y) < 0) {
		gameLost = true;
		return true;
	}
	revealCount++;
	checkWin();
}

bool MinesweeperGame::getSquareRevealed(const int x, const int y) {
	return revealedGrid[x][y];
}

bool MinesweeperGame::getGameLost() {
	return gameLost;
}

bool MinesweeperGame::getGameWon() {
	return gameWon;
}

void MinesweeperGame::checkWin() {
	if (revealCount + board.getMineCount() == board.getWidth() * board.getHeight() and not gameLost) {
		gameWon = true;
	}
	else {
		gameWon = false;
	}
}