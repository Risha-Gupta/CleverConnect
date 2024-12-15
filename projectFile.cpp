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

                    cout << endl;

        int move = -1;
        switch (difficulty) {
            case Difficulty::BEGINNER:
                move = greedyMove(board);
                break;
            case Difficulty::INTERMEDIATE:
                move = minimaxMove(board, 3);
                break;
            case Difficulty::ADVANCED:
                move = minimaxMove(board, 5);
                break;
            case Difficulty::EXPERT:
                move = minimaxMove(board, MAX_DEPTH);
                break;
        }

        cout << "Computer explored " << nodesExplored << " positions." << endl;
        return move;
    }

    // Make this method public so we can use it in our solution finder
    int minimaxMove(const Board& board, int depth) {
        vector<int> validMoves = board.getValidMoves();

        // Add a bit of randomness at lower depths for variety
        if (depth < 5) {
            shuffle(validMoves.begin(), validMoves.end(), rng);
        }

        int bestScore = numeric_limits<int>::min();
        vector<int> bestMoves;

        // Check each valid move
        for (int col : validMoves) {
            Board tempBoard = board.getCopy();
            tempBoard.dropPiece(col, computerPiece);

            // If this move wins the game, choose it
            if (tempBoard.checkWin(computerPiece)) {
                return col;
            }

            int score = minimax(tempBoard, depth - 1, numeric_limits<int>::min(),
                                numeric_limits<int>::max(), false);

            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(col);
            } else if (score == bestScore) {
                bestMoves.push_back(col);
            }
        }

        // Choose randomly among equally good moves for variety
        if (bestMoves.empty()) {
            return validMoves[0];
        } else {
            uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
            return bestMoves[dist(rng)];
        }
    }

private:
    int greedyMove(const Board& board) {
        vector<int> validMoves = board.getValidMoves();

        // Shuffle the valid moves for randomness at beginner level
        shuffle(validMoves.begin(), validMoves.end(), rng);

        // Check if Computer can win in the next move
        for (int col : validMoves) {
            Board tempBoard = board.getCopy();
            tempBoard.dropPiece(col, computerPiece);
            if (tempBoard.checkWin(computerPiece)) {
                return col;  // Win in one move
            }
            nodesExplored++;
        }

        // Block player's winning move
        for (int col : validMoves) {
            Board tempBoard = board.getCopy();
            tempBoard.dropPiece(col, playerPiece);
            if (tempBoard.checkWin(playerPiece)) {
                return col;  // Block player's win
            }
            nodesExplored++;
        }

        // Prefer center column as it offers more winning opportunities
        int centerCol = COLS / 2;
        if (find(validMoves.begin(), validMoves.end(), centerCol) != validMoves.end()) {
            return centerCol;
        }

        // Otherwise, make a random move
        return validMoves[0];
    }

    int minimax(Board board, int depth, int alpha, int beta, bool isMaximizing) {
        nodesExplored++;

        // Terminal conditions
        if (board.checkWin(computerPiece)) {
            return 1000 + depth;  // Prefer quicker wins
        }
        if (board.checkWin(playerPiece)) {
            return -1000 - depth;  // Avoid quicker losses
        }
        if (board.isBoardFull() || depth == 0) {
            return board.evaluateBoard(computerPiece);
        }

        vector<int> validMoves = board.getValidMoves();

        if (isMaximizing) {
            int maxScore = numeric_limits<int>::min();
            for (int col : validMoves) {
                Board tempBoard = board.getCopy();
                tempBoard.dropPiece(col, computerPiece);
                int score = minimax(tempBoard, depth - 1, alpha, beta, false);
                maxScore = max(maxScore, score);
                alpha = max(alpha, score);
                if (beta <= alpha) {
                    break;  // Beta cut-off
                }
            }
            return maxScore;
        } else {
            int minScore = numeric_limits<int>::max();
            for (int col : validMoves) {
                Board tempBoard = board.getCopy();
                tempBoard.dropPiece(col, playerPiece);
                int score = minimax(tempBoard, depth - 1, alpha, beta, true);
                minScore = min(minScore, score);
                beta = min(beta, score);
                if (beta <= alpha) {
                    break;  // Alpha cut-off
                }
            }
            return minScore;
        }
    }
};

