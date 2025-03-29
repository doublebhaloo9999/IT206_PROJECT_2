#include "menu.h"
#include "utils.h"
#include <iostream>
#include <windows.h>

using namespace std;

void displayHomeWindow() {
    cout << "Welcome to Tetris!" << endl;
    cout << "(Q) Quickie Mode" << endl;
    cout << "(A) Advanced Mode" << endl;
    cout << "(S) Show Scoreboard" << endl;
    cout << "(C) Customize" << endl;
    cout << "(E) Exit" << endl;
}

void displayPauseMenu() {
    cout << "Game Paused" << endl;
    cout << "(R) Restart" << endl;
    cout << "(C) Resume" << endl;
    cout << "(M) Customize" << endl;
    cout << "(E) Exit" << endl;
}