#ifndef _CHESS_BOARD
#define _CHESS_BOARD

#include <string>
using namespace std;

template<typename L>
class ChessBoard {
private:
	L board;
	void setPiece(char piece, int x, int y, bool white); // places given piece at given position
public:
	ChessBoard();
	//ChessBoard(ChessBoard other);
	//string getPiece(string alg); // returns piece at a given position in algebraic notation (letter and number)
	//string getPiece(int x, int y); // returns piece at the given zero-indexed position
	string getMoves(string alg); // returns all moves of the piece at the given position in algebraic notation
	string getMoves(int x, int y); // returns all moves of the piece at the given zero-indexed position
	bool makeMove(string alg); // attempts to make the given move in algebraic notation as either white or black, depending on previous move; returns true if move is successful
	string toString(); // returns ASCII representation of current board, with pawns as p and empty spaces as .
};

#endif
