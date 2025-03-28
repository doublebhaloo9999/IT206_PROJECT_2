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
    {{1, 1, 1}, {0, 0, 1}}, // J
    {{1, 1, 1, 1}, {1, 0, 0, 1}} // New larger tetromino
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

void displayPauseMenu() {
    system("cls"); // Clear the screen
    int consoleWidth = 70; // Adjust as needed
    string separator(consoleWidth, '=');
    string pauseText = "GAME PAUSED";
    string option1 = "(R) Restart";
    string option2 = "(C) Resume";
    string option3 = "(M) Customize";
    string option4 = "(E/Esc) Exit";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green color

    cout << "\n";
    cout << separator << endl; // Top border
    cout << "\n";

    int pausePadding = (consoleWidth - pauseText.size()) / 2;
    cout << string(pausePadding, ' ') << pauseText << endl; // Larger title
    cout << endl; // Add spacing between the title and options

    int optionPadding = (consoleWidth - option1.size()) / 2;
    cout << string(optionPadding, ' ') << option1 << endl;

    optionPadding = (consoleWidth - option2.size()) / 2;
    cout << string(optionPadding, ' ') << option2 << endl;

    optionPadding = (consoleWidth - option3.size()) / 2;
    cout << string(optionPadding, ' ') << option3 << endl;

    optionPadding = (consoleWidth - option4.size()) / 2;
    cout << string(optionPadding, ' ') << option4 << endl;

    cout << "\n";
    cout << separator << endl; // Bottom border

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
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
                        case 27 : // (ESC key=27) displays pause screen
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
                                    case 'c': // Resume
                                        system("cls"); // Clear the screen after choosing resume
                                        inPauseMenu = false;
                                        break;
                                    case 'e': // Exit
                                        system("cls"); // Clear the screen before exiting
                                        exit(0); // Exit the game
                                        break;
                                    case 27: //ESC exit
                                        system("cls");
                                        exit(0);
                                        break;
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
    string exitText = "Press any key to continue...";

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
    int paddingForMenu;

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

    // Displaying menu options centered below the ASCII art
    paddingForMenu = (consoleWidth - option1.size()) / 2;
    cout << string(paddingForMenu, ' ') << option1 << endl;

    paddingForMenu = (consoleWidth - option2.size()) / 2;
    cout << string(paddingForMenu, ' ') << option2 << endl;

    paddingForMenu = (consoleWidth - option3.size()) / 2;
    cout << string(paddingForMenu, ' ') << option3 << endl;

    paddingForMenu = (consoleWidth - option4.size()) / 2;
    cout << string(paddingForMenu, ' ') << option4 << endl;

    paddingForMenu = (consoleWidth - option5.size()) / 2;
    cout << string(paddingForMenu, ' ') << option5 << endl;

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
        cout << "2. Adjust Game Speed\n";
        cout << "3. Reset High Scores\n";
        cout << "4. Back to Menu\n";
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
                    cout << "Enter new color code (1-15): ";
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
                // Adjust Game Speed
                system("cls");
                cout << "Adjust Game Speed\n";
                cout << "=================\n";
                cout << "Current Speed: " << (500 - (level - 1) * 50) << " ms per fall\n";
                cout << "Enter new fall interval (in milliseconds, e.g., 300 for faster, 700 for slower): ";
                int newSpeed;
                cin >> newSpeed;

                if (newSpeed >= 100 && newSpeed <= 1000) {
                    level = 1; // Reset level to apply new speed
                    cout << "Game speed updated successfully!\n";
                } else {
                    cout << "Invalid speed. Please enter a value between 100 and 1000 ms.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 3: {
                // Reset High Scores
                system("cls");
                cout << "Reset High Scores\n";
                cout << "=================\n";
                cout << "Are you sure you want to reset all high scores? (y/n): ";
                char confirm;
                cin >> confirm;

                if (tolower(confirm) == 'y') {
                    highScore = 0;
                    saveHighScore();
                    cout << "High scores reset successfully!\n";
                } else {
                    cout << "High scores were not reset.\n";
                }

                cout << "Press any key to return...";
                _getch();
                break;
            }
            case 4:
                return; // Back to main menu
            default:
                cout << "Invalid choice. Please try again.\n";
                cout << "Press any key to return...";
                _getch();
                break;
        }
    }
}

void startGame(bool advancedMode) {
    resetGameState(); // Reset the game state

    if (advancedMode) {
        cout << "Enter your username: ";
        cin >> username;
        loadHighScore();
        cout << "Current High Score: " << highScore << endl;
        // cout<<"Do you want to customize? (y/n)"<<endl;
        // char choice;
        // choice = _getch();
        // if(tolower(choice) == 'y'){
            
        // }
    }

    initialize();
    gameLoop();

    char choice;
    cout << "Do you want to play again? (y/n): ";
    choice = _getch();
    if (choice == 'y' || choice == 'Y') {
        startGame(advancedMode);
    }
}

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
            cout << "\nInvalid input. Please select a valid option.\n";
            Sleep(1000); // Pause for 1 second to show the error
        }
    }

    return 0;
}