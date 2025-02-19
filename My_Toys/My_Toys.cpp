// My_Toys.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
