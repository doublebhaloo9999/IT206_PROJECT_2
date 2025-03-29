#include<iostream>
#include <ctime>    // For time()
#include <conio.h>  // For _getch()
#include <windows.h> // For Sleep()
#include "menu.h"
#include "game.h"
#include "customization.h"
#include "scoreboard.h"
#include "utils.h"

using namespace std;

int main() {
    srand(time(0)); // Initialize random seed

    while (true) {
        displayHomeWindow(); // Display the home window

        char mode;
        mode = _getch(); // Get user input without requiring Enter

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