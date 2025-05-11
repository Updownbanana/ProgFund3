
#include <iostream>
#include "MinesweeperBoard.h"

string DiscordMinesweeper(int width, int height, int mineCount, string mine, string numberSpacing = "") {
    MinesweeperBoard board(width, height, mineCount);
    string message = "";
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            message.append("||");
            int currentSquare = board.getSquare(i, j);
            if (currentSquare < 0) {
                message.append(mine);
            }
            else {
                message.append(to_string(currentSquare) + numberSpacing);
            }
            message.append("||");
        }
        message.append("\n");
    }
    return message;
}

int main()
{
    cout << DiscordMinesweeper(6, 6, 12, ":boom:", "   ");
}
