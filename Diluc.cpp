#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = static_cast<double>(clock()) / CLOCKS_PER_SEC;
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Game {
public:
    int score = 0;
    bool gameOver = false;

    void HandleInput()
    {
        // Handle user input here (e.g., move block, quit game, etc.)
        char input;
        cin >> input;

        if (input == 'q')
        {
            gameOver = true;
        }
        // Additional input handling logic (e.g., moving pieces) would go here
    }

    void MoveBlockDown()
    {
        // Logic for moving block down (or any other game logic)
        if (!gameOver)
        {
            score += 10; // Example score increase
        }
    }

    void Draw()
    {
        // Draw game state to the console using text (e.g., ASCII art)
        system("clear");  // For Linux/macOS, use "cls" for Windows

        cout << "Score: " << score << endl;
        cout << "Next Block: (to be implemented)" << endl;
        if (gameOver)
        {
            cout << "GAME OVER" << endl;
        }
    }
};

int main()
{
    Game game;

    while (!game.gameOver)
    {
        game.HandleInput();
        if (EventTriggered(0.2))  // Move block every 0.2 seconds
        {
            game.MoveBlockDown();
        }

        game.Draw();
        
        // Sleep to maintain game loop timing
        this_thread::sleep_for(chrono::milliseconds(100));  // Target FPS ~60
    }

    cout << "Game Over! Final Score: " << game.score << endl;

    return 0;
}
