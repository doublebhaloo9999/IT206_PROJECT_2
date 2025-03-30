const string RED = "\033[31m"; // ANSI escape code for red text
const string RESET = "\033[0m"; // Reset text color
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

void displayFrame(const string& frame) {
    clearScreen();
    cout << RED << frame << RESET << endl; // Display frame in red
    Sleep(600); // Windows-specific sleep function
}

void displayGameOver() {
    clearScreen();
    cout << RED
         << "\n"
         << "   ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗  \n"
         << "  ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗ \n"
         << "  ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝ \n"
         << "  ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║▄▄ ██║██║   ██║██╔══╝  ██╔═══╝  \n"
         << "  ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║      \n"
         << "   ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝      \n"
         << RESET << endl;
    Sleep(2000); // Pause for 2 seconds
}

int main() {
    string frames[] = {FRAME_PART1, FRAME_PART2, FRAME_PART1, FRAME_PART2};
    
    for (int i = 0; i < 10; i++) { // Loop animation
        displayFrame(frames[i % 4]);
    }

    displayGameOver(); // Display "Game Over" screen
    
    return 0;
}
