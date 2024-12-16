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
        }
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

        if (!optimalMoves.empty()) {
            return optimalMoves; // Blocking moves found
        }

        // If no immediate win or block, use minimax to find best move
        ComputerPlayer tempComputer(COMPUTER_PIECE);
        tempComputer.setDifficulty(Difficulty::ADVANCED);

        // Create a temporary board with the current player as Computer
        Board tempBoard = board.getCopy();
        int bestMove = -1;

        if (currentPlayer == PLAYER_PIECE) {
            // If current player is the player, we need to swap pieces for the Computer calculation
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    if (tempBoard.getPiece(row, col) == PLAYER_PIECE) {
                        tempBoard.setPiece(row, col, COMPUTER_PIECE);
                    } else if (tempBoard.getPiece(row, col) == COMPUTER_PIECE) {
                        tempBoard.setPiece(row, col, PLAYER_PIECE);
                    }
                }
            }
            bestMove = tempComputer.minimaxMove(tempBoard, 4);
        } else {
            // If current player is already the Computer, just use the Computer's move
            bestMove = tempComputer.minimaxMove(board, 4);
        }

        if (bestMove != -1) {
            optimalMoves.push_back(bestMove);
        } else {
            // If no strategic move found, add center or any valid move
            int centerCol = COLS / 2;
            if (find(validMoves.begin(), validMoves.end(), centerCol) != validMoves.end()) {
                optimalMoves.push_back(centerCol);
            } else if (!validMoves.empty()) {
                optimalMoves.push_back(validMoves[0]);
            }
        }

        return optimalMoves;
    }

    // Play the random challenge mode (Level 6)
    void playRandomChallengeMode() {
        clearScreen();
        cout << "\n==============================================\n";
        cout << "           RANDOM CHALLENGE MODE              \n";
        cout << "==============================================\n\n";
        cout << "This mode generates random board configurations.\n";
        cout << "Try to find the best move in each position!\n\n";

        // Let player choose their piece
        choosePiece();

        cout << "You are playing as '" << PLAYER_PIECE << "' and the Computer is '" << COMPUTER_PIECE << "'\n\n";

        // Generate initial random board
        board = generateRandomBoard();
        vector<int> optimalMoves = findOptimalMoves(board, PLAYER_PIECE);

        while (true) {
            board.print();

            cout << "Options:\n";
            cout << "1-7: Make a move\n";
            cout << "8: Get a hint\n";
            cout << "9: Show solution\n";
            cout << "0: Return to challenges menu\n";
            cout << "N: Generate a new random board\n\n";

            cout << "Enter your choice: ";
            string input;
            getline(cin, input);

            if (input.empty()) {
                continue;
            }

            // Check for 'N' or 'n' to generate a new board
            if (input[0] == 'N' || input[0] == 'n') {
                board = generateRandomBoard();
                optimalMoves = findOptimalMoves(board, PLAYER_PIECE);
                clearScreen();
                cout << "\nNew random board generated!\n\n";
                continue;
            }

            // Convert other inputs to integer
            int choice;
            try {
                choice = stoi(input);
            } catch (...) {
                cout << "Invalid input. Please try again.\n";
                continue;
            }

            if (choice == 0) {
                return;
            } else if (choice == 8) {
                // Provide a hint
                if (!optimalMoves.empty()) {
                    cout << "\nHINT: Consider column " << optimalMoves[0] + 1 << "\n\n";
                } else {
                    cout << "\nNo good moves available. The position might be a draw.\n\n";
                }
            } else if (choice == 9) {
                // Show solution
                cout << "\nSOLUTION:\n";
                if (!optimalMoves.empty()) {
                    for (size_t i = 0; i < optimalMoves.size(); i++) {
                        cout << "Best move " << i + 1 << ": Column " << optimalMoves[i] + 1 << "\n";
                    }
                } else {
                    cout << "No clearly optimal moves in this position.\n";
                }
                cout << "\nPress Enter to continue...";
                cin.get();
            } else if (choice >= 1 && choice <= 7) {
                // Make a move
                int col = choice - 1;

                if (board.isColumnFull(col)) {
                    cout << "\nColumn " << col + 1 << " is full. Choose another column.\n\n";
                    continue;
                }

                board.dropPiece(col, PLAYER_PIECE);
                board.print();

                if (board.checkWin(PLAYER_PIECE)) {
                    cout << "\nCongratulations! You've won!\n";
                    cout << "Press Enter to generate a new board...";
                    cin.get();
                    board = generateRandomBoard();
                    optimalMoves = findOptimalMoves(board, PLAYER_PIECE);
                    continue;
                }

                // Check if the move was optimal
                bool wasOptimal = false;
                for (int optimalCol : optimalMoves) {
                    if (col == optimalCol) {
                        wasOptimal = true;
                        break;
                    }
                }

                if (wasOptimal) {
                    cout << "\nGood move! That's one of the optimal plays.\n";
                } else {
                    cout << "\nThat's not the optimal move. ";
                    if (!optimalMoves.empty()) {
                        cout << "The best move was column " << optimalMoves[0] + 1 << ".\n";
                    }
                }

                // Computer makes a counter move
                ComputerPlayer tempComputer(COMPUTER_PIECE);
                tempComputer.setDifficulty(Difficulty::ADVANCED);
                int computerCol = tempComputer.makeMove(board);
                board.dropPiece(computerCol, COMPUTER_PIECE);
                cout << "Computer drops a piece in column " << computerCol + 1 << endl;

                board.print();

                if (board.checkWin(COMPUTER_PIECE)) {
                    cout << "\nThe Computer wins. Try again!\n";
                    cout << "Press Enter to generate a new board...";
                    cin.get();
                    board = generateRandomBoard();
                    optimalMoves = findOptimalMoves(board, PLAYER_PIECE);
                    continue;
                }

                if (board.isBoardFull()) {
                    cout << "\nThe game is a draw!\n";
                    cout << "Press Enter to generate a new board...";
                    cin.get();
                    board = generateRandomBoard();
                    optimalMoves = findOptimalMoves(board, PLAYER_PIECE);
                    continue;
                }

                // Update optimal moves for the new board state
                optimalMoves = findOptimalMoves(board, PLAYER_PIECE);
            }
        }
    }

