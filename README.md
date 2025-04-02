# 🎮 Tetris Game in C++ (IT206 Project 2)

Welcome to the **Tetris Game**, a console-based implementation of the classic Tetris game developed in **C++**. This project is part of the IT206 Data Structures Lab and showcases advanced programming concepts, including dynamic memory, file handling, and real-time user interaction.

---

## 📋 Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Game Modes](#game-modes)
4. [Controls](#controls)
5. [Customization Options](#customization-options)
6. [How to Play](#how-to-play)
7. [Technical Details](#technical-details)
8. [File Structure](#file-structure)
9. [Future Enhancements](#future-enhancements)
10. [Contributors](#contributors)
11. [License](#license)

---

## 📝 Overview

This project is a modern take on the classic **Tetris** game, designed for the console environment. It features dynamic gameplay, customizable settings, and a leaderboard system to track high scores. The game is built using **C++** and demonstrates the use of data structures like vectors, queues, and maps.

- **Platform**: Console-based (Windows)
- **Language**: C++
- **Purpose**: Educational project for IT206 Data Structures Lab

---

## ✨ Features

- **Dynamic Gameplay**: Speed increases as you clear more lines.
- **Game Modes**:
  - **Quickie Mode**: Casual gameplay without leaderboard tracking.
  - **Advanced Mode**: Competitive gameplay with leaderboard tracking.
- **Leaderboard**: Tracks top scores and allows players to save their names.
- **Customization**: Change colors, adjust speed, and reset settings.
- **Pause and Resume**: Pause the game anytime and resume seamlessly.
- **Game Over Animation**: A visually appealing "Game Over" sequence.

---

## 🎮 Game Modes

1. **Quickie Mode**:
   - A casual mode for quick gameplay.
   - Scores are not saved to the leaderboard.

2. **Advanced Mode**:
   - A competitive mode with leaderboard tracking.
   - Players can save their scores with usernames.

---

## 🎛️ Controls

| **Key**       | **Action**                     |
|---------------|--------------------------------|
| **Arrow Keys**| Move left, right, down, or rotate Tetromino. |
| **W/A/S/D**   | Alternative controls for movement and rotation. |
| **Spacebar**  | Instantly drop the Tetromino.  |
| **Enter**     | Open the pause menu.           |
| **ESC**       | Pause or exit the game.        |

---

## 🎨 Customization Options

- **Color Customization**:
  - Change colors for Tetrominoes, borders, settled blocks, and text.
- **Speed Adjustment**:
  - Choose from predefined difficulty levels:
    - GrandMaster, Expert, Advanced, Hard, Normal, NOOB
  - Reset speed to default settings.
- **Reset to Default**:
  - Restore all settings to their default values.

---

## 🕹️ How to Play

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/YourUsername/IT206_PROJECT_2.git
   ```

2. **Navigate to the Directory**:
   ```bash
   cd IT206_PROJECT_2
   ```

3. **Compile the Code**:
   ```bash
   g++ -o tetris Others/test.cpp
   ```

4. **Run the Game**:
   ```bash
   ./tetris
   ```

5. **Gameplay**:
   - Select a mode from the home screen.
   - Use the controls to play the game.
   - Customize settings or view the leaderboard as needed.

---

## ⚙️ Technical Details

- **Language**: C++
- **Platform**: Console-based (Windows)
- **Libraries Used**:
  - `<vector>`: For dynamic arrays.
  - `<fstream>`: For file handling.
  - `<windows.h>`: For console manipulation.
  - `<conio.h>`: For real-time keyboard input.

---

## 📁 File Structure

```
📦 IT206_PROJECT_2
 ├── 📂 Others
 │    └── test.cpp       → Main game logic
 ├── leaderboard.txt     → Stores leaderboard data
 ├── highscore.txt       → Stores personal high scores
 └── README.md           → Project documentation
```

---

## 🚀 Future Enhancements

- Add support for multiplayer mode.
- Implement sound effects and background music.
- Port the game to other platforms (e.g., Linux, macOS).
- Add more Tetromino shapes and advanced gameplay mechanics.

---

## 🤝 Contributors

- 👨‍💻 **Dev Parmar**  
- 👨‍💻 **Krrish Kamani**  
- 👨‍💻 **Jaymit**  
- 👨‍💻 **Yuvraj**

---

## 📜 License

This project is for educational purposes and is not intended for commercial use.

---

🎉 Enjoy playing Tetris and feel free to contribute or suggest improvements!