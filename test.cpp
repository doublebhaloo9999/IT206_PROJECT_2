#include <iostream>
#include <chrono>  // High-precision timing
#include <thread>  // For sleep
#include <conio.h> // For getch()

using namespace std;
using namespace std::chrono;

bool GameOver = false;

class Game {
public:
    bool isRunning;

    Game() {
        std::GameOver = false;
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'q') isRunning = false;  // Quit game
        }
    }

    void update() {
        // Game logic (moving pieces, checking collisions)
    }

    void draw() {
        system("cls");
        cout << "Game running... Press 'Q' to quit.\n";
    }

    void run() {
        
    }
};

int main() {
    Game game;
    

    const int targetFPS = 30;  // Set desired FPS
    const int frameDuration = 1000 / targetFPS;  // Time per frame (ms)

    while (!GameOver) {
        auto frameStart = high_resolution_clock::now();

        handleInput();  // Capture user input
        update();       // Game logic update
        draw();         // Redraw board

        auto frameEnd = high_resolution_clock::now();
        int elapsedTime = duration_cast<milliseconds>(frameEnd - frameStart).count();
    
        if (elapsedTime < frameDuration) {
                this_thread::sleep_for(milliseconds(frameDuration - elapsedTime));
            }

        }
    return 0;
}
