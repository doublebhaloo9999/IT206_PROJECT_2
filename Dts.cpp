#include <iostream>
#include <conio.h>  // For getch() and kbhit()
#include <windows.h> // For Sleep()

using namespace std;

const int WIDTH = 10;   // Board width
const int HEIGHT = 20;  // Board height

char board[HEIGHT][WIDTH]; // Game board

// Tetromino shapes (4x4 matrices)
char tetrominoes[7][4][4] = {
    { // {' ', ' ', ' ', ' '},
      {'X', 'X', 'X', 'X'},
      {' ', ' ', ' ', ' '},
    //   {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {'X', 'X', 'X', ' '},
      {'X', ' ', ' ', ' '},
    //   {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {'X', 'X', 'X', ' '},
      {' ', ' ', 'X', ' '},
    //  {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {'X', 'X', ' ', ' '},
      {'X', 'X', ' ', ' '},
    //   {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {'X', 'X', ' ', ' '},
      {' ', 'X', 'X', ' '},
    //   {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {' ', 'X', 'X', ' '},
      {'X', 'X', ' ', ' '},
    //   {' ', ' ', ' ', ' '} 
    },

    { // {' ', ' ', ' ', ' '},
      {'X', 'X', 'X', ' '},
      {' ', 'X', ' ', ' '},
    //   {' ', ' ', ' ', ' '} 
    }
};

// Tetromino class
class Tetromino {
public:
    int x, y; // Position on board
    int type; // Type of tetromino

    Tetromino(int t) {
        x = WIDTH / 2 - 2; // Start in middle
        y = 0;
        type = t;
    }

    void rotate() {
        char temp[4][4];
        // Rotate 90 degrees
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = tetrominoes[type][i][j];

        // Copy back
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tetrominoes[type][i][j] = temp[i][j];
    }

    bool canMove(int dx, int dy) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominoes[type][i][j] == 'X') {
                    int newX = x + j + dx;
                    int newY = y + i + dy;
                    if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && board[newY][newX] == 'X')) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void placeOnBoard() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (tetrominoes[type][i][j] == 'X' && y + i >= 0)
                    board[y + i][x + j] = 'X';
    }
};

// Game class
class Game {
public:
    Tetromino *currentTetromino;
    bool isRunning;

    Game() {
        isRunning = true;
        clearBoard();
        spawnTetromino();
    }

    void clearBoard() {
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
                board[i][j] = ' ';
    }

    void spawnTetromino() {
        int type = rand() % 7;
        currentTetromino = new Tetromino(type);
    }

    void clearFullRows() {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++) {
                if (board[i][j] == ' ') {
                    full = false;
                    break;
                }
            }
            if (full) {
                for (int k = i; k > 0; k--)
                    for (int j = 0; j < WIDTH; j++)
                        board[k][j] = board[k - 1][j];
            }
        }
    }

    void moveTetromino(int dx, int dy) {
        if (currentTetromino->canMove(dx, dy)) {
            currentTetromino->x += dx;
            currentTetromino->y += dy;
        } else if (dy > 0) { // Piece landed
            currentTetromino->placeOnBoard();
            clearFullRows();
            spawnTetromino();
        }
    }

    void rotateTetromino() {
        currentTetromino->rotate();
    }

    void drawBoard() {
        system("cls"); // Clear screen
        for (int i = 0; i < HEIGHT; i++) {
            cout << "#";
            for (int j = 0; j < WIDTH; j++)
                cout << (board[i][j] == 'X' ? "X" : " ");
            cout << "#\n";
        }
        cout << "##############\n";
        cout << "Controls: A - Left | D - Right | S - Down | W - Rotate\n";
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a') moveTetromino(-1, 0);
            if (key == 'd') moveTetromino(1, 0);
            if (key == 's') moveTetromino(0, 1);
            if (key == 'w') rotateTetromino();
        }
    }

    void run() {
        while (isRunning) {
            drawBoard();
            handleInput();
            moveTetromino(0, 1); // Gravity
            Sleep(500); // Adjust speed
        }
    }
};

// Main function
int main() {
    Game game;
    game.run();
    return 0;
}
