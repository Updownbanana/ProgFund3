#include "MinesweeperBoard.h"
#include "MinesweeperGame.h"
#include <ctime>
#include <cassert>
using namespace std;

// default constructor; returns a completely empty board with dimensions (0,0)
MinesweeperBoard::MinesweeperBoard() {
	width = 0;
	height = 0;
	mineCount = 0;
	grid = { {} };
}

// parameterized constructor using width, height, and mine count; mine count must fit within grid of specified size
MinesweeperBoard::MinesweeperBoard(const int w, const int h, const int m) {
	width = w;
	height = h;
	mineCount = m;
	assert(width * height >= mineCount);
	for (int i = 0; i < height; i++) {
		vector<int> new_row(width, 0);
		grid.push_back(new_row);
	}
	placeMines();
}

// method for randomly placing the set number of mines in an empty grid
void MinesweeperBoard::placeMines() {
	srand((unsigned)time(0));
	int x = rand() % width;
	int y = rand() % height;
	for (int m = 0; m < mineCount; m++) {
		while (grid[x][y] < 0) {
			x = rand() % width;
			y = rand() % height;
		}
		grid[x][y] = -1;
		for (int i = max(x - 1, 0); i <= min(x + 1, width - 1); i++) {
			for (int j = max(y - 1, 0); j <= min(y + 1, height - 1); j++) {
				if (grid[i][j] >= 0) {
					grid[i][j]++;
				}
			}
		}
	}
}

// getters
int MinesweeperBoard::getSquare(const int x, const int y) const {
	return grid[x][y];
}
int MinesweeperBoard::getHeight() const {
	return height;
}
int MinesweeperBoard::getWidth() const {
	return width;
}
int MinesweeperBoard::getMineCount() const {
	return mineCount;
}

// toString method for debugging purposes
string MinesweeperBoard::toString() const {
	string out = "";
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (grid[i][j] < 0) {
				out.append("*");
			}
			else {
				out.append(to_string(grid[i][j]));
			}
			out.append(" ");
		}
		out.append("\n");
	}
	return out;
}


MinesweeperGame::MinesweeperGame() {
	board = MinesweeperBoard();
	revealedGrid = { {} };
	gameWon = false;
	gameLost = false;
	revealCount = 0;
}

MinesweeperGame::MinesweeperGame(const int width, const int height, const int mineCount) {
	board = MinesweeperBoard(width, height, mineCount);
	for (int i = 0; i < height; i++) {
		vector<bool> new_row(width, false);
		revealedGrid.push_back(new_row);
	}
	gameWon = false;
	gameLost = false;
	revealCount = 0;
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