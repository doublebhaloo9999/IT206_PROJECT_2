#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> // For Sleep()
#include <chrono> // For timing
#include <string>
#include <fstream>

using namespace std;
using namespace std::chrono;

const int width = 10;
const int height = 20;
vector<vector<int>> grid(height, vector<int>(width, 0));
int score = 0;
int level = 1;
int linesCleared = 0;
string username;
int highScore = 0;

// Tetromino shapes and colors
vector<vector<vector<int>>> tetrominos = {
    {{1, 1, 1, 1}}, // I
    {{1, 1, 1}, {0, 1, 0}}, // T
    {{1, 1, 0}, {0, 1, 1}}, // Z
    {{1, 1, 1, 1}}, // I
    {{1, 1, 1}, {0, 1, 0}}, // T
    {{1, 1, 0}, {0, 1, 1}}, // Z
    {{0, 1, 1}, {1, 1, 0}}, // S
    {{1, 1}, {1, 1}}, // O
    {{1, 1, 1}, {1, 0, 0}}, // L
    {{1, 1, 1}, {0, 0, 1}} // J
};

struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
};

Tetromino currentTetromino;

// Cross-platform _kbhit() and _getch() alternatives
bool kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

char getch() {
    termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Get terminal size dynamically
pair<int, int> getTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
}

// Clear screen
void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen and move cursor to top-left
}

// Set text color
void setTextColor(const string& color) {
    if (color == "red") {
        cout << "\033[31m";
    } else if (color == "green") {
        cout << "\033[32m";
    } else if (color == "blue") {
        cout << "\033[34m";
    } else if (color == "reset") {
        cout << "\033[0m";
    }
}

void resetGameState() {
    grid = vector<vector<int>>(height, vector<int>(width, 0));
    score = 0;
    level = 1;
    linesCleared = 0;
}

void initialize() {
    int index = rand() % tetrominos.size();
    currentTetromino.shape = tetrominos[index];
    currentTetromino.x = width / 2 - currentTetromino.shape[0].size() / 2;
    currentTetromino.y = 0;
}

bool checkCollision(int newX, int newY, vector<vector<int>> newShape) {
    for (int i = 0; i < newShape.size(); ++i) {
        for (int j = 0; j < newShape[i].size(); ++j) {
            if (newShape[i][j]) {
                int gridX = newX + j;
                int gridY = newY + i;
                if (gridX < 0 || gridX >= width || gridY >= height || (gridY >= 0 && grid[gridY][gridX])) {
                    return true;
                }
            }
        }
    }
    return false;
}

void placeTetromino() {
    for (int i = 0; i < currentTetromino.shape.size(); ++i) {
        for (int j = 0; j < currentTetromino.shape[i].size(); ++j) {
            if (currentTetromino.shape[i][j]) {
                grid[currentTetromino.y + i][currentTetromino.x + j] = 1;
            }
        }
    }
}

void clearLines() {
    for (int i = height - 1; i >= 0; --i) {
        bool fullLine = true;
        for (int j = 0; j < width; ++j) {
            if (!grid[i][j]) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), vector<int>(width, 0));
            score += 100;
            linesCleared++;
            if (linesCleared % 10 == 0) {
                level++;
            }
            i++; // Recheck the same row after shifting
        }
    }
}

void rotateTetromino() {
    vector<vector<int>> newShape(currentTetromino.shape[0].size(), vector<int>(currentTetromino.shape.size()));
    for (int i = 0; i < currentTetromino.shape.size(); ++i) {
        for (int j = 0; j < currentTetromino.shape[i].size(); ++j) {
            newShape[j][currentTetromino.shape.size() - 1 - i] = currentTetromino.shape[i][j];
        }
    }
    if (!checkCollision(currentTetromino.x, currentTetromino.y, newShape)) {
        currentTetromino.shape = newShape;
    }
}

void draw() {
    clearScreen(); // Clear the terminal before drawing

    // Draw top border
    for (int i = 0; i < width + 2; ++i) {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; ++i) {
        cout << "#"; // Left border
        for (int j = 0; j < width; ++j) {
            if (grid[i][j]) {
                cout << "1";
            } else {
                bool isTetromino = false;
                for (int k = 0; k < currentTetromino.shape.size(); ++k) {
                    for (int l = 0; l < currentTetromino.shape[k].size(); ++l) {
                        if (currentTetromino.shape[k][l] && currentTetromino.y + k == i && currentTetromino.x + l == j) {
                            cout << "1";
                            isTetromino = true;
                            break;
                        }
                    }
                    if (isTetromino) break;
                }
                if (!isTetromino) {
                    cout << " ";
                }
            }
        }
        cout << "#"; // Right border
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; ++i) {
        cout << "#";
    }
    cout << endl;

    // Display score only
    cout << "Score: " << score << endl;
}

void saveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << username << " " << highScore << endl;
        file.close();
    }
}

void loadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> username >> highScore;
        file.close();
    }
}

void gameLoop() {
    clearScreen(); // Clear the terminal before starting the game

    auto lastFrameTime = high_resolution_clock::now();
    auto lastFallTime = high_resolution_clock::now();
    int fallInterval = 500 - (level - 1) * 50; // Initial fall interval

    bool isGameOver = false;

    while (!isGameOver) {
        auto currentTime = high_resolution_clock::now();
        auto frameDuration = duration_cast<milliseconds>(currentTime - lastFrameTime);
        auto fallDuration = duration_cast<milliseconds>(currentTime - lastFallTime);

        if (frameDuration.count() >= 33) { // 30 FPS -> 1000ms / 30 = ~33ms per frame
            lastFrameTime = currentTime;

            if (kbhit()) {
                char ch = getch();
                switch (ch) {
                    case 'a': // Move left
                        if (!checkCollision(currentTetromino.x - 1, currentTetromino.y, currentTetromino.shape)) {
                            currentTetromino.x--;
                        }
                        break;
                    case 'd': // Move right
                        if (!checkCollision(currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape)) {
                            currentTetromino.x++;
                        }
                        break;
                    case 's': // Move down
                        if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                            currentTetromino.y++;
                        }
                        break;
                    case 'w': // Rotate
                        rotateTetromino();
                        break;
                    case ' ': // Drop
                        while (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                            currentTetromino.y++;
                        }
                        break;
                    case 27: // ESC key
                        clearScreen();
                        cout << "Game Paused. Press any key to resume.\n";
                        getch();
                        break;
                }
            }

            if (fallDuration.count() >= fallInterval) {
                lastFallTime = currentTime;
                if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                    currentTetromino.y++;
                } else {
                    placeTetromino();
                    clearLines();
                    int index = rand() % tetrominos.size();
                    currentTetromino.shape = tetrominos[index];
                    currentTetromino.x = width / 2 - currentTetromino.shape[0].size() / 2;
                    currentTetromino.y = 0;
                    if (checkCollision(currentTetromino.x, currentTetromino.y, currentTetromino.shape)) {
                        isGameOver = true;
                    }
                }
            }

            // Draw the game
            draw();
        }
    }

    // Game Over screen
    clearScreen();
    setTextColor("red");
    cout << "GAME OVER\n";
    setTextColor("reset");
    cout << "Your Score: " << score << "\n";
    if (score > highScore) {
        highScore = score;
        saveHighScore();
        cout << "New High Score: " << highScore << "!\n";
    } else {
        cout << "High Score: " << highScore << "\n";
    }
    cout << "Press any key to exit...\n";
    getch();
}

