# Tetris Game Project

This project is a console-based implementation of the classic Tetris game, written in C++. It includes various features such as customizable colors, leaderboard tracking, and multiple game modes.

## Features

- **Game Modes**:
  - **Quickie Mode**: A fast-paced version of the game.
  - **Advanced Mode**: Includes leaderboard tracking and username input.
  
- **Leaderboard**:
  - Tracks the top 10 scores.
  - Allows players to enter their username if their score qualifies.

- **Customization**:
  - Change colors for tetrominoes, frame, fallen blocks, menu text, and score text.
  - Reset all settings to default.

- **Pause Menu**:
  - Options to restart, resume, customize, or return to the home screen.

- **Dynamic Difficulty**:
  - The game increases in difficulty as the player clears more lines.

## How to Play

1. **Controls**:
   - Arrow keys or `W`, `A`, `S`, `D` to move and rotate tetrominoes.
   - Spacebar to drop the tetromino instantly.
   - `ESC` or `Enter` to open the pause menu.

2. **Objective**:
   - Clear as many lines as possible by completing horizontal rows without gaps.
   - Avoid letting the tetrominoes stack to the top of the grid.

3. **Scoring**:
   - Earn points for clearing lines.
   - Bonus points for clearing multiple lines at once.

## Customization Options

- Change the colors of:
  - Tetrominoes (I, T, Z, S, O, L, J shapes).
  - Game frame.
  - Fallen blocks.
  - Menu text.
  - Score text.

- Reset all settings to default.

## Leaderboard

- The leaderboard tracks the top 10 scores in Advanced Mode.
- Players can enter their username if their score qualifies.
- The leaderboard is saved in a file named `leaderboard.txt`.

## Files

- **`TTs.cpp`**: The main source code for the game.
- **`leaderboard.txt`**: Stores the leaderboard data.

## How to Run

1. Compile the program using a C++ compiler that supports C++11 or later.
   ```
   g++ -o Tetris TTs.cpp
   ```
2. Run the compiled executable.
   ```
   ./Tetris
   ```

## Requirements

- Windows OS (uses `conio.h` and `windows.h` for console manipulation).
- A C++ compiler (e.g., GCC, MSVC).

## Known Issues

- The game is designed for Windows and may not work on other operating systems due to platform-specific libraries.
- Ensure the console window is large enough to display the game properly.

## Future Enhancements

- Add support for multiplayer mode.
- Implement sound effects and background music.
- Port the game to other platforms.

## License

This project is for educational purposes and is not intended for commercial use.
