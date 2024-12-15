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
     bool checkWin(char piece) const {
        // Check horizontal
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                bool win = true;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row][col + k] != piece) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        // Check vertical
        for (int row = 0; row <= ROWS - WIN_LENGTH; row++) {
            for (int col = 0; col < COLS; col++) {
                bool win = true;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row + k][col] != piece) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        // Check diagonal (downward)
        for (int row = 0; row <= ROWS - WIN_LENGTH; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                bool win = true;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row + k][col + k] != piece) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        // Check diagonal (upward)
        for (int row = WIN_LENGTH - 1; row < ROWS; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                bool win = true;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row - k][col + k] != piece) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        return false;
    }

    int evaluateBoard(char piece) const {
        char opponent = (piece == PLAYER_PIECE) ? COMPUTER_PIECE : PLAYER_PIECE;
        int score = 0;

        // Check horizontal windows
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                int pieceCount = 0, emptyCount = 0, opponentCount = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row][col + k] == piece) pieceCount++;
                    else if (grid[row][col + k] == EMPTY) emptyCount++;
                    else opponentCount++;
                }
                score += evaluateWindow(pieceCount, emptyCount, opponentCount);
            }
        }

        // Check vertical windows
        for (int row = 0; row <= ROWS - WIN_LENGTH; row++) {
            for (int col = 0; col < COLS; col++) {
                int pieceCount = 0, emptyCount = 0, opponentCount = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row + k][col] == piece) pieceCount++;
                    else if (grid[row + k][col] == EMPTY) emptyCount++;
                    else opponentCount++;
                }
                score += evaluateWindow(pieceCount, emptyCount, opponentCount);
            }
        }

                // Check diagonal windows (downward)
        for (int row = 0; row <= ROWS - WIN_LENGTH; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                int pieceCount = 0, emptyCount = 0, opponentCount = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row + k][col + k] == piece) pieceCount++;
                    else if (grid[row + k][col + k] == EMPTY) emptyCount++;
                    else opponentCount++;
                }
                score += evaluateWindow(pieceCount, emptyCount, opponentCount);
            }
        }

        // Check diagonal windows (upward)
        for (int row = WIN_LENGTH - 1; row < ROWS; row++) {
            for (int col = 0; col <= COLS - WIN_LENGTH; col++) {
                int pieceCount = 0, emptyCount = 0, opponentCount = 0;
                for (int k = 0; k < WIN_LENGTH; k++) {
                    if (grid[row - k][col + k] == piece) pieceCount++;
                    else if (grid[row - k][col + k] == EMPTY) emptyCount++;
                    else opponentCount++;
                }
                score += evaluateWindow(pieceCount, emptyCount, opponentCount);
            }
        }

        // Center column preference (strategic advantage)
        int centerCol = COLS / 2;
        int centerPieceCount = 0;
        for (int row = 0; row < ROWS; row++) {
            if (grid[row][centerCol] == piece) {
                centerPieceCount++;
            }
        }
        score += centerPieceCount * 3;

        return score;
    }

    int evaluateWindow(int pieceCount, int emptyCount, int opponentCount) const {
        // Score the window based on piece configurations
        if (pieceCount == 4) return 100;
        if (pieceCount == 3 && emptyCount == 1) return 5;
        if (pieceCount == 2 && emptyCount == 2) return 2;
        if (opponentCount == 3 && emptyCount == 1) return -4;  // Block opponent wins
        return 0;
    }

    Board getCopy() const {
        Board copy;
        copy.grid = this->grid;
        copy.lastMoveRow = this->lastMoveRow;
        copy.lastMoveCol = this->lastMoveCol;
        return copy;
    }

    vector<int> getValidMoves() const {
        vector<int> validMoves;
        for (int col = 0; col < COLS; col++) {
            if (!isColumnFull(col)) {
                validMoves.push_back(col);
            }
        }
        return validMoves;
    }

    int getLastMoveRow() const { return lastMoveRow; }
    int getLastMoveCol() const { return lastMoveCol; }

    void print() const {
        // Print column numbers
        cout << "\n ";
        for (int col = 0; col < COLS; col++) {
            cout << " " << col + 1 << "  ";
        }
        cout << "\n";

        // Print top border
        cout << " ";
        for (int col = 0; col < COLS; col++) {
            cout << "----";
        }
        cout << "-\n";

        // Print rows with side borders
        for (int row = 0; row < ROWS; row++) {
            cout << "| ";
            for (int col = 0; col < COLS; col++) {
                cout << grid[row][col] << " | ";
            }
            cout << "\n";

            // Print row separator
            cout << " ";
            for (int col = 0; col < COLS; col++) {
                cout << "----";
            }
            cout << "-\n";
        }
        cout << endl;
    }
};

// Computer Player class
class ComputerPlayer {
private:
    Difficulty difficulty;
    char computerPiece;
    char playerPiece;
    int nodesExplored;
    mt19937 rng;

public:
    ComputerPlayer(char piece) :
        difficulty(Difficulty::BEGINNER),
        computerPiece(piece),
        playerPiece((piece == PLAYER_PIECE) ? COMPUTER_PIECE : PLAYER_PIECE),
        nodesExplored(0) {
        // Initialize random number generator
        random_device rd;
        rng = mt19937(rd());
    }

    void setDifficulty(Difficulty diff) {
        difficulty = diff;
    }

    Difficulty getDifficulty() const {
        return difficulty;
    }

    void increaseDifficulty() {
        if (difficulty == Difficulty::BEGINNER) {
            difficulty = Difficulty::INTERMEDIATE;
            cout << "\nComputer difficulty increased to INTERMEDIATE!\n";
        } else if (difficulty == Difficulty::INTERMEDIATE) {
            difficulty = Difficulty::ADVANCED;
            cout << "\nComputer difficulty increased to ADVANCED!\n";
        } else if (difficulty == Difficulty::ADVANCED) {
            difficulty = Difficulty::EXPERT;
            cout << "\nComputer difficulty increased to EXPERT!\n";
        }
    }

    int makeMove(const Board& board) {
        nodesExplored = 0;
        vector<int> validMoves = board.getValidMoves();

        if (validMoves.empty()) {
            return -1;  // No valid moves
        }

        // Simulate thinking
        cout << "Computer is thinking";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(300));