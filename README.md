# Tetris Game Project

This project is a console-based implementation of the classic **Tetris** game, written in C++. It features customizable gameplay, dynamic speed adjustments, and a leaderboard system to track high scores.

## Features

- **Game Modes**:
  - **Quickie Mode**: A casual mode for quick gameplay.
  - **Advanced Mode**: A competitive mode with leaderboard tracking.
- **Dynamic Speed**: The game speed increases as you clear more lines.
- **Customization**:
  - Change colors for Tetrominoes, borders, and text.
  - Adjust game speed with predefined difficulty levels.
- **Leaderboard**:
  - Tracks top scores for Advanced Mode.
  - Allows players to save their scores with usernames.
- **Pause Menu**:
  - Options to restart, resume, customize, or return to the home screen.
- **Game Over Animation**: A visually appealing "Game Over" sequence.

## Controls

- **Arrow Keys**:
  - Left: Move Tetromino left.
  - Right: Move Tetromino right.
  - Down: Move Tetromino down.
  - Up: Rotate Tetromino.
- **W/A/S/D**: Alternative controls for movement and rotation.
- **Spacebar**: Instantly drop the Tetromino.
- **Enter**: Open the pause menu.
- **ESC**: Open the pause menu or exit the game.

## Customization Options

- Change colors for:
  - Tetrominoes
  - Borders
  - Settled blocks
  - Text in menus and score displays
- Adjust game speed with difficulty levels:
  - GrandMaster, Expert, Advanced, Hard, Normal, NOOB
- Reset all settings to default.

## How to Run

1. **Prerequisites**:
   - A Windows system with a C++ compiler (e.g., MinGW or Visual Studio).
   - Console support for ANSI escape codes (for color rendering).

2. **Compile the Code**:
   Use a C++ compiler to compile the `test.cpp` file. For example:
   ```bash
   g++ -o tetris test.cpp
   ```

3. **Run the Game**:
   Execute the compiled binary:
   ```bash
   ./tetris
   ```

4. **Gameplay**:
   - Select a mode from the home screen.
   - Use the controls to play the game.
   - Customize settings or view the leaderboard as needed.

## File Structure

- **test.cpp**: The main source code for the game.
- **leaderboard.txt**: Stores the leaderboard data.
- **highscore.txt**: Stores personal high scores.

## Known Issues

- The game is designed for Windows and may not work on other operating systems due to dependencies like `windows.h` and `conio.h`.
- Console resizing may affect the display layout.

## Future Enhancements

- Add support for multiplayer mode.
- Implement sound effects and background music.
- Port the game to other platforms (e.g., Linux, macOS).

## License

This project is for educational purposes and is not intended for commercial use.

## Acknowledgments

- ASCII art and color effects inspired by various online resources.
- Classic Tetris gameplay mechanics.

Enjoy the game!