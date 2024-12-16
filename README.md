# CleverConnect

CleverConnect is a terminal-based Connect 4 game written in C++. You play against a computer opponent that actually gets smarter as the match goes on. It also has a Training Mode where you can practice specific board positions and learn better strategies.

The game is played on a standard 6x7 grid. The goal, like classic Connect 4, is to get four of your pieces in a row before the computer does. Either horizontally, vertically, or diagonally.

***

## Features

- **Player vs Computer mode** with an adaptive AI that starts easy and gets harder as you play
- **4 difficulty levels**: Beginner, Intermediate, Advanced, and Expert
- **Training Mode** with 5 hand-crafted puzzle challenges plus a random board generator
- **Hint system** that suggests the best move when you are stuck
- **Solution viewer** that shows the optimal move sequence for any puzzle
- **Score tracking** across rounds in the same session
- **Choose your piece**: play as X or O

***

## How the AI works

The computer uses the **Minimax algorithm with Alpha-Beta pruning** to decide its moves. In simple terms, it looks ahead at all possible future moves and picks the one that gives it the best outcome.

- At **Beginner** level, it just looks one move ahead (greedy strategy). It will win if it can, block you if needed, and otherwise prefer the center column.
- At **Intermediate** level, it searches 3 moves deep.
- At **Advanced** level, it searches 5 moves deep.
- At **Expert** level, it searches up to 8 moves deep, which means it is planning quite far ahead.

In Player Mode, the AI starts at Beginner and automatically upgrades every 6 turns, so the game gets harder naturally as it goes on.

The board evaluation gives bonus points for controlling the center column, having 2 or 3 pieces lined up, and blocking the opponent from getting 3 in a row.

***

## Game Modes

### Player Mode

You play a full game against the computer from an empty board. The AI difficulty increases every 6 turns. Your wins and the computer's wins are tracked across games in the same session.

### Training Mode

There are 5 preset challenges and one random challenge mode.

| Challenge | Difficulty | Description |
|-----------|------------|-------------|
| Level 1 | Easy | Find the winning move |
| Level 2 | Easy-Medium | Block the opponent's winning move |
| Level 3 | Medium | Set up a win in two moves |
| Level 4 | Hard | Handle multiple threats at once |
| Level 5 | Expert | Find the winning sequence in a complex position |
| Level 6 | Random | Randomly generated board, solved by the AI |

In every challenge, you can:
- Type `8` to get a hint (tells you which column to consider)
- Type `9` to see the full solution
- Type `0` to go back to the menu

***

## How to Build and Run

### Requirements

- A C++ compiler that supports C++11 or later (like g++ or clang++)

### Compile

```bash
g++ -std=c++11 -o cleverconnect main.cpp
```

### Run

```bash
./cleverconnect
```

On Windows:
```bash
cleverconnect.exe
```

***

## How to Play

1. Run the program. You will see the main menu with two options: Player Mode and Training Mode.
2. In **Player Mode**, choose your piece (X or O) and start dropping pieces by entering a column number (1 through 7).
3. In **Training Mode**, pick a challenge level. The board will be set up for you with pieces already placed. Your job is to find the best next move.
4. At any point during your turn, type `0` to open the in-game menu where you can reset or go back to the main menu.

***

## Project Structure

The entire game is in a single `.cpp` file with four main classes:

| Class | What it does |
|-------|--------------|
| `Board` | Stores the 6x7 grid, handles piece drops, win checks, and board evaluation |
| `ComputerPlayer` | Runs the Minimax algorithm and decides the AI's move based on difficulty |
| `TrainingChallenge` | Stores a preset board position, its description, and the optimal moves |
| `Game` | Controls the game loop, menus, Training Mode, and score tracking |

***

## Notes

- The computer shows a "thinking..." animation before every move to make it feel more natural.
- After each move in Training Mode, the game tells you whether your move was optimal or not.
- In the Random Challenge mode (Level 6), you can press `N` at any time to generate a new random board.
- The `clearScreen()` function works on both Windows (`cls`) and Unix/Linux/Mac (`clear`).

***

## License

MIT