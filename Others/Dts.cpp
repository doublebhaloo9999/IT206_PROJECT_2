#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> // For Sleep()
#include <chrono> // For timing
#include <string>
#include <fstream>
#include <algorithm> // For sort()

using namespace std;
using namespace std::chrono;

// Constants and global variables
const int width = 10;
const int height = 20;
vector<vector<int>> grid(height, vector<int>(width, 0));
int score = 0;
int level = 1;
int linesCleared = 0;
string username;
int highScore = 0;
int frameColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int fallenBlockColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int menuTextColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
int scoreTextColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

// Tetromino shapes and colors
vector<vector<vector<int>>> tetrominos = {
    {{1, 1, 1, 1}}, // I
    {{1, 1, 1}, {0, 1, 0}}, // T
    {{1, 1, 0}, {0, 1, 1}}, // Z
    {{0, 1, 1}, {1, 1, 0}}, // S
    {{1, 1}, {1, 1}}, // O
    {{1, 1, 1}, {1, 0, 0}}, // L
    {{1, 1, 1}, {0, 0, 1}}, // J
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

// Utility functions
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

// Drawing and rendering functions
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
        buffer[i].Attributes = frameColor;
    }

    for (int i = 0; i < height; ++i) {
        buffer[(i + 1) * bufferSize.X].Char.AsciiChar = '#'; // Left border
        buffer[(i + 1) * bufferSize.X].Attributes = frameColor;
        for (int j = 0; j < width; ++j) {
            if (grid[i][j]) {
                buffer[(i + 1) * bufferSize.X + j + 1].Char.AsciiChar = '1';
                buffer[(i + 1) * bufferSize.X + j + 1].Attributes = fallenBlockColor;
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
        buffer[(i + 1) * bufferSize.X + width + 1].Attributes = frameColor;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; ++i) {
        buffer[(height + 1) * bufferSize.X + i].Char.AsciiChar = '#';
        buffer[(height + 1) * bufferSize.X + i].Attributes = frameColor;
    }

    // Display score only
    string scoreStr = "Score: " + to_string(score);
    for (int i = 0; i < scoreStr.size(); ++i) {
        buffer[(height + 2) * bufferSize.X + i].Char.AsciiChar = scoreStr[i];
        buffer[(height + 2) * bufferSize.X + i].Attributes = scoreTextColor; // Teal color
    }

    // Write buffer to console
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(bufferSize.X - 1), static_cast<SHORT>(bufferSize.Y - 1) };
    WriteConsoleOutputA(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
}

// File handling functions
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

// Menu display functions
void displayPauseMenu() {
    system("cls"); // Clear the screen
    int consoleWidth = 70; // Adjust as needed
    string separator(consoleWidth, '=');
    string pauseText = "GAME PAUSED";
    string option1 = "(R) Restart";
    string option2 = "(Enter) Resume";
    string option3 = "(M) Customize";
    string option4 = "(ESC/B) Back to Home";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, menuTextColor); // Use customizable menu text color

    cout << "\n";
    cout << separator << endl; // Top border
    cout << "\n";

    int pausePadding = (consoleWidth - pauseText.size()) / 2;
    cout << string(pausePadding, ' ') << pauseText << endl; // Larger title
    cout << endl; // Add spacing between the title and options

    int leftMargin = 10; // Fixed left margin for options
    cout << string(leftMargin, ' ') << option1 << endl;
    cout << string(leftMargin, ' ') << option2 << endl;
    cout << string(leftMargin, ' ') << option3 << endl;
    cout << string(leftMargin, ' ') << option4 << endl;

    cout << "\n";
    cout << separator << endl; // Bottom border

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void displayHomeWindow() {
    system("cls"); // Clear the console

    int consoleWidth = 70; // Adjust as needed
    string separator(consoleWidth, '=');
    string option1 = "(Q) Quickie Mode";
    string option2 = "(A) Advanced Mode";
    string option3 = "(S) Show Scoreboard";
    string option4 = "(C) Customize";
    string option5 = "(E/Esc) Exit";

    // ASCII Art for "Tetris"
    string tetrisArt = R"(
  TTTTTT  EEEEEE  TTTTTT  RRRRRR   IIIIII  SSSSSS
    TT    EE        TT    RR   RR    II    SS
    TT    EEEEE     TT    RRRRRR     II    SSSSSS
    TT    EE        TT    RR RR      II        SS
    TT    EEEEEE    TT    RR RRR   IIIIII  SSSSSS
    )";

    // Calculate the padding for center alignment of ASCII art
    int paddingForArt = (consoleWidth - 70) / 2;  // Width of the ASCII art
    int leftMargin = 10; // Fixed left margin for options

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Set the console text color to teal
    SetConsoleTextAttribute(hConsole, 3); 

    // Display the ASCII art in colorful parts
    cout << "\n";
    cout << separator << endl; // Top border

    // Loop through each character in the ASCII art and change colors
    for (int i = 0; i < tetrisArt.length(); i++) {
        if (tetrisArt[i] == '\n') {
            cout << endl; // Print newline character as normal
        } else {
            // Cycle through colors for each character
            int color = (i % 7) + 1; // Colors range from 1 to 7
            SetConsoleTextAttribute(hConsole, color);

            cout << tetrisArt[i]; // Print the character in its assigned color
        }
    }

    // Set the console text color to teal
    SetConsoleTextAttribute(hConsole, 3); 
    
    cout << endl;
    cout << separator << endl; // Below title border
    cout << "\n";

    // Displaying menu options with left alignment
    cout << string(leftMargin, ' ') << option1 << endl;
    cout << string(leftMargin, ' ') << option2 << endl;
    cout << string(leftMargin, ' ') << option3 << endl;
    cout << string(leftMargin, ' ') << option4 << endl;
    cout << string(leftMargin, ' ') << option5 << endl;

    cout << "\n";
    cout << separator << endl; // Bottom border
    cout << string((consoleWidth - 30) / 2, ' ') << "Select your option: ";

    // Reset the color to default after displaying everything
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
    cout << string((consoleWidth - 20) / 2, ' ') << "Press any key to exit...";
    _getch(); // Wait for user input

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void showLeaderboard() {
    system("cls"); // Clear the console
    ifstream file("leaderboard.txt");
    vector<pair<string, int>> leaderboard;

    // Load leaderboard data from file
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            leaderboard.push_back({name, score});
        }
        file.close();
    }

    // Sort leaderboard by score in descending order
    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second < a.second;
    });

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color

    cout << "\n\n";
    int consoleWidth = 50; // Adjust as needed
    string title = "Leaderboard";
    string separator(consoleWidth, '=');
    int padding = (consoleWidth - title.size()) / 2;

    cout << string(padding, ' ') << separator << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(padding, ' ') << separator << endl;

    if (leaderboard.empty()) {
        cout << "\n";
        string noGamesMessage = "No games have been played yet.";
        padding = (consoleWidth - noGamesMessage.size()) / 2;
        cout << string(padding, ' ') << noGamesMessage << endl;
    } else {
        cout << "\n";
        cout << "Rank   Username       Score\n";
        cout << "===========================\n";
        for (int i = 0; i < leaderboard.size() && i < 10; ++i) { // Display top 10 scores
            string rank = to_string(i + 1) + ".";
            string name = leaderboard[i].first;
            string score = to_string(leaderboard[i].second);

            cout << rank << string(7 - rank.size(), ' ') // Align rank
                 << name << string(15 - name.size(), ' ') // Align username
                 << score << endl; // Display score
        }
    }

    cout << "\n";
    cout << string((consoleWidth - 20) / 2, ' ') << "Press any key to exit...";
    _getch(); // Wait for user input

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void displayColorOptions() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Array of color names corresponding to color codes
    string colorNames[] = {
        "Black", "Blue", "Green", "Cyan", "Red", "Magenta", "Yellow", "White",
        "Gray", "Bright Blue", "Bright Green", "Bright Cyan", "Bright Red", "Bright Magenta", "Bright Yellow", "Bright White"
    };

    cout << "Available Colors:\n";
    cout << "=================\n";

    for (int i = 1; i <= 15; ++i) {
        SetConsoleTextAttribute(hConsole, i); // Set the console text color
        cout << i << ". " << colorNames[i] << "\n"; // Display the color code and its name
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
    cout << "=================\n";
}