public:
    Game() : computer(COMPUTER_PIECE), currentChallenge(0), turnsPlayed(0),
             playerWins(0), computerWins(0), trainingMode(false) {
        setupTrainingChallenges();
    }

    void setupTrainingChallenges() {
        // Challenge 1: Easy win in one move
        TrainingChallenge challenge1("Find the winning move", 1);
        challenge1.setupBoard(
            {{5, 0}, {5, 1}, {5, 2}},  // Player pieces
            {{5, 3}, {5, 4}, {4, 0}}   // Computer pieces
        );
        challenge1.setOptimalMoves({3}); // Column 4 is the winning move
        challenges.push_back(challenge1);

        // Challenge 2: Block opponent's winning move
        TrainingChallenge challenge2("Block the opponent's winning move", 2);
        challenge2.setupBoard(
            {{5, 1}, {5, 2}, {5, 3}},  // Player pieces
            {{5, 0}, {4, 1}, {4, 3}}   // Computer pieces
        );
        challenge2.setOptimalMoves({0}); // Column 1 blocks opponent
        challenges.push_back(challenge2);

        // Challenge 3: Setup for a win in two moves
        TrainingChallenge challenge3("Setup a winning position", 3);
        challenge3.setupBoard(
            {{5, 1}, {5, 3}, {4, 3}},  // Player pieces
            {{5, 2}, {5, 4}, {4, 1}}   // Computer pieces
        );
        challenge3.setOptimalMoves({5, 0}); // Columns 6, then 1
        challenges.push_back(challenge3);

        // Challenge 4: Complex position with multiple threats
        TrainingChallenge challenge4("Handle multiple threats", 4);
        challenge4.setupBoard(
            {{5, 0}, {5, 2}, {4, 2}, {5, 4}, {4, 4}},  // Player pieces
            {{5, 1}, {5, 3}, {4, 1}, {4, 3}, {3, 2}}   // Computer pieces
        );
        challenge4.setOptimalMoves({6, 5}); // Create double threat
        challenges.push_back(challenge4);

        // Challenge 5: Expert level setup
        TrainingChallenge challenge5("Expert challenge: Find the winning sequence", 5);
        challenge5.setupBoard(
            {{5, 0}, {5, 2}, {5, 4}, {4, 1}, {4, 3}, {3, 2}},  // Player pieces
            {{5, 1}, {5, 3}, {5, 5}, {4, 2}, {4, 4}, {3, 3}}   // Computer pieces
        );
        challenge5.setOptimalMoves({6, 0, 6}); // Complex winning sequence
        challenges.push_back(challenge5);
    }

    void start() {
        while (true) {
            displayMainMenu();
            int choice = getMenuChoice(1, 3);

            switch (choice) {
                case 1:
                    trainingMode = false;
                    playGame();
                    break;
                case 2:
                    trainingMode = true;
                    playTrainingMode();
                    break;
                case 3:
                    cout << "\nThanks for playing Connect 4! Goodbye.\n";
                    return;
            }
        }
    }

private:
    void displayMainMenu() {
        clearScreen();
        cout << "\n==============================================\n";
        cout << "              CleverConnect                   \n";
        cout << "==============================================\n\n";
        cout << "1. Player Mode\n";
        cout << "2. Training Mode\n";
        cout << "3. Exit\n\n";
        cout << "Enter your choice (1-3): ";
    }

    void choosePiece() {
        cout << "Choose your piece:\n";
        cout << "1. X\n";
        cout << "2. O\n";
        cout << "Enter your choice (1-2): ";

        int choice = getMenuChoice(1, 2);

        if (choice == 1) {
            PLAYER_PIECE = 'X';
            COMPUTER_PIECE = 'O';
        } else {
            PLAYER_PIECE = 'O';
            COMPUTER_PIECE = 'X';
        }

        // Update computer's pieces
        computer = ComputerPlayer(COMPUTER_PIECE);
    }