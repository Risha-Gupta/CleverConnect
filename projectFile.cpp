#include <bits/stdc++.h>

using namespace std;

// Constants
const int ROWS = 6;
const int COLS = 7;
const char EMPTY = ' ';
char PLAYER_PIECE = 'X';
char COMPUTER_PIECE = 'O';
const int WIN_LENGTH = 4;
const int MAX_DEPTH = 8;  // Maximum depth for minimax algorithm

// Game difficulty levels
enum class Difficulty { BEGINNER, INTERMEDIATE, ADVANCED, EXPERT };

// Forward declarations
class Board;
class ComputerPlayer;
class Game;
// Board class to represent the game state
class Board {
private:
    vector<vector<char>> grid;
    int lastMoveRow;
    int lastMoveCol;

public:
    Board() : lastMoveRow(-1), lastMoveCol(-1) {
        grid.resize(ROWS, vector<char>(COLS, EMPTY));
    }

    void reset() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                grid[i][j] = EMPTY;
            }
        }
        lastMoveRow = -1;
        lastMoveCol = -1;
    }

    bool isColumnFull(int col) const {
        return grid[0][col] != EMPTY;
    }

    bool isBoardFull() const {
        for (int col = 0; col < COLS; col++) {
            if (!isColumnFull(col)) return false;
        }
        return true;
    }

    bool dropPiece(int col, char piece) {
        if (col < 0 || col >= COLS || isColumnFull(col)) {
            return false;
        }

        // Find the lowest empty row in the column
        int row = ROWS - 1;
        while (row >= 0 && grid[row][col] != EMPTY) {
            row--;
        }

        if (row >= 0) {
            grid[row][col] = piece;
            lastMoveRow = row;
            lastMoveCol = col;
            return true;
        }

        return false;
    }

    char getPiece(int row, int col) const {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            return grid[row][col];
        }
        return EMPTY;
    }

    void setPiece(int row, int col, char piece) {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            grid[row][col] = piece;
        }
    }