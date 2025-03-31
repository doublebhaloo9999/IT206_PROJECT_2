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
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
vector<vector<int>> gameGrid(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
int currentScore = 0, currentLevel = 1, totalLinesCleared = 0;
string playerName;
int personalHighScore = 0, globalHighScore = 0;

// Default colors for various elements
int borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int settledBlockColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
int menuTextColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
int scoreDisplayColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

// Tetromino shapes and colors
vector<vector<vector<int>>> tetrominoShapes = {
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
    int x, y, color;
};

Tetromino activeTetromino;

// Utility functions
void resetGame() {
    // Resets the game grid and score-related variables
    gameGrid = vector<vector<int>>(GRID_HEIGHT, vector<int>(GRID_WIDTH, 0));
    currentScore = 0;
    currentLevel = 1;
    totalLinesCleared = 0;
}

void initializeTetromino() {
    // Initializes a new Tetromino at the start of the game or after placing one
    int randomIndex = rand() % tetrominoShapes.size();
    activeTetromino.shape = tetrominoShapes[randomIndex];
    activeTetromino.x = GRID_WIDTH / 2 - activeTetromino.shape[0].size() / 2;
    activeTetromino.y = 0;
    activeTetromino.color = tetrominoColors[randomIndex];
}

bool isCollision(int newX, int newY, const vector<vector<int>>& newShape) {
    // Checks if the Tetromino collides with the grid boundaries or other blocks
    for (int i = 0; i < newShape.size(); ++i) {
        for (int j = 0; j < newShape[i].size(); ++j) {
            if (newShape[i][j]) {
                int gridX = newX + j;
                int gridY = newY + i;
                if (gridX < 0 || gridX >= GRID_WIDTH || gridY >= GRID_HEIGHT || (gridY >= 0 && gameGrid[gridY][gridX])) {
                    return true;
                }
            }
        }
    }
    return false;
}

void placeTetrominoOnGrid() {
    // Places the current Tetromino on the grid
    for (int i = 0; i < activeTetromino.shape.size(); ++i) {
        for (int j = 0; j < activeTetromino.shape[i].size(); ++j) {
            if (activeTetromino.shape[i][j]) {
                gameGrid[activeTetromino.y + i][activeTetromino.x + j] = 1;
            }
        }
    }
}

// Add a global variable for speed adjustment
int fallInterval = 500; // Default fall interval
bool isSpeedDynamic = true; // Determines if speed changes dynamically

// Modify clearFullLines to use linesPerSpeedIncrease
void clearFullLines() {
    // Clears full lines from the grid, updates the score, and adjusts speed dynamically
    for (int row = GRID_HEIGHT - 1; row >= 0; --row) {
        bool isLineFull = all_of(gameGrid[row].begin(), gameGrid[row].end(), [](int cell) { return cell != 0; });
        if (isLineFull) {
            // Remove the full line and add an empty line at the top
            gameGrid.erase(gameGrid.begin() + row);
            gameGrid.insert(gameGrid.begin(), vector<int>(GRID_WIDTH, 0));

            // Update score and track cleared lines
            currentScore += 100;
            totalLinesCleared++;

            // Adjust speed dynamically based on cleared lines
            if (isSpeedDynamic && totalLinesCleared % 5 == 0) {
                currentLevel++;
                fallInterval = max(50, 500 - (currentLevel - 1) * 50); // Minimum interval is 50ms
            }

            // Recheck the same row after shifting
            row++;
        }
    }
}

void rotateActiveTetromino() {
    // Rotates the current Tetromino clockwise
    vector<vector<int>> rotatedShape(activeTetromino.shape[0].size(), vector<int>(activeTetromino.shape.size()));

    for (int row = 0; row < activeTetromino.shape.size(); ++row) {
        for (int col = 0; col < activeTetromino.shape[row].size(); ++col) {
            rotatedShape[col][activeTetromino.shape.size() - 1 - row] = activeTetromino.shape[row][col];
        }
    }

    // Apply rotation only if it doesn't cause a collision
    if (!isCollision(activeTetromino.x, activeTetromino.y, rotatedShape)) {
        activeTetromino.shape = rotatedShape;
    }
}

// Drawing and rendering functions
void draw(HANDLE hConsole, COORD bufferSize, CHAR_INFO* buffer) {
    // Clears the buffer and renders the game grid, Tetromino, and UI elements
    for (int row = 0; row < bufferSize.Y; ++row) {
        for (int col = 0; col < bufferSize.X; ++col) {
            buffer[row * bufferSize.X + col].Char.AsciiChar = ' ';
            buffer[row * bufferSize.X + col].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }

    // Draw top border
    for (int col = 0; col < GRID_WIDTH + 2; ++col) {
        buffer[col].Char.AsciiChar = '#';
        buffer[col].Attributes = borderColor;
    }

    // Draw game grid and Tetromino
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        buffer[(row + 1) * bufferSize.X].Char.AsciiChar = '#'; // Left border
        buffer[(row + 1) * bufferSize.X].Attributes = borderColor;

        for (int col = 0; col < GRID_WIDTH; ++col) {
            if (gameGrid[row][col]) {
                // Render settled blocks
                buffer[(row + 1) * bufferSize.X + col + 1].Char.AsciiChar = '1';
                buffer[(row + 1) * bufferSize.X + col + 1].Attributes = settledBlockColor;
            } else {
                // Render active Tetromino
                bool isTetrominoPart = false;
                for (int tetRow = 0; tetRow < activeTetromino.shape.size(); ++tetRow) {
                    for (int tetCol = 0; tetCol < activeTetromino.shape[tetRow].size(); ++tetCol) {
                        if (activeTetromino.shape[tetRow][tetCol] && activeTetromino.y + tetRow == row && activeTetromino.x + tetCol == col) {
                            buffer[(row + 1) * bufferSize.X + col + 1].Char.AsciiChar = '1';
                            buffer[(row + 1) * bufferSize.X + col + 1].Attributes = activeTetromino.color;
                            isTetrominoPart = true;
                            break;
                        }
                    }
                    if (isTetrominoPart) break;
                }
            }
        }

        buffer[(row + 1) * bufferSize.X + GRID_WIDTH + 1].Char.AsciiChar = '#'; // Right border
        buffer[(row + 1) * bufferSize.X + GRID_WIDTH + 1].Attributes = borderColor;
    }

    // Draw bottom border
    for (int col = 0; col < GRID_WIDTH + 2; ++col) {
        buffer[(GRID_HEIGHT + 1) * bufferSize.X + col].Char.AsciiChar = '#';
        buffer[(GRID_HEIGHT + 1) * bufferSize.X + col].Attributes = borderColor;
    }

    // Display score
    string scoreStr = "Score: " + to_string(currentScore);
    for (int i = 0; i < scoreStr.size(); ++i) {
        buffer[(GRID_HEIGHT + 2) * bufferSize.X + i].Char.AsciiChar = scoreStr[i];
        buffer[(GRID_HEIGHT + 2) * bufferSize.X + i].Attributes = scoreDisplayColor;
    }

    // Write buffer to console
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(bufferSize.X - 1), static_cast<SHORT>(bufferSize.Y - 1) };
    WriteConsoleOutputA(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
}

// File handling functions
void loadGlobalHighScore() {
    // Loads the global high score from the leaderboard file
    ifstream leaderboardFile("leaderboard.txt");
    if (leaderboardFile.is_open()) {
        string name;
        int score;
        if (leaderboardFile >> name >> score) { // Read the first entry
            globalHighScore = score;
        }
        leaderboardFile.close();
    } else {
        globalHighScore = 0; // Default to 0 if leaderboard.txt doesn't exist
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
    string option3 = "(C) Customize";
    string option4 = "(ESC/B) Back to Home";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green color for pause menu

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
    string option3 = "(L) Leaderboard";
    string option4 = "(C) Customize";
    string option5 = "(E/Esc) Exit";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

    
    // Set the console text color to teal
    SetConsoleTextAttribute(hConsole, 3); 

    // Display the ASCII art in colorful parts
    cout << "\n";
    cout << separator << endl; // Top border

    // Display ASCII art with color-changing effect
    for (int i = 0; i < tetrisArt.length(); i++) {
        if (tetrisArt[i] == '\n') {
            cout << endl; // Print newline character as normal
        } else {
            int color = (i % 7) + 1; // Cycle through colors (1 to 7)
            SetConsoleTextAttribute(hConsole, color);
            cout << tetrisArt[i];
        }
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color
    cout << endl;
    cout << separator << endl; // Below title border
    cout << "\n";

    // Displaying menu options
    cout << option1 << endl;
    cout << option2 << endl;
    cout << option3 << endl;
    cout << option4 << endl;
    cout << option5 << endl;

    cout << "\n";
    cout << separator << endl; // Bottom border

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

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Teal color

    cout << "\n\n";
    int consoleWidth = 50; // Adjust as needed
    string title = "Leaderboard";
    string separator(consoleWidth, '=');
    int padding = (consoleWidth - title.size()) / 2;

    cout << separator << endl;
    cout << string(padding, ' ') << title << endl;
    cout << separator << endl;

    if (leaderboard.empty()) {
        cout << "\n";
        string noGamesMessage = "No games have been played yet.";
        padding = (consoleWidth - noGamesMessage.size()) / 2;
        cout << string(padding, ' ') << noGamesMessage << endl;
    } else {
        // Print the leaderboard with proper alignment
        cout << "+------+-----------------+-------+" << endl;
        cout << "| Rank | Username        | Score |" << endl;
        cout << "+------+-----------------+-------+" << endl;
        for (int i = 0; i < leaderboard.size() && i < 10; ++i) { // Display top 10 scores
            string rank = to_string(i + 1);
            string name = leaderboard[i].first;
            string score = to_string(leaderboard[i].second);

            cout << "| " << rank << string(4 - rank.size(), ' ') // Align rank
                 << " | " << name << string(15 - name.size(), ' ') // Align username
                 << " | " << score << string(5 - score.size(), ' ') << " |" << endl; // Align score
        }
        cout << "+------+-----------------+-------+" << endl;
    }

    cout << "\n";
    cout << string((consoleWidth - 20) / 2, ' ') << "Press any key to exit...";
    _getch(); // Wait for user input

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
}

void displayColorTemplate(HANDLE hConsole, int originalColor) {
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

    SetConsoleTextAttribute(hConsole, originalColor); // Restore the original text color
    cout << "=================\n";
}

void resetToDefault() {
    // Reset all customizable settings to their default values
    borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Default white
    settledBlockColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Default white
    menuTextColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Default green
    scoreDisplayColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Default teal

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

// Add a new menu option for adjusting speed in customizeGame
void customizeGame() {
    system("cls"); // Clear the console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        system("cls");
        SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
        cout << "\n\n";
        cout << "Customize Your Game\n";
        cout << "===================\n";
        cout << "1. Change Falling Tetromino Colors\n";
        cout << "2. Change Frame Color\n";
        cout << "3. Change Settled Blocks Color\n";
        cout << "4. Change Home Window Text Color\n";
        cout << "5. Change Pause Window Text Color\n";
        cout << "6. Change Customization Menu Text Color\n";
        cout << "7. Change Current Score Text Color\n";
        cout << "8. Reset to Default\n";
        cout << "9. Adjust Speed\n";
        cout << "(B) Back to Menu\n";
        cout << "===================\n";
        cout << "Enter your choice: ";

        char choice = _getch(); // Use _getch() to get a single character input

        switch (choice) {
            case '1': {
                // Change Tetromino Colors
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Tetromino Colors\n";
                cout << "========================\n";
                for (int i = 0; i < tetrominoColors.size(); ++i) {
                    cout << i + 1 << ". Tetromino " << char('I' + i) << " (Current Color: " << tetrominoColors[i] << ")\n";
                }
                cout << tetrominoColors.size() + 1 << ". Back to Customization Menu\n";
                cout << "========================\n";
                cout << "Enter your choice: ";

                int colorChoice;
                cin >> colorChoice;

                if (colorChoice >= 1 && colorChoice <= 7) {
                    int index = colorChoice - 1;
                    cout << "\nCurrent Color: " << tetrominoColors[index] << "\n";
                    cout << "Enter new color code (1-15) or '0' to go back: ";
                    displayColorTemplate(hConsole, menuTextColor);
                    int newColor;
                    cin >> newColor;

                    if (newColor == 0) {
                        break; // Go back without changing the color
                    }

                    if (newColor >= 1 && newColor <= 15) {
                        tetrominoColors[index] = newColor;
                        cout << "\nColor updated successfully!\n";
                    } else {
                        cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                    }
                } else if (colorChoice == 8) {
                    break; // Back to customization menu
                } else {
                    cout << "\nInvalid choice. Please try again.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '2': {
                // Change Frame Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Frame Color\n";
                cout << "==================\n";
                cout << "Current Color: " << borderColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    borderColor = newColor;
                    cout << "\nFrame color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '3': {
                // Change Fallen Blocks Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Fallen Blocks Color\n";
                cout << "==========================\n";
                cout << "Current Color: " << settledBlockColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    settledBlockColor = newColor;
                    cout << "\nFallen blocks color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '4': {
                // Change Home Window Text Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Home Window Text Color\n";
                cout << "=============================\n";
                cout << "Current Color: " << menuTextColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    menuTextColor = newColor;
                    cout << "\nHome window text color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '5': {
                // Change Pause Window Text Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Pause Window Text Color\n";
                cout << "==============================\n";
                cout << "Current Color: " << menuTextColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    menuTextColor = newColor;
                    cout << "\nPause window text color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '6': {
                // Change Customization Menu Text Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Customization Menu Text Color\n";
                cout << "====================================\n";
                cout << "Current Color: " << menuTextColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    menuTextColor = newColor;
                    cout << "\nCustomization menu text color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '7': {
                // Change Current Score Text Color
                system("cls");
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color
                cout << "Change Current Score Text Color\n";
                cout << "===============================\n";
                cout << "Current Color: " << scoreDisplayColor << "\n";
                cout << "Enter new color code (1-15) or '0' to go back: ";
                displayColorTemplate(hConsole, menuTextColor);
                int newColor;
                cin >> newColor;

                if (newColor == 0) {
                    break; // Go back without changing the color
                }

                if (newColor >= 1 && newColor <= 15) {
                    scoreDisplayColor = newColor;
                    cout << "\nScore text color updated successfully!\n";
                } else {
                    cout << "\nInvalid color code. Please enter a value between 1 and 15.\n";
                }

                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '8': {
                // Reset to Default
                resetToDefault();
                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case '9': {
                system("cls");
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, menuTextColor); // Use the current menu text color

                cout << "Adjust Speed\n";
                cout << "========================\n";
                cout << "1. GrandMaster\n";
                cout << "2. Expert \n";
                cout << "3. Advanced \n";
                cout << "4. Hard \n";
                cout << "5. Normal \n";
                cout << "6. Reset Default \n";
                cout << "7. NOOB \n";
                cout << "8. Back to Customization Menu\n";
                cout << "========================\n";
                cout << "Enter your choice: ";

                char speedChoice = _getch();
                switch (speedChoice) {
                    case '1': // GrandMaster
                        isSpeedDynamic = false;
                        fallInterval = 50; // Set fixed fall interval
                        cout << "\nSpeed adjustment set to GrandMaster.\n";
                        break;
                    case '2': // Expert
                        isSpeedDynamic = false;
                        fallInterval = 100; // Set fixed fall interval
                        cout << "\nSpeed adjustment set to Expert.\n";
                        break;
                    case '3': // Advanced
                        isSpeedDynamic = false;
                        fallInterval = 200; // Set fixed fall interval
                        cout << "\nSpeed adjustment set to Advanced.\n";
                        break;
                    case '4': // Hard
                        isSpeedDynamic = false;
                        fallInterval = 400; // Set fixed fall interval
                        cout << "\nSpeed adjustment set to Hard.\n";
                        break;
                    case '5': // Normal
                        isSpeedDynamic = false;
                        fallInterval = 500; // Set fixed fall interval
                        cout << "\nSpeed adjustment set to Normal.\n";
                        break;
                    case '6': // Reset Default
                        isSpeedDynamic = true;
                        fallInterval = 500; // Reset to default fall interval
                        cout << "\nSpeed adjustment reset to Default.\n";
                        break;
                    case '7': // NOOB
                        isSpeedDynamic = false;
                        fallInterval = 500; // Constant speed
                        cout << "\nSpeed adjustment set to NOOB mode (constant speed).\n";
                        break;
                    case '8': // Back to Customization Menu
                        return;
                    default:
                        cout << "\nInvalid choice. Please try again.\n";
                        break;
                }
                Sleep(1000); // Pause for 1 second to show the message
                break;
            }
            case 'b': // ESC key
                return; // Back to main menu
            default:
                cout << "\nInvalid choice. Please try again.\n";
                Sleep(1000); // Pause for 1 second to show the error
                break;
        }

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to default
    }
}

// Game logic
void gameLoop() {
    system("cls"); // Clear the terminal before starting the game

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Declare only once
    COORD bufferSize = { GRID_WIDTH + 2, GRID_HEIGHT + 3 };
    CHAR_INFO buffer[bufferSize.X * bufferSize.Y];

    auto lastFrameTime = high_resolution_clock::now();
    auto lastFallTime = high_resolution_clock::now();
    int fallInterval = 500 - (currentLevel - 1) * 50; // Initial fall interval

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
                            if (!isCollision(activeTetromino.x - 1, activeTetromino.y, activeTetromino.shape)) {
                                activeTetromino.x--;
                            }
                            break;
                        case 77: // Right arrow key
                            if (!isCollision(activeTetromino.x + 1, activeTetromino.y, activeTetromino.shape)) {
                                activeTetromino.x++;
                            }
                            break;
                        case 80: // Down arrow key
                            if (!isCollision(activeTetromino.x, activeTetromino.y + 1, activeTetromino.shape)) {
                                activeTetromino.y++;
                            }
                            break;
                        case 72: // Up arrow key
                            rotateActiveTetromino();
                            break;
                    }
                } else {
                    switch (ch) {
                        case 'a':
                            if (!isCollision(activeTetromino.x - 1, activeTetromino.y, activeTetromino.shape)) {
                                activeTetromino.x--;
                            }
                            break;
                        case 'd':
                            if (!isCollision(activeTetromino.x + 1, activeTetromino.y, activeTetromino.shape)) {
                                activeTetromino.x++;
                            }
                            break;
                        case 's':
                            if (!isCollision(activeTetromino.x, activeTetromino.y + 1, activeTetromino.shape)) {
                                activeTetromino.y++;
                            }
                            break;
                        case 'w':
                            rotateActiveTetromino();
                            break;
                        case ' ':
                            while (!isCollision(activeTetromino.x, activeTetromino.y + 1, activeTetromino.shape)) {
                                activeTetromino.y++;
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
                                        resetGame();
                                        initializeTetromino();
                                        system("cls"); // Clear the screen after choosing restart
                                        inPauseMenu = false;
                                        break;
                                    case 13: // Enter to Resume
                                    case 'c': // Resume
                                        system("cls"); // Clear the screen after choosing resume
                                        inPauseMenu = false;
                                        break;
                                    case ' ': // Customize
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
                if (!isCollision(activeTetromino.x, activeTetromino.y + 1, activeTetromino.shape)) {
                    activeTetromino.y++;
                } else {
                    placeTetrominoOnGrid();
                    int previousLevel = currentLevel; // Track the current level before clearing lines
                    clearFullLines();
                    if (currentLevel != previousLevel) {
                        fallInterval = 500 - (currentLevel - 1) * 50; // Recalculate fall interval
                    }
                    int index = rand() % tetrominoShapes.size();
                    activeTetromino.shape = tetrominoShapes[index];
                    activeTetromino.x = GRID_WIDTH / 2 - activeTetromino.shape[0].size() / 2;
                    activeTetromino.y = 0;
                    activeTetromino.color = tetrominoColors[index];
                    if (isCollision(activeTetromino.x, activeTetromino.y, activeTetromino.shape)) {
                        isGameOver = true;
                    }
                }
            }

            draw(hConsole, bufferSize, buffer);
        }
    }

    // Animated "Game Over" sequence in blood red color
    const string FRAME_PART1 = 
        "\n"
        "        .-''''''''-.  \n"
        "       /            \\  \n"
        "      |              |  \n"
        "      |,  .-.  .-.  ,|  \n"
        "      | )(_o/   \\o_)(|  \n"
        "      |/     /\\    \\|   \n"
        "      (_     ^^     _)  \n"
        "       \\__|IIIIII|__/  \n"
        "        |\\       | |    \n"
        "        | \\IIIIIII |  \n"
        "        \\  ~  ~   /\"\"/  \n"
        "         `--------`  \n";

    const string FRAME_PART2 = 
        "\n"
        "        .-''''''''''-.   \n"
        "       /              \\  \n"
        "      |               |  \n"
        "      |,  .-.  .-.  , |  \n"
        "      | )(_o/  \\o_)( |  \n"
        "      |/     /\\      \\|  \n"
        "      (_     ^^      _)  \n"
        "       \\__|IIIIII|__/  \n"
        "        |  \\IIIIII/ |  \n"
        "        \\         /\"\"/  \n"
        "         `----------`  \n";

    string frames[] = {FRAME_PART1, FRAME_PART2, FRAME_PART1, FRAME_PART2};
    for (int i = 0; i < 10; i++) { // Loop animation
        system("cls");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Blood red color
        cout << frames[i % 4] << endl;
        Sleep(600); // Pause for 600ms
    }

    // Smooth transition to Game Over screen
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Red color for Game Over

    int consoleWidth = 70; // Adjust to match the screen width
    string border(consoleWidth, '=');

    cout << "\n\n";
    cout << border << endl << endl ;

    // Print the Game Over text directly
    cout << u8"   ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗  \n";
    cout << u8"  ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██║ \n";
    cout << u8"  ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╝ \n";
    cout << u8"  ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║██║   ██║██╔══╝  █████║  \n";
    cout << u8"  ██║   ██║██╔══██║██║╚██╔╝██║██║         ██║   ██║██║   ██║██║     ██║ ██║    \n";
    cout << u8"  ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║  ██║   \n";
    cout << u8"   ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝     \n";

    cout << border << endl << endl ;

    string scoreText = "Your Score: " + to_string(currentScore);
    string highScoreText = (currentScore > personalHighScore) ? "New High Score: " + to_string(currentScore) + "!" : "High Score: " + to_string(personalHighScore);
    string exitText = "Press any key to return to the home window...";

    cout << "\n";
    cout << string((consoleWidth - scoreText.size()) / 2, ' ') << scoreText << endl;
    cout << string((consoleWidth - highScoreText.size()) / 2, ' ') << highScoreText << endl;
    cout << "\n";
    cout << string((consoleWidth - exitText.size()) / 2, ' ') << exitText << endl;
    cout << border << endl; // Add a border line after "Press any key to exit..."

    _getch(); // Wait for user input
    system("cls"); // Clear the screen and return to the home window
}

void updateLeaderboard(int finalScore, bool advancedMode) {
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

    // Skip adding entry if in Quickie Mode, score is 0, and leaderboard isn't full
    if (!advancedMode && finalScore == 0 && leaderboard.size() < 10) {
        return;
    }

    // Check if the score qualifies for the leaderboard
    string name;
    if (leaderboard.size() < 10 || finalScore > leaderboard.back().second) {
        if (!advancedMode) { // Ask for username only in Quickie Mode
            cout << "Congratulations! Your score qualifies for the leaderboard.\n";
            cout << "Enter your username (or type 'n' to skip): ";
            cin >> name;

            if (name == "n" || name == "N") {
                name = "UNKNOWN";
            }
        } else {
            name = playerName; // Use the username provided at the start of Advanced Mode
        }

        leaderboard.push_back({name, finalScore});
    }

    // Sort leaderboard by score in descending order and keep top 10
    sort(leaderboard.begin(), leaderboard.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second < a.second;
    });
    if (leaderboard.size() > 10) {
        leaderboard.resize(10);
    }

    // Save updated leaderboard to file
    ofstream outFile("leaderboard.txt");
    if (outFile.is_open()) {
        for (const auto& entry : leaderboard) {
            outFile << entry.first << " " << entry.second << endl;
        }
        outFile.close();
    }
}

void startGame(bool isAdvancedMode) {
    resetGame(); // Reset the game state

    if (isAdvancedMode) {
        cout << "Enter Username: ";
        cin >> playerName;
        cout << "Current High Score: " << globalHighScore << endl;
    }

    initializeTetromino();
    gameLoop();

    // Update leaderboard after the game ends
    updateLeaderboard(currentScore, isAdvancedMode);
    if (isAdvancedMode && currentScore > globalHighScore) {
        globalHighScore = currentScore; // Update the global high score
    }
}

int main() {
    // Set console to UTF-8 mode
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0)); // Initialize random seed
    loadGlobalHighScore(); // Initialize globalHighScore

    while (true) {
        displayHomeWindow(); // Display the home window

        char selectedMode = _getch(); // Get user input for mode selection

        if (tolower(selectedMode) == 'a') {
            startGame(true); // Advanced mode
        } else if (tolower(selectedMode) == 'q') {
            startGame(false); // Quickie mode
        } else if (tolower(selectedMode) == 'l') {
            showLeaderboard(); // Show leaderboard
        } else if (tolower(selectedMode) == 'c') {
            customizeGame(); // Open customization menu
        } else if (tolower(selectedMode) == 'e' || selectedMode == 27) { // Exit
            return 0;
        } else {
            cout << "\nInvalid input.\n";
            Sleep(1000); // Pause for 1 second to show the error
        }
    }

    return 0;
}