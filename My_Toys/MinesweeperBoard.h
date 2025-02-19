/* Minesweeper Board class
* Uses 2d vectors to represent a board for the game Minesweeper
* On instantiation, randomly places a set number of mines (represented as -1) within the given width and height, then sets all other squares to a number equal to the number of surrounding mines
* Client can get the value of a particular square, as well as the width, height, and mine count
* Client can also get a string representing the current board as a grid; this is for debugging purposes and should not be used as part of a game
*/

#ifndef _MINESWEEPER_BOARD
#define _MINESWEEPER_BOARD

#include <vector>
#include <string>
using namespace std;

class MinesweeperBoard {
private:
	// attributes
	vector<vector<int>> grid;
	int width;
	int height;
	int mineCount;

	// method used by the constructor to randomly place mines
	void placeMines();
public:
	// default constructor
	MinesweeperBoard();

	// parameterized constructor; randomly generates board of given width, height, and depth
	MinesweeperBoard(const int w, const int h, const int m);

	// returns value of a given square; mines are represented as -1, and other numbers are given as-is. coordinates are zero-indexed. 
	int getSquare(const int x, const int y) const;

	// getters
	int getWidth() const;
	int getHeight() const;
	int getMineCount() const;

	// returns a string representing the board as a grid of characters
	string toString() const;
};

#endif
