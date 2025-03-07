#include <iostream>
#include <conio.h>
#include <stdexcept>
#include <windows.h>
#include <ctime>
#include <limits>

using namespace std;


#define MAX_LENGTH 20
#define MAX_WIDTH 10

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;


void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int main()
{
    srand(time(0));
    try
    {
        initScreen();
        
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}