void displayHomeWindow() {
    clearScreen(); // Clear the console

    auto [consoleWidth, consoleHeight] = getTerminalSize(); // Dynamically get console width
    string title = "WELCOME TO TETRIS!";
    string option1 = "Q - Quickie Mode";
            if (fallDuration.count() >= fallInterval) {
                lastFallTime = currentTime;
                if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                    currentTetromino.y++;
                } else {
                    placeTetromino();
                    clearLines();
                    int index = rand() % tetrominos.size();
                    currentTetromino.shape = tetrominos[index];
                    currentTetromino.x = width / 2 - currentTetromino.shape[0].size() / 2;
                    currentTetromino.y = 0;
                    currentTetromino.color = tetrominoColors[index];
                    if (checkCollision(currentTetromino.x, currentTetromino.y, currentTetromino.shape)) {
                        isGameOver = true;
                    }
                }
            }

            draw(hConsole, bufferSize, buffer);
        }
    }

    // Smooth transition to Game Over screen
    system("cls");

    int consoleWidth = getConsoleWidth(); // Dynamically get console width
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Red color for Game Over

    string gameOverText = "G A M E   O V E R";
    string separator(consoleWidth, '=');
    string scoreText = "Your Score: " + to_string(score);
    string highScoreText = "High Score: " + to_string(highScore);
    string newHighScoreText = "New High Score: " + to_string(highScore) + "!";
    string exitText = "Press any key to exit...";
    int gameOverPadding = (consoleWidth - gameOverText.size()) / 2;
    int scorePadding = (consoleWidth - scoreText.size()) / 2;
    int highScorePadding = (consoleWidth - highScoreText.size()) / 2;
    int exitPadding = (consoleWidth - exitText.size()) / 2;

    cout << "\n\n\n";
    cout << string(gameOverPadding, ' ') << separator << endl;
    cout << string(gameOverPadding, ' ') << gameOverText << endl;
    cout << string(gameOverPadding, ' ') << separator << endl;
    cout << "\n\n";
    cout << string(scorePadding, ' ') << scoreText << endl;

    if (score > highScore) {
        cout << string(scorePadding, ' ') << newHighScoreText << endl;
        highScore = score;
        saveHighScore();
    } else {
        cout << string(highScorePadding, ' ') << highScoreText << endl;
    }

    cout << "\n\n";
    cout << string(exitPadding, ' ') << exitText << endl;
    _getch(); // Wait for user input

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void displayHomeWindow() {
    system("cls"); // Clear the console

    int consoleWidth = getConsoleWidth(); // Dynamically get console width
    string title = "WELCOME TO TETRIS!";
    string option1 = "Q - Quickie Mode";
    string option2 = "A - Advanced Mode";
    string option3 = "S - Show Scoreboard";
    string separator(consoleWidth, '=');

    int titlePadding = (consoleWidth - title.size()) / 2;
    int optionPadding = (consoleWidth - option1.size()) / 2;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color

    cout << "\n\n\n";
    cout << string(titlePadding, ' ') << title << endl; // Center-align title
    cout << separator << endl;
    cout << "\n\n";

    // Center-align options
    cout << string(optionPadding, ' ') << option1 << endl;
    cout << string(optionPadding, ' ') << option2 << endl;
    cout << string(optionPadding, ' ') << option3 << endl;

    cout << "\n\n";
    cout << string(optionPadding, ' ') << "Select your option: ";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void showScoreboard() {
    system("cls"); // Clear the console
    ifstream file("highscore.txt");
    vector<pair<string, int>> scores;

    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            scores.push_back({name, score});
        }
        file.close();
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color

    cout << "\n\n";
    int consoleWidth = 50; // Adjust as needed
    string title = "Scoreboard";
    string separator(consoleWidth, '=');
    int padding = (consoleWidth - title.size()) / 2;

    cout << string(padding, ' ') << separator << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(padding, ' ') << separator << endl;

    if (scores.empty()) {
        cout << "\n";
        string noGamesMessage = "No games have been played in the Advanced Mode.";
        padding = (consoleWidth - noGamesMessage.size()) / 2;
        cout << string(padding, ' ') << noGamesMessage << endl;
    } else {
        cout << "\n";
        for (int i = 0; i < scores.size() && i < 10; ++i) {
            string entry = to_string(i + 1) + ". " + scores[i].first + " - " + to_string(scores[i].second);
            padding = (consoleWidth - entry.size()) / 2;
            cout << string(padding, ' ') << entry << endl;
        }
    }

    cout << "\n";
    cout << string((consoleWidth - 20) / 2, ' ') << "Press any key to return...";
    _getch(); // Wait for user input

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void startGame(bool advancedMode) {
    resetGameState(); // Reset the game state

    if (advancedMode) {
        cout << "Enter your username: ";
        cin >> username;
        loadHighScore();
        cout << "Current High Score: " << highScore << endl;
    }

    initialize();
    gameLoop();

    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        startGame(advancedMode);
    }
}

int main() {
    system("mode con: cols=100 lines=30"); // Set console width and height
    srand(time(0)); // Initialize random seed

    while (true) {
        displayHomeWindow(); // Display the home window

        char mode;
        cin >> mode;

        if (mode == 'a' || mode == 'A') {
            startGame(true);
        } else if (mode == 'q' || mode == 'Q') {
            startGame(false);
        } else if (mode == 's' || mode == 'S') {
            showScoreboard();
        } else {
            break; // Exit the game
        }
    }

    return 0;
}
