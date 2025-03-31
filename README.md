# Tetris Game Project

Welcome to the **Tetris Game Project**, a modernized and customizable version of the classic Tetris game. This project is built using C++ and leverages the Windows Console for rendering and user interaction.

---

## 📖 Overview

This project is a console-based Tetris game designed to provide an engaging and customizable gaming experience. It includes features like dynamic speed adjustment, leaderboard tracking, and various customization options for colors and gameplay modes.

---

## ✨ Features

- **Dynamic Gameplay**: Speed increases as you clear more lines, making the game progressively challenging.
- **Two Game Modes**:
  - **Quickie Mode**: A casual mode for quick gameplay.
  - **Advanced Mode**: A competitive mode with leaderboard tracking.
- **Leaderboard**: Tracks the top 10 scores for Advanced Mode.
- **Customization**:
  - Change Tetromino colors.
  - Customize frame, text, and score display colors.
  - Adjust game speed to match your skill level.
- **Pause Menu**: Pause the game anytime and access options like restart, resume, or customization.
- **Game Over Animation**: A visually appealing "Game Over" sequence.

---

## 🛠️ Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/IT206_PROJECT_2.git
   cd IT206_PROJECT_2
   ```

2. **Build the Project**:
   - Open the project in your preferred C++ IDE (e.g., Visual Studio).
   - Ensure the project is set to compile for Windows Console Applications.
   - Build and run the project.

3. **Run the Executable**:
   - Navigate to the output directory (e.g., `Debug` or `Release`).
   - Run the `Dts.exe` file.

---

## 🚀 Usage

1. **Start the Game**:
   - Launch the executable.
   - Select a game mode from the home menu:
     - `(Q)` Quickie Mode
     - `(A)` Advanced Mode
     - `(L)` Leaderboard
     - `(C)` Customize
     - `(E)` Exit

2. **Controls**:
   - **Arrow Keys**: Move the Tetromino left, right, or down.
   - **Up Arrow**: Rotate the Tetromino.
   - **Spacebar**: Drop the Tetromino instantly.
   - **ESC/Enter**: Open the pause menu.

3. **Pause Menu Options**:
   - `(R)` Restart the game.
   - `(Enter)` Resume the game.
   - `(C)` Customize game settings.
   - `(ESC/B)` Return to the home menu.

4. **Customization**:
   - Access the customization menu to change colors and adjust game speed.

---

## 🎨 Customization

The game offers extensive customization options:

- **Tetromino Colors**: Change the colors of individual Tetromino shapes.
- **Frame Color**: Customize the border color of the game grid.
- **Text Colors**: Modify the colors of menu text, pause menu text, and score display.
- **Game Speed**: Choose from predefined speed levels (e.g., GrandMaster, Expert, Normal, NOOB).

To access these options, select `(C)` Customize from the home menu or pause menu.

---

## 📊 Leaderboard

The leaderboard tracks the top 10 scores in Advanced Mode. Scores are saved in a `leaderboard.txt` file and are sorted in descending order. Players can enter their username to save their scores.

---

## 🧑‍💻 Contribution

We welcome contributions to enhance the game! Here's how you can contribute:

1. **Fork the Repository**: Click the "Fork" button on GitHub.
2. **Clone Your Fork**:
   ```bash
   git clone https://github.com/your-username/IT206_PROJECT_2.git
   ```
3. **Create a Branch**:
   ```bash
   git checkout -b feature-name
   ```
4. **Make Changes**: Implement your feature or fix.
5. **Commit Changes**:
   ```bash
   git commit -m "Add feature-name"
   ```
6. **Push Changes**:
   ```bash
   git push origin feature-name
   ```
7. **Submit a Pull Request**: Open a pull request on the original repository.

---

## 📂 File Structure

```
IT206_PROJECT_2/
├── Others/
│   ├── Dts.cpp         # Main game logic and rendering
│   ├── leaderboard.txt # Stores leaderboard data
├── README.md           # Project documentation
└── highscore.txt       # Stores high scores for Advanced Mode
```

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 📧 Contact

For questions or feedback, feel free to reach out:

- **Email**: your-email@example.com
- **GitHub**: [your-username](https://github.com/your-username)

---

Enjoy the game and happy coding! 🎮
