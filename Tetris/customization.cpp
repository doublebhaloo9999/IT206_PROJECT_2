#include "customization.h"
#include "utils.h"
#include <iostream>
#include <windows.h>

using namespace std;

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
        cout << "6. Back to Menu\n";
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
            case 6:
                return; // Back to main menu
            default:
                cout << "Invalid choice. Please try again.\n";
                cout << "Press any key to return...";
                _getch();
                break;
        }
    }
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
