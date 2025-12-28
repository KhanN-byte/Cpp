//
// Created by haris on 12/20/2025.
//

#include "TicTacToe.h"

#include <iostream>
#include <array>
#include <limits>

using namespace std;

void printBoard(const array<char, 9>& b) {
    auto cell = [&](int i) -> char {
        return (b[i] == ' ') ? char('1' + i) : b[i];
    };

    cout << "\n";
    cout << " " << cell(0) << " | " << cell(1) << " | " << cell(2) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(3) << " | " << cell(4) << " | " << cell(5) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(6) << " | " << cell(7) << " | " << cell(8) << "\n";
    cout << "\n";
}

bool isWin(const array<char, 9>& b, char p) {
    const int wins[8][3] = {
            {0,1,2},{3,4,5},{6,7,8}, // rows
            {0,3,6},{1,4,7},{2,5,8}, // cols
            {0,4,8},{2,4,6}          // diagonals
    };
    for (auto& w : wins) {
        if (b[w[0]] == p && b[w[1]] == p && b[w[2]] == p) return true;
    }
    return false;
}

bool isDraw(const array<char, 9>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int readMove(const array<char, 9>& b, char player) {
    while (true) {
        cout << "Player " << player << ", choose a square (1-9): ";
        int pos;
        if (!(cin >> pos)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number 1-9.\n";
            continue;
        }
        if (pos < 1 || pos > 9) {
            cout << "Out of range. Pick 1-9.\n";
            continue;
        }
        int idx = pos - 1;
        if (b[idx] != ' ') {
            cout << "That square is taken. Try again.\n";
            continue;
        }
        return idx;
    }
}

int main() {
    array<char, 9> board;
    board.fill(' ');

    char current = 'X';

    cout << "=== Tic-Tac-Toe (2 Player) ===\n";
    cout << "Enter moves by choosing a number 1-9.\n";

    while (true) {
        printBoard(board);

        int move = readMove(board, current);
        board[move] = current;

        if (isWin(board, current)) {
            printBoard(board);
            cout << "🎉 Player " << current << " wins!\n";
            break;
        }
        if (isDraw(board)) {
            printBoard(board);
            cout << "It's a draw!\n";
            break;
        }

        current = (current == 'X') ? 'O' : 'X';
    }

    return 0;
}