// Training challenge class
class TrainingChallenge {
private:
    Board initialBoard;
    string description;
    int difficulty;
    vector<int> optimalMoves;

public:
    TrainingChallenge(const string& desc, int diff) :
        description(desc), difficulty(diff) {
    }

    void setupBoard(const vector<pair<int, int>>& playerPositions,
                   const vector<pair<int, int>>& computerPositions) {
        initialBoard.reset();

        for (const auto& pos : playerPositions) {
            initialBoard.setPiece(pos.first, pos.second, PLAYER_PIECE);
        }

        for (const auto& pos : computerPositions) {
            initialBoard.setPiece(pos.first, pos.second, COMPUTER_PIECE);
        }
    }

       void setOptimalMoves(const vector<int>& moves) {
        optimalMoves = moves;
    }

    const Board& getBoard() const {
        return initialBoard;
    }

    const string& getDescription() const {
        return description;
    }

    int getDifficulty() const {
        return difficulty;
    }

    const vector<int>& getOptimalMoves() const {
        return optimalMoves;
    }

    int getFirstOptimalMove() const {
        return optimalMoves.empty() ? -1 : optimalMoves[0];
    }
};

// Function declarations for the missing functions
void clearScreen();
int getMenuChoice(int min, int max);

// Game class to manage the game flow
class Game {
private:
    Board board;
    ComputerPlayer computer;
    vector<TrainingChallenge> challenges;
    int currentChallenge;
    int turnsPlayed;
    int playerWins;
    int computerWins;
    bool trainingMode;

    // Generate a random valid board configuration
    Board generateRandomBoard() {
        Board randomBoard;
        random_device rd;
        mt19937 rng(rd());

        // Decide how many pieces to place (between 5 and 20)
        uniform_int_distribution<int> pieceDist(5, 20);
        int totalPieces = pieceDist(rng);

        // Place pieces in a valid way (pieces must be supported)
        for (int i = 0; i < totalPieces; i++) {
            uniform_int_distribution<int> colDist(0, COLS - 1);
            int col = colDist(rng);

            // Find the lowest empty row in the column
            int row = ROWS - 1;
            while (row >= 0 && randomBoard.getPiece(row, col) != EMPTY) {
                row--;
            }

            if (row >= 0) {
                // Alternate between player and Computer pieces
                char piece = (i % 2 == 0) ? PLAYER_PIECE : COMPUTER_PIECE;
                randomBoard.setPiece(row, col, piece);
            }
        }

        // Ensure the board is not in a winning state already
        if (randomBoard.checkWin(PLAYER_PIECE) || randomBoard.checkWin(COMPUTER_PIECE)) {
            // If it's already a win, generate a new one
            return generateRandomBoard();
        }

        return randomBoard;
    }

    // Find the best move for the current player on the given board
    vector<int> findOptimalMoves(const Board& board, char currentPlayer) {
        vector<int> optimalMoves;
        vector<int> validMoves = board.getValidMoves();

        if (validMoves.empty()) {
            return optimalMoves;
        }

        // First, check for immediate wins
        for (int col : validMoves) {
            Board tempBoard = board.getCopy();
            tempBoard.dropPiece(col, currentPlayer);
            if (tempBoard.checkWin(currentPlayer)) {
                optimalMoves.push_back(col);
                return optimalMoves; // Winning move found
            }
        }

        // If no immediate win, check for blocking opponent's win
        char opponent = (currentPlayer == PLAYER_PIECE) ? COMPUTER_PIECE : PLAYER_PIECE;
        for (int col : validMoves) {
            Board tempBoard = board.getCopy();
            tempBoard.dropPiece(col, opponent);
            if (tempBoard.checkWin(opponent)) {
                optimalMoves.push_back(col);
                // Continue checking for other blocking moves
            }
        }
