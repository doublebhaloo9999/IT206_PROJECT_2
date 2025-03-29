s#include "menu.h"
#include <iostream>
#include <windows.h>

using namespace std;

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

void displayPauseMenu() {
    cout << "Game Paused" << endl;
    cout << "(R) Restart" << endl;
    cout << "(C) Resume" << endl;
    cout << "(M) Customize" << endl;
    cout << "(E) Exit" << endl;
}