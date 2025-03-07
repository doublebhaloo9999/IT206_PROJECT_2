#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h> // For _kbhit() and _getch()

using namespace std;

const int width = 10;
const int height = 20;
vector<vector<int>> grid(height, vector<int>(width, 0));

// Tetromino shapes
vector<vector<vector<int>>> tetrominos = {
    {{1, 1, 1, 1}}, // I
    {{1, 1, 1}, {0, 1, 0}}, // T
    {{1, 1, 0}, {0, 1, 1}}, // Z
    {{0, 1, 1}, {1, 1, 0}}, // S
    {{1, 1}, {1, 1}}, // O
    {{1, 1, 1}, {1, 0, 0}}, // L
    {{1, 1, 1}, {0, 0, 1}} // J
};

struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
};

Tetromino currentTetromino;

void initialize() {
    srand(time(0));
    currentTetromino.shape = tetrominos[rand() % tetrominos.size()];
    currentTetromino.x = width / 2 - currentTetromino.shape[0].size() / 2;
    currentTetromino.y = 0;
}

bool checkCollision(int newX, int newY, vector<vector<int>> newShape) {
    for (int i = 0; i < newShape.size(); ++i) {
        for (int j = 0; j < newShape[i].size(); ++j) {
            if (newShape[i][j] && (newX + j < 0 || newX + j >= width || newY + i >= height || grid[newY + i][newX + j])) {
                return true;
            }
        }
    }
    return false;
}

void placeTetromino() {
    for (int i = 0; i < currentTetromino.shape.size(); ++i) {
        for (int j = 0; j < currentTetromino.shape[i].size(); ++j) {
            if (currentTetromino.shape[i][j]) {
                grid[currentTetromino.y + i][currentTetromino.x + j] = 1;
            }
        }
    }
}

void clearLines() {
    for (int i = height - 1; i >= 0; --i) {
        bool fullLine = true;
        for (int j = 0; j < width; ++j) {
            if (!grid[i][j]) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), vector<int>(width, 0));
            ++i;
        }
    }
}

void rotateTetromino() {
    vector<vector<int>> newShape(currentTetromino.shape[0].size(), vector<int>(currentTetromino.shape.size()));
    for (int i = 0; i < currentTetromino.shape.size(); ++i) {
        for (int j = 0; j < currentTetromino.shape[i].size(); ++j) {
            newShape[j][currentTetromino.shape.size() - 1 - i] = currentTetromino.shape[i][j];
        }
    }
    if (!checkCollision(currentTetromino.x, currentTetromino.y, newShape)) {
        currentTetromino.shape = newShape;
    }
}

void draw() {
    system("cls");
    // Draw top border
    for (int i = 0; i < width + 2; ++i) {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; ++i) {
        cout << "#"; // Left border
        for (int j = 0; j < width; ++j) {
            if (grid[i][j]) {
                cout << "1";
            } else {
                bool isTetromino = false;
                for (int k = 0; k < currentTetromino.shape.size(); ++k) {
                    for (int l = 0; l < currentTetromino.shape[k].size(); ++l) {
                        if (currentTetromino.shape[k][l] && currentTetromino.y + k == i && currentTetromino.x + l == j) {
                            cout << "1";
                            isTetromino = true;
                            break;
                        }
                    }
                    if (isTetromino) break;
                }
                if (!isTetromino) cout << " ";
            }
        }
        cout << "#"; // Right border
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; ++i) {
        cout << "#";
    }
    cout << endl;
}

void gameLoop() {
    while (true) {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a':
                    if (!checkCollision(currentTetromino.x - 1, currentTetromino.y, currentTetromino.shape)) {
                        currentTetromino.x--;
                    }
                    break;
                case 'd':
                    if (!checkCollision(currentTetromino.x + 1, currentTetromino.y, currentTetromino.shape)) {
                        currentTetromino.x++;
                    }
                    break;
                case 's':
                    if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
                        currentTetromino.y++;
                    }
                    break;
                case 'w':
                    rotateTetromino();
                    break;
            }
        }

        if (!checkCollision(currentTetromino.x, currentTetromino.y + 1, currentTetromino.shape)) {
            currentTetromino.y++;
        } else {
            placeTetromino();
            clearLines();
            currentTetromino.shape = tetrominos[rand() % tetrominos.size()];
            currentTetromino.x = width / 2 - currentTetromino.shape[0].size() / 2;
            currentTetromino.y = 0;
            if (checkCollision(currentTetromino.x, currentTetromino.y, currentTetromino.shape)) {
                cout << "Game Over!" << endl;
                break;
            }
        }

        draw();
        _sleep(500); // Adjust the speed of the game
    }
}

int main() {
    initialize();
    gameLoop();
    return 0;
}