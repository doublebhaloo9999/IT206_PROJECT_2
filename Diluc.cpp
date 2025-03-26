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
    {{0, 1, 1}, {1, 1, 0}}, // S
    {{1, 1}, {1, 1}}, // O
    {{1, 1, 1}, {1, 0, 0}}, // L
    {{1, 1, 1}, {0, 0, 1}} // J
};

vector<int> tetrominoColors = {
    FOREGROUND_BLUE | FOREGROUND_INTENSITY, // I
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // T
    FOREGROUND_RED | FOREGROUND_INTENSITY, // Z
    FOREGROUND_GREEN | FOREGROUND_INTENSITY, // S
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // O
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // L
    FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY // J
};

struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
    int color;
};

Tetromino currentTetromino;

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
    currentTetromino.color = tetrominoColors[index];
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

void draw(HANDLE hConsole, COORD bufferSize, CHAR_INFO* buffer) {
    // Clear buffer
    for (int i = 0; i < bufferSize.Y; ++i) {
        for (int j = 0; j < bufferSize.X; ++j) {
            buffer[i * bufferSize.X + j].Char.AsciiChar = ' ';
            buffer[i * bufferSize.X + j].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }

    // Draw top border
    for (int i = 0; i < width + 2; ++i) {
        buffer[i].Char.AsciiChar = '#';
        buffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    for (int i = 0; i < height; ++i) {
        buffer[(i + 1) * bufferSize.X].Char.AsciiChar = '#'; // Left border
        buffer[(i + 1) * bufferSize.X].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        for (int j = 0; j < width; ++j) {
            if (grid[i][j]) {
                buffer[(i + 1) * bufferSize.X + j + 1].Char.AsciiChar = '1';
                buffer[(i + 1) * bufferSize.X + j + 1].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            } else {
                bool isTetromino = false;
                for (int k = 0; k < currentTetromino.shape.size(); ++k) {
                    for (int l = 0; l < currentTetromino.shape[k].size(); ++l) {
                        if (currentTetromino.shape[k][l] && currentTetromino.y + k == i && currentTetromino.x + l == j) {
                            buffer[(i + 1) * bufferSize.X + j + 1].Char.AsciiChar = '1';
                            buffer[(i + 1) * bufferSize.X + j + 1].Attributes = currentTetromino.color;
                            isTetromino = true;
                            break;
                        }
                    }
                    if (isTetromino) break;
                }
            }
        }
        buffer[(i + 1) * bufferSize.X + width + 1].Char.AsciiChar = '#'; // Right border
        buffer[(i + 1) * bufferSize.X + width + 1].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; ++i) {
        buffer[(height + 1) * bufferSize.X + i].Char.AsciiChar = '#';
        buffer[(height + 1) * bufferSize.X + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    // Display score only
    string scoreStr = "Score: " + to_string(score);
    for (int i = 0; i < scoreStr.size(); ++i) {
        buffer[(height + 2) * bufferSize.X + i].Char.AsciiChar = scoreStr[i];
        buffer[(height + 2) * bufferSize.X + i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Teal color
    }

    // Write buffer to console
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(bufferSize.X - 1), static_cast<SHORT>(bufferSize.Y - 1) };
    WriteConsoleOutputA(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
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

// Function to get console width dynamically
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80; // Default width if unable to fetch
}

// Function to get console height dynamically
int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 25; // Default height if unable to fetch
}

void gameLoop() {
    system("cls"); // Clear the terminal before starting the game

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Declare only once
    COORD bufferSize = { width + 2, height + 3 };
    CHAR_INFO buffer[bufferSize.X * bufferSize.Y];

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

            if (_kbhit()) {
                int ch = _getch();
                if (ch == 0 || ch == 224) {
                    switch (_getch()) {
                        case 75: // Left arrow key
                            if (!checkCollision(currentTetromino.x - 1, currentTetromino.y, currentTetromino.shape)) {
                                currentTetromino.x--;
                            }
                            break;
                        case 77: // Right arrow key
                            if (!checkCollision(currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape)) {
                                currentTetromino.x++;
                            }
                            break;
                        case 80: // Down arrow key
                            if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                                currentTetromino.y++;
                            }
                            break;
                        case 72: // Up arrow key
                            rotateTetromino();
                            break;
                    }
                } else {
                    switch (ch) {
                        case 'a':
                            if (!checkCollision(currentTetromino.x - 1, currentTetromino.y, currentTetromino.shape)) {
                                currentTetromino.x--;
                            }
                            break;
                        case 'd':
                            if (!checkCollision(currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape)) {
                                currentTetromino.x++;
                            }
                            break;
                        case 's':
                            if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                                currentTetromino.y++;
                            }
                            break;
                        case 'w':
                            rotateTetromino();
                            break;
                        case ' ':
                            while (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                                currentTetromino.y++;
                            }
                            break;
                        case 27: // ESC key
                            system("cls"); // Clear the screen
                            int consoleWidth = getConsoleWidth(); // Dynamically get console width
                            int consoleHeight = getConsoleHeight(); // Dynamically get console height
                            string pauseText = "  G A M E   P A U S E D  ";
                            string resumeText = "1 - Resume";
                            string restartText = "2 - Restart";
                            string quitText = "3 - Quit";
                            string separator(consoleWidth, '=');

                            HANDLE hConsolePause = GetStdHandle(STD_OUTPUT_HANDLE);
                            SetConsoleTextAttribute(hConsolePause, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green color

                            int pausePadding = (consoleWidth - pauseText.size()) / 2;
                            int resumePadding = (consoleWidth - resumeText.size()) / 2;
                            int restartPadding = (consoleWidth - restartText.size()) / 2;
                            int quitPadding = (consoleWidth - quitText.size()) / 2;

                            for (int i = 0; i < consoleHeight / 2 - 4; ++i) {
                                cout << endl; // Add vertical padding
                            }

                            cout << string(pausePadding, ' ') << pauseText << endl;
                            cout << separator << endl; // Full-width separator
                            cout << endl; // Add spacing
                            cout << string(resumePadding, ' ') << resumeText << endl;
                            cout << string(restartPadding, ' ') << restartText << endl;
                            cout << string(quitPadding, ' ') << quitText << endl;

                            char pauseChoice;
                            cin >> pauseChoice;

                            if (pauseChoice == '1') {
                                system("cls"); // Resume game
                            } else if (pauseChoice == '2') {
                                resetGameState();
                                initialize();
                                system("cls");
                            } else if (pauseChoice == '3') {
                                exit(0); // Quit game
                            }

                            SetConsoleTextAttribute(hConsolePause, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
                            break;
                    }
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
