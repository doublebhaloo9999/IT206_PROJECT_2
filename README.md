# Tetris Game Project

## Overview
This project is a console-based **Tetris Game** implemented in C++ with customizable features, leaderboard functionality, and two game modes: **Quickie Mode** and **Advanced Mode**. The game includes features such as tetromino rotation, line clearing, scoring, and a leaderboard to track high scores.

---

## Features
### Gameplay
- **Tetromino Movement and Rotation**: Move tetrominoes left, right, down, or rotate them.
- **Line Clearing**: Clear lines to earn points and progress to higher levels.
- **Game Over Detection**: The game ends when tetrominoes stack up to the top of the grid.

### Game Modes
1. **Quickie Mode**: Play without entering a username. Scores are not saved unless they qualify for the leaderboard.
2. **Advanced Mode**: Enter a username and compete for the global high score.

### Customization
- Change colors for:
  - Tetrominoes
  - Frame
  - Fallen blocks
  - Home window text
  - Pause menu text
  - Score text
- Reset all settings to default.

### Leaderboard
- Tracks the top 10 scores.
- Separate leaderboard for Quickie Mode and Advanced Mode.
- Saves leaderboard data to a file (`leaderboard.txt`).

### Graphics
- ASCII-based rendering of the game grid.
- Colorful tetrominoes and UI elements.
- Animated "Game Over" screen.

---

## How to Play
### Controls
- **Arrow Keys**:
  - Left: Move tetromino left.
  - Right: Move tetromino right.
  - Down: Move tetromino down faster.
  - Up: Rotate tetromino.
- **Spacebar**: Drop tetromino to the bottom instantly.
- **ESC/Enter**: Open the pause menu.
- **W/A/S/D**: Alternative controls for movement and rotation.

### Pause Menu Options
- **R**: Restart the game.
- **Enter**: Resume the game.
- **M**: Open the customization menu.
- **ESC/B**: Return to the home window.

---

## Installation and Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/IT206_PROJECT_2.git
