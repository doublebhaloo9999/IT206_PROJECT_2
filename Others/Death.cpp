#include <iostream>
#include <windows.h> // For Sleep() and Windows API functions

#ifdef _WIN32
    #define CLEAR_SCREEN "CLS"
#else         
    #define CLEAR_SCREEN "clear"
#endif

using namespace std;

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

void clearScreen() {
    system(CLEAR_SCREEN);
}

void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetConsoleColor() {
    setConsoleColor(7); // Default console color
}

void setupConsole() {
    // Set console to UTF-8 encoding
    SetConsoleOutputCP(CP_UTF8);
}

void displayFrame(const string& frame) {
    clearScreen();
    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Set text color to bright red
    cout << frame << endl;
    resetConsoleColor();
    Sleep(600); // Windows-specific sleep function
}

void displayGameOver() {
    clearScreen();
    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Set text color to bright red
    cout << u8"\n"
         << u8"   ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗  \n"
         << u8"  ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗ \n"
         << u8"  ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝ \n"
         << u8"  ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║▄▄ ██║██║   ██║██╔══╝  ██╔═══╝  \n"
         << u8"  ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║      \n"
         << u8"   ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝      \n";
    resetConsoleColor();
    Sleep(2000); // Pause for 2 seconds
}

int main() {
    setupConsole(); // Set up the console for UTF-8 encoding

    string frames[] = {FRAME_PART1, FRAME_PART2, FRAME_PART1, FRAME_PART2};
    
    for (int i = 0; i < 10; i++) { // Loop animation
        displayFrame(frames[i % 4]);
    }

    displayGameOver(); // Display "Game Over" screen
    
    return 0;
}