void resetToDefault() {
    // Reset all customizable settings to their default values
    frameColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Default white
    fallenBlockColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Default white
    menuTextColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Default green
    scoreTextColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Default teal

    tetrominoColors = {
        FOREGROUND_BLUE | FOREGROUND_INTENSITY, // I
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // T
        FOREGROUND_RED | FOREGROUND_INTENSITY, // Z
        FOREGROUND_GREEN | FOREGROUND_INTENSITY, // S
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // O
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // L
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY // J
    };

    cout << "All settings have been reset to default values.\n";
    cout << "Press any key to return to the customization menu...";
    _getch();
}

void customizeGame() {
    system("cls"); // Clear the console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color

    while (true) {
        system("cls");
        cout << "\n\n";
        cout << "Customize Your Game\n";
        cout << "===================\n";
        cout << "1. Change Tetromino Colors\n";
        cout << "2. Change Frame Color\n";
        cout << "3. Change Fallen Blocks Color\n";
        cout << "4. Change Home and Pause Window Text Color\n";
        cout << "5. Change Current Score Text Color\n";
        cout << "6. Reset to Default\n";
        cout << "7. Back to Menu\n";
        cout << "===================\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                // Change Tetromino Colors
                system("cls");
                cout << "Change Tetromino Colors\n";
                cout << "========================\n";
                cout << "1. I Tetromino (Current Color: Blue)\n";
                cout << "2. T Tetromino (Current Color: Purple)\n";
                cout << "3. Z Tetromino (Current Color: Red)\n";
                cout << "4. S Tetromino (Current Color: Green)\n";
                cout << "5. O Tetromino (Current Color: White)\n";
                cout << "6. L Tetromino (Current Color: Yellow)\n";
                cout << "7. J Tetromino (Current Color: Cyan)\n";
                cout << "8. Back to Customization Menu\n";
                cout << "========================\n";
                cout << "Enter your choice: ";

                int colorChoice;
                cin >> colorChoice;

                if (colorChoice >= 1 && colorChoice <= 7) {
                    cout << "Enter new color code (1-15):\n";
                    displayColorOptions(); // Show color options with examples
                    int newColor;
                    cin >> newColor;

                    if (newColor >= 1 && newColor <= 15) {
                        tetrominoColors[colorChoice - 1] = newColor;
                        cout << "Color updated successfully!\n";
                    } else {
                        cout << "Invalid color code. Please enter a value between 1 and 15.\n";
                    }
                } else if (colorChoice == 8) {
                    break; // Back to customization menu
                } else {
                    cout << "Invalid choice. Please try again.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 2: {
                // Change Frame Color
                system("cls");
                cout << "Change Frame Color\n";
                cout << "==================\n";
                cout << "Enter new color code for the frame (1-15):\n";
                displayColorOptions(); // Show color options with examples
                int newColor;
                cin >> newColor;

                if (newColor >= 1 && newColor <= 15) {
                    frameColor = newColor; // Update the global frame color variable
                    cout << "Frame color updated successfully!\n";
                } else {
                    cout << "Invalid color code. Please enter a value between 1 and 15.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 3: {
                // Change Fallen Blocks Color
                system("cls");
                cout << "Change Fallen Blocks Color\n";
                cout << "==========================\n";
                cout << "Enter new color code for fallen blocks (1-15):\n";
                displayColorOptions(); // Show color options with examples
                int newColor;
                cin >> newColor;

                if (newColor >= 1 && newColor <= 15) {
                    fallenBlockColor = newColor; // Update the global fallen block color variable
                    cout << "Fallen blocks color updated successfully!\n";
                } else {
                    cout << "Invalid color code. Please enter a value between 1 and 15.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 4: {
                // Change Home and Pause Window Text Color
                system("cls");
                cout << "Change Home and Pause Window Text Color\n";
                cout << "=======================================\n";
                cout << "Enter new color code for menu text (1-15):\n";
                displayColorOptions(); // Show color options with examples
                int newColor;
                cin >> newColor;

                if (newColor >= 1 && newColor <= 15) {
                    menuTextColor = newColor; // Update the global menu text color variable
                    cout << "Menu text color updated successfully!\n";
                } else {
                    cout << "Invalid color code. Please enter a value between 1 and 15.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 5: {
                // Change Current Score Text Color
                system("cls");
                cout << "Change Current Score Text Color\n";
                cout << "===============================\n";
                cout << "Enter new color code for the score text (1-15):\n";
                displayColorOptions(); // Show color options with examples
                int newColor;
                cin >> newColor;

                if (newColor >= 1 && newColor <= 15) {
                    scoreTextColor = newColor; // Update the global score text color variable
                    cout << "Score text color updated successfully!\n";
                } else {
                    cout << "Invalid color code. Please enter a value between 1 and 15.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 6: {
                // Reset to Default
                resetToDefault();
                break;
            }
            case 7:
                return; // Back to main menu
            default:
                cout << "Invalid choice. Please try again.\n";
                cout << "Press any key to return...";
                _getch();
                break;
        }
    }
}

// Game logic
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
                        case 27: // ESC key to open pause menu
                        case 13: // Enter key to open pause menu
                            bool inPauseMenu = true;
                            while (inPauseMenu) {
                                displayPauseMenu();
                                char choice = _getch();
                                switch (tolower(choice)) {
                                    case 'r': // Restart
                                        resetGameState();
                                        initialize();
                                        system("cls"); // Clear the screen after choosing restart
                                        inPauseMenu = false;
                                        break;
                                    case 13: // Enter to Resume
                                    case 'c': // Resume
                                        system("cls"); // Clear the screen after choosing resume
                                        inPauseMenu = false;
                                        break;
                                    case 'm': // Customize
                                        customizeGame();
                                        break;
                                    case 'b': // Back to Home
                                    case 27: // ESC key for Back to Home
                                        system("cls"); // Clear the screen
                                        return; // Exit the game loop and return to the home window
                                    default:
                                        cout << "\nInvalid input. Please try again.\n";
                                        Sleep(1000); // Pause for 1 second to show the error
                                        break;
                                }
                            }
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Red color for Game Over

    int consoleWidth = 70; // Adjust to match the screen width
    string border(consoleWidth, '=');
    string gameOverText = "GAME OVER";
    string scoreText = "Your Score: " + to_string(score);
    string highScoreText = (score > highScore) ? "New High Score: " + to_string(score) + "!" : "High Score: " + to_string(highScore);
    string exitText = "Press any key to return to the home window...";

    cout << "\n\n";
    cout << border << endl;
    cout << string((consoleWidth - gameOverText.size()) / 2, ' ') << gameOverText << endl;
    cout << border << endl;
    cout << "\n";
    cout << string((consoleWidth - scoreText.size()) / 2, ' ') << scoreText << endl;
    cout << string((consoleWidth - highScoreText.size()) / 2, ' ') << highScoreText << endl;
    cout << "\n";
    cout << string((consoleWidth - exitText.size()) / 2, ' ') << exitText << endl;
    cout << border << endl; // Add a border line after "Press any key to exit..."

    _getch(); // Wait for user input
    system("cls"); // Clear the screen and return to the home window
}

void startGame(bool advancedMode) {
    resetGameState(); // Reset the game state

    if (advancedMode) {
        cout << "Enter Username: ";
        cin >> username;
        loadHighScore();
        cout << "Current High Score: " << highScore << endl;
    }

    initialize();
    gameLoop();
}

// Main function
int main() {
    srand(time(0)); // Initialize random seed

    while (true) {
        displayHomeWindow(); // Display the home window

        char mode;
        mode = _getch();

        if (tolower(mode) == 'a') {
            startGame(true);
        } else if (tolower(mode) == 'q') {
            startGame(false);
        } else if (tolower(mode) == 's') {
            showScoreboard();
        } else if (tolower(mode) == 'c') {
            customizeGame();
        } else if (tolower(mode) == 'e' || mode == 27) { // Exit
            saveHighScore();
            return 0;
        } else {
            cout << "\nInvalid input.\n";
            Sleep(1000); // Pause for 1 second to show the error
        }
    }

    return 0;
